#include "cBMPReader.h"

cBMPReader::cBMPReader()
{
    w=0;
    h=0;
    img=NULL;
    tc[0]=255;
    tc[1]=255;
    tc[2]=255;
}
void cBMPReader::SetTransparentColor(unsigned char r, unsigned char g, unsigned char b)
{
    tc[0]=r;
    tc[1]=g;
    tc[2]=b;
}
vec4ub* cBMPReader::Load(char* filename, unsigned char t_zone)
{
    delete[] img;
    img=NULL;
    FILE* f=fopen(filename,"rb");
    if (!f) return NULL;
    fseek(f,10,SEEK_SET);
    int shift;
    fread(&shift,4,1,f);
    fseek(f,4,SEEK_CUR);
    fread(&w,4,1,f);
    fread(&h,4,1,f);
    fseek(f,shift,SEEK_SET);

    img=new vec4ub[w*h];
    vec3ub* line=new vec3ub[w+1];
    int rw=w*3;
    while (rw%4) rw++;

    for (int i=0; i<h; i++)
    {
        fread(line,rw,1,f);
        for (int j=0; j<w; j++)
        {
            img[i*w+j][0]=line[j][2];
            img[i*w+j][1]=line[j][1];
            img[i*w+j][2]=line[j][0];

            if (abs(line[j][2]-tc[0])<t_zone && abs(line[j][1]-tc[1])<t_zone && abs(line[j][0]-tc[2])<t_zone)
                img[i*w+j][3]=0;
            else
                img[i*w+j][3]=255;

        }
    }
    delete[] line;
    return img;
}
vec4ub* cBMPReader::LoadWithAlpha(char* filename)
{
    delete[] img;
    img=NULL;
    FILE* f=fopen(filename,"rb");
    if (!f) return NULL;
    fread(&w,sizeof(int),1,f);
    fread(&h,sizeof(int),1,f);
    img=new vec4ub[w*h];
    fread(img,sizeof(vec4ub)*w*h,1,f);
    fclose(f);
    return img;
}
vec4ub* cBMPReader::Img()
{
    return img;
}
vec4ub* cBMPReader::ImgCpy()
{
    if (!img) return NULL;
    vec4ub* buf=new vec4ub[w*h];
    memcpy(buf,img,sizeof(vec4ub)*w*h);
    return img;

}
int cBMPReader::Width()
{
    return w;
}
int cBMPReader::Height()
{
    return h;
}
cBMPReader::~cBMPReader()
{
    delete[] img;
}

