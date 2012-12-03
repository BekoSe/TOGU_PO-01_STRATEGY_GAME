#ifndef CBMPREADER_H
#define CBMPREADER_H


#include "stdio.h"
#include "stdlib.h"
#include "memory.h"

//Минимальный класс для работы с 24-битными несжатыми bmp, не выполняет никаких проверок!
typedef unsigned char vec4ub[4];
typedef unsigned char vec3ub[3];

class cBMPReader
{
private:
    int w;
    int h;
    vec3ub tc;
    vec4ub* img;
public:
    cBMPReader();
    void SetTransparentColor(unsigned char r, unsigned char g, unsigned char b); //Имеет смысл до вызова Load
    vec4ub* Load(char* filename, unsigned char t_zone=80);
    vec4ub* LoadWithAlpha(char* filebname); //Загружает из собственного бинарного вормата: width(4б), height(4б), img(4*width*height б)
    vec4ub* Img();
    vec4ub* ImgCpy();
    int Width();
    int Height();
    ~cBMPReader();
};

#endif // CBMPREADER_H
