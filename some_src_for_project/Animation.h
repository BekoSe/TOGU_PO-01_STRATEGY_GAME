#ifndef ANIMATION_H
#define ANIMATION_H

#include "cBMPReader.h"
#include "math.h"
#include "GL/gl.h"

typedef float frame_tex_coord[4][2]; //Тип для представления текстурных координат в каждом кадре анимации

//Структура, для хранения статических даных анимации
struct Animation
{
    int dam; //Количество направлений, для которых построена анимация
    int aam; //Количество типов анимаций на каждое из направлений
    int fr_per_dir; //Количество кадров на направление
    int* fr_am; //Количество кадров в каждом типе анимаций
    int* fr_shift; //Смещение каждой анимации в рамках одного направления
    bool* repeat; //Флаг повторяемости каждого типа анимации (по умолчанию все анимации повторяемы)  
    GLuint tex_id; //Идентивикатор текстуры с анимацией
    frame_tex_coord* buf; //Буффер для хранения анимации

    Animation()
    {
        dam=0;
        aam=0;
        fr_per_dir=0;
        fr_am=NULL;
        repeat=NULL;
        buf=NULL;
        fr_shift=NULL;        
        tex_id=0;        
    }
    Animation(char* img_name, char* desc_name)
    {
        dam=0;
        aam=0;
        fr_per_dir=0;
        fr_am=NULL;
        repeat=NULL;
        buf=NULL;
        fr_shift=NULL;        
        tex_id=0;        
        SetAnimation(img_name,desc_name);
    }
    bool IsLoaded()
    {
        if (buf) return true;
        return false;
    }

    void SetAnimation(char* img_name, char* desc_name) //Должен быть активен контекст OpenGL
    {
        FreeAnimation();
        cBMPReader bmp;        
        bmp.LoadWithAlpha(img_name);
        //Генерируем текстуру для анимации
        glGenTextures(1,&tex_id);
        glBindTexture(GL_TEXTURE_2D,tex_id);
        //Настраиваем параметры фильтрации для текстуры
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //Загружаем текстуру в видеопамять
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,bmp.Width(),bmp.Height(),0,GL_RGBA,GL_UNSIGNED_BYTE,bmp.Img());
        glBindTexture(GL_TEXTURE_2D,0);

        //Читаем данные описания анимации
        FILE* f=fopen(desc_name,"rb");
        fread(&dam,sizeof(int),1,f);
        fread(&aam,sizeof(int),1,f);
        fr_am=new int[aam];
        repeat=new bool[aam];
        fr_shift=new int[aam];
        fread(fr_am,sizeof(int)*aam,1,f);

        fr_per_dir=0;
        int cur=0;
        for (int i=0; i<aam; i++)
        {
            fr_shift[i]=cur;
            repeat[i]=true;
            fr_per_dir+=fr_am[i];
            cur+=fr_am[i];
        }
        buf=new frame_tex_coord[dam*fr_per_dir];
        fread(buf,sizeof(frame_tex_coord)*dam*fr_per_dir,1,f);
        fclose(f);        
    }        
    void FreeAnimation()
    {
        if (tex_id) glDeleteTextures(1,&tex_id);
        delete[] fr_am;
        delete[] repeat;
        delete[] buf;
        delete[] fr_shift;
        dam=0;
        aam=0;
        fr_per_dir=0;
        fr_am=NULL;
        repeat=NULL;
        buf=NULL;
        fr_shift=NULL;
        tex_id=0;
    }
    ~Animation()
    {
        FreeAnimation();
    }
};
//Структура для хранения данных анимации, уникальных для каждого объекта
struct SelfAnimation
{
    Animation* a; //Указатель на статическую часть анимации
    bool play; //Флаг проигрывания анимации
    int cur_animation; //Текущая анимация
    int cur_frame; //Текущий кадр анимации
    int cur_dir; //Текущее направление объекта

    SelfAnimation()
    {
        a=NULL;
        cur_animation=0;
        cur_frame=0;
        cur_dir=0;
        play=false;
    }
    void SetDir(int dir)
    {
        cur_dir=dir;
        if (cur_dir>=a->dam) cur_dir=a->dam-1;
    }
    void SetDir(float ang)
    {
        float da=2*3.14159f/(float)a->dam;
        cur_dir=(int)(ang/da+0.5f);
        if (cur_dir>=a->dam) cur_dir=a->dam-1;
    }
    void SetDir(float x_dir, float y_dir)
    {
        float len=sqrtf(x_dir*x_dir+y_dir*y_dir);
        y_dir/=len;
        float ang=acos(-y_dir);
        if (x_dir<0.0) ang=2*3.14159f-ang;
        float da=2*3.14159f/(float)a->dam;
        cur_dir=(int)(ang/da+0.5f);
        if (cur_dir>=a->dam) cur_dir=a->dam-1;
    }
    frame_tex_coord& get_tc()
    {
        if (cur_dir>=a->dam) cur_dir=a->dam-1;
        if (cur_animation>=a->aam) cur_animation=a->aam-1;
        if (cur_frame>=a->fr_am[cur_animation]) cur_frame=a->fr_am[cur_animation]-1;

        return a->buf[cur_dir*a->fr_per_dir+a->fr_shift[cur_animation]+cur_frame];
    }
    void NextFrame()
    {
        if (!play) return;
        cur_frame++;
        if (cur_frame>=a->fr_am[cur_animation])
        {
            if (a->repeat[cur_animation]) cur_frame=0;
            else cur_frame=a->fr_am[cur_animation]-1;
        }
    }
    void Bind()
    {
        glBindTexture(GL_TEXTURE_2D,a->tex_id);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    }
    void Unbind()
    {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
    }
};

#endif // ANIMATION_H
