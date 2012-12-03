#include "cGameObject.h"

cScene* cGameObject::scene;
cMap* cGameObject::map;
cParam* cGameObject::params;


cGameObject::cGameObject()
{    
    pos[0]=0.0;
    pos[1]=0.0;
    //if (map) pos[2]=map->GetH(pos[0],pos[1]);
    //else pos[2]=0.0;
    pos[2]=0.0;
    rad=0.5;
    width=1.0;
    height=1.5;

    type_id=BASE_GAME_OBJECT; //Идентификатор базового класса
    selectable=false; //По умолчанию объект нельзя выбирать
    attackable=true; //По умолчанию любой объект можно атаковать
    selected=false;
    visible=true;
    cur_state=0;
    max_life=100.0;
    cur_life=100.0;

    del_flag=false;
    obj_id=0;
}
void cGameObject::SetPointers(cMap* m, cScene* s, cParam* p) //Статический метод инициализации указателей на необходимые объекты
{
    map=m;
    scene=s;
    params=p;
}
void cGameObject::Select(bool multiple)
{
    //В данной функции должны быть перегружены все операции, связанные с выделением объекта
    if (selectable) selected=true;
}
void cGameObject::Unselect()
{
    //В данной функции должны быть перегружены все операции, связанные с снятием выделения
    selected=false;
}
void cGameObject::Action(float* click_pos, cGameObject* target)
{
    //Данная функция перегружается, если объект должен отвечать на команды   
}
void cGameObject::Calculate()
{
    //Функция определяет и рассчитывет действия объекта в зависимости от состояний, других объектов и т.д.   
    anim.NextFrame();
}
void cGameObject::Draw()
{
    if (!visible) return;
    //Функция отрисовки объектов (Все объекты отрисовываются одинаково)
    if (selected)
    {
        float lpw=width/max_life;
        float cp=width-(max_life-cur_life)*lpw;

        float r=width/2.0;
        float h=height*0.1+pos[2];
        glLineWidth(4.0);
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_LINE_LOOP);
            glVertex3f(r+pos[0],pos[1],h);            
            glVertex3f(0.5f*r+pos[0],0.866f*r+pos[1],h);
            glVertex3f(-0.5f*r+pos[0],0.866f*r+pos[1],h);
            glVertex3f(-r+pos[0],pos[1],h);
            glVertex3f(-0.5f*r+pos[0],-0.866f*r+pos[1],h);
            glVertex3f(0.5f*r+pos[0],-0.866f*r+pos[1],h);
        glEnd();

        glLineWidth(5.0f);
        glBegin(GL_LINES);
            glVertex3f(pos[0]-width*0.5,pos[1],height*1.1+pos[2]);
            glVertex3f(pos[0]-width*0.5+cp,pos[1],height*1.1+pos[2]);
            glColor3f(1.0,0.0,0.0);
            glVertex3f(pos[0]-width*0.5+cp,pos[1],height*1.1+pos[2]);
            glVertex3f(pos[0]+width*0.5,pos[1],height*1.1+pos[2]);
        glEnd();

    }
    if (!anim.a)
    {
        glColor3f(1.0,0.0,0.0);
        glBegin(GL_QUADS);
            glVertex3f(-0.5*width+pos[0],pos[1],pos[2]);
            glVertex3f(0.5*width+pos[0],pos[1],pos[2]);
            glVertex3f(0.5*width+pos[0],pos[1],pos[2]+height);
            glVertex3f(-0.5*width+pos[0],pos[1],pos[2]+height);
        glEnd();
    }
    else
    {
        anim.Bind();
        frame_tex_coord &tc=anim.get_tc();

        glBegin(GL_QUADS);
            glTexCoord2fv(tc[0]);
            glVertex3f(-0.5*width+pos[0],pos[1],pos[2]);
            glTexCoord2fv(tc[1]);
            glVertex3f(0.5*width+pos[0],pos[1],pos[2]);
            glTexCoord2fv(tc[2]);
            glVertex3f(0.5*width+pos[0],pos[1],pos[2]+height);
            glTexCoord2fv(tc[3]);
            glVertex3f(-0.5*width+pos[0],pos[1],pos[2]+height);
        glEnd();

        anim.Unbind();
    }
}
void cGameObject::Pos(float* p)
{
    p[0]=pos[0];
    p[1]=pos[1];
    p[2]=pos[2];
}
float cGameObject::Rad()
{
    return rad;
}
bool cGameObject::IsSelectable()
{
    return selectable;
}
bool cGameObject::IsSelected()
{
    return selected;
}
bool cGameObject::IsAttackable()
{
    return attackable;
}
float cGameObject::Attack(float power)
{
    cur_life-=power;
    if (cur_life<0.0)
    {
        power+=cur_life;
        cur_life=0.0;
    }
    return power;
}
bool cGameObject::ReadyToDel()
{
    return del_flag;
}
void cGameObject::SetIndex(int id)
{
    obj_id=id;
}
int  cGameObject::GetIndex()
{
    return obj_id;
}
cGameObject::~cGameObject()
{
    //Удаляем память выделенную конкретным объектом
}
