#ifndef GLC_1_0_1_H
#define GLC_1_0_1_H

#pragma comment(lib,"opengl32.lib")
#include "windows.h"

#include "GL/GL.h"
#include "GL/GLU.h"
#include "libExt.h"
#include "stdio.h"


struct ShaderObj
{
	int index;
	GLhandleARB obj;
	char* info;
	bool correct;
	bool compiled;
	ShaderObj* next;
	ShaderObj* prev;
};
class cShader
{
private:
	bool Supported;
	struct
	{
		int MaxVertexAttributes;
		int MaxVertexUniformComponents;
		int MaxVaryingFloats;
		int MaxVertexTextureImageUnits;
		int MaxCombinedTextureImageUnits;
		int MaxTextureImageUnits;
		int MaxTextureCoords;
		int MaxFragmentUniformComponents;
		int MaxTextureUnits;
		int MaxLights;
		int MaxClipPlanes;
		int MaxDrawBuffers;
		
	} Info;
	GLhandleARB prog;
	char* LinkInfo;
	bool linked;
	bool correct;
	bool current;
	ShaderObj* VertObj;
	ShaderObj* FragObj;
	bool AddShaderObject(ShaderObj* &stack, char* code, int index, bool type);
	void DeleteShaderObject(ShaderObj* &stack, int index);
public:
	cShader();
	//Функции возврата информации о реализации
	bool IsSupported(){return Supported;}
	int GetMaxVertexAttributes(){ return Info.MaxVertexAttributes;}
	int GetMaxVertexUniformComponents(){return Info.MaxVertexUniformComponents;}
	int GetMaxVaryingFloats(){return Info.MaxVaryingFloats;}
	int GetMaxVertexTextureImageUnits(){return Info.MaxVertexTextureImageUnits;}
	int GetMaxCombinedTextureImageUnits(){return Info.MaxCombinedTextureImageUnits;}
	int GetMaxTextureImageUnits(){return Info.MaxTextureImageUnits;}
	int GetMaxTextureCoords(){return Info.MaxTextureCoords;}
	int GetMaxFragmentUniformComponents(){return Info.MaxFragmentUniformComponents;}
	int GetMaxTextureUnits(){return Info.MaxTextureUnits;}
	int GetMaxLights(){return Info.MaxLights;}
	int GetMaxClipPlanes(){return Info.MaxClipPlanes;}
	int GetMaxDrawBuffers(){return Info.MaxDrawBuffers;}
	void PrintInfo(char* filename);
	//Конец функций возврата информации о реализации

	//Функции работы с шейдерным объектом и программой
	bool AddFragmentShader(char* code, int index=-1);
	bool AddVertexShader(char* code, int index=-1);
	bool fAddFragmentShader(char* filename, int index=-1);
	bool fAddVertexShader(char* filename, int index=-1);
	void DeleteFragmentShader(int index=-1);
	void DeleteVertexShader(int index=-1);
	void PrintVertexShaderInfo(char* filename, int index=-1);
	void PrintFragmentShaderInfo(char* filename, int index=-1);
	bool LinkProgram();
	void PrintProgramInfo(char* filename);
	void MakeCurrent();
	void Stop();
	//Конец функциям работы с шейдерным объектом и программой

	//Функции передачи переменных в шейдеры
    bool BindAttribute(int index, const char* name);
	int GetAttributeIndex(const char* name);
	int GetUniformLocation(const char* name);
	//Конец функций передачи переменных в шейдеры
	void FreeMem();
	~cShader();
};

#endif