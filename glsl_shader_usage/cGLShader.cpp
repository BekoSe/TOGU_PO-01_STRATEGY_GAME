#include "cGLShader.h"

cShader::cShader()
{
	Supported=false;
	if (isExtensionSupported("ARB_shader_objects") && 
		isExtensionSupported("ARB_vertex_shader") &&
		isExtensionSupported("ARB_fragment_shader"))
		Supported=true;
	else
		return;
	//Определяем особенности реализациии
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB,&Info.MaxVertexAttributes);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB,&Info.MaxVertexUniformComponents);
	glGetIntegerv(GL_MAX_VARYING_FLOATS_ARB,&Info.MaxVaryingFloats);
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB,&Info.MaxVertexTextureImageUnits);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB,&Info.MaxCombinedTextureImageUnits);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB,&Info.MaxTextureImageUnits);
	glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB,&Info.MaxTextureCoords);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB,&Info.MaxFragmentUniformComponents);
	glGetIntegerv(GL_MAX_TEXTURE_UNITS,&Info.MaxTextureUnits);
	glGetIntegerv(GL_MAX_LIGHTS,&Info.MaxLights);
	glGetIntegerv(GL_MAX_CLIP_PLANES,&Info.MaxClipPlanes);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS,&Info.MaxDrawBuffers);
	//Инициализируем свойства
	prog=glCreateProgramObjectARB();
	if (!prog) 
	{
		Supported=false;
		return;
	}
	LinkInfo=NULL;
	linked=false;
	correct=false;
	current=false;
	VertObj=NULL;
	FragObj=NULL;
}
void cShader::PrintInfo(char* filename)
{
	FILE* f=fopen(filename,"wt");
	if (!f) return;
	if (Supported)
	{
		fprintf(f,"GLSL is supported.\n");
		fprintf(f,"Max vertex attributes: %d\n",Info.MaxVertexAttributes);
		fprintf(f,"Max vertex uniform components: %d\n",Info.MaxVertexUniformComponents);
		fprintf(f,"Max varying floats: %d\n",Info.MaxVaryingFloats);
		fprintf(f,"Max vertex texture image units: %d\n",Info.MaxVertexTextureImageUnits);
		fprintf(f,"Max combined texture image units: %d\n",Info.MaxCombinedTextureImageUnits);
		fprintf(f,"Max texture image units: %d\n",Info.MaxTextureImageUnits);
		fprintf(f,"Max texture coords: %d\n",Info.MaxTextureCoords);
		fprintf(f,"Max fragment uniform components: %d\n",Info.MaxFragmentUniformComponents);
		fprintf(f,"Max texture units: %d\n",Info.MaxTextureUnits);
		fprintf(f,"Max lights: %d\n",Info.MaxLights);
		fprintf(f,"Max clip planes: %d\n",Info.MaxClipPlanes);
		fprintf(f,"Max draw buffers: %d\n",Info.MaxDrawBuffers);
	}
	else fprintf(f,"GLSL is not supported.");
	fclose(f);
}
bool cShader::AddShaderObject(ShaderObj* &stack, char* code, int index, bool type)
{
	if (!Supported) return false;
	ShaderObj* NewShader;
	if (stack && index<0) NewShader=stack; 
	else if (!stack) //Первая добавляемая фрагментная программа
	{
		stack=new ShaderObj;
		stack->next=NULL;
		stack->prev=NULL;
		stack->index=index;
		stack->info=NULL;
		stack->compiled=false;
		stack->correct=false;
		stack->obj=NULL;
		NewShader=stack;
	}
	else
	{
		//Пробуем найти фрагментный объект с заданным индексом
		ShaderObj* buf=stack;
		bool is=false;
		while (buf)
		{
			if (buf->index==index) 
			{
				NewShader=buf;
				is=true;
				break;
			}
			buf=buf->next;
		}
		if (!is)
		{
			NewShader=new ShaderObj;
			NewShader->next=stack;
			NewShader->prev=NULL;
			NewShader->compiled=false;
			NewShader->correct=false;
			NewShader->index=index;
			NewShader->info=NULL;
			NewShader->obj=NULL;
			stack->prev=NewShader;
			stack=NewShader;		
		}
	}
	//Теперь можно работать с объектом
	if (!NewShader->obj)
	{
		if (type)
		   NewShader->obj=glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		else
		  NewShader->obj=glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		if (NewShader->obj==NULL)
		{
			NewShader->correct=false;
			NewShader->compiled=false;
			NewShader->info=NULL;
			return false;
		}
		glAttachObjectARB(prog,NewShader->obj);
	}
	const char* source=code;
	//Загружаем исходный код в шейдер
	glShaderSourceARB(NewShader->obj,1,&source,NULL);
	//Компилируем шейдерный объект
	glCompileShaderARB(NewShader->obj);
	NewShader->compiled=true;
	//Определяем статус компиляции
	GLint res;
	glGetObjectParameterivARB(NewShader->obj,GL_OBJECT_COMPILE_STATUS_ARB,&res);
	if (res==GL_TRUE) NewShader->correct=true;
	else NewShader->correct=false;
	//Заполняем строку информации
	if (NewShader->info) delete[] NewShader->info;
	NewShader->info=NULL;
	int size;
	glGetObjectParameterivARB(NewShader->obj,GL_OBJECT_INFO_LOG_LENGTH_ARB, &size);
	if (size>0)
	{
		NewShader->info=new char[size];
		glGetInfoLogARB(NewShader->obj,size,NULL,NewShader->info);
	}
	return NewShader->correct;
}
void cShader::DeleteShaderObject(ShaderObj* &stack, int index)
{
	if (!Supported) return;
	if (index<0) //Нужно удалить все объекты
	{
		ShaderObj* buf=stack;
		while (buf)
		{
			if (stack->info) delete[] stack->info;
			if (stack->obj)
			{
				glDetachObjectARB(prog,stack->obj);
				glDeleteObjectARB(stack->obj);
			}
			buf=buf->next;
			delete stack;
			stack=buf;
		}
	}
	else
	{
		ShaderObj* buf=stack;
		while (buf)
		{
			if (buf->index==index)
			{
				if (buf->info) delete[] buf->info;
				if (buf->obj)
				{
					glDetachObjectARB(prog,buf->obj);
					glDeleteObjectARB(buf->obj);
				}
				//Меняем ссылки
				if (buf->next) buf->next->prev=buf->prev;
				if (buf->prev) buf->prev->next=buf->next;
				else stack=buf->next;
				delete buf;
				break;
			}
			buf=buf->next;
		}
	}
}
bool cShader::AddFragmentShader(char* code, int index)
{
	return AddShaderObject(FragObj,code,index,true);
}
bool cShader::AddVertexShader(char* code, int index)
{
	return AddShaderObject(VertObj,code,index,false);
}
bool cShader::fAddFragmentShader(char* filename, int index)
{
	FILE* f=fopen(filename,"rb");
	if (!f) return false;
	fseek(f,0,SEEK_END);
	int size=ftell(f);
	fseek(f,0,SEEK_SET);
	char* str=new char[size+1];
	str[size]=0;
	fread(str,size,1,f);
	fclose(f);
	return AddShaderObject(FragObj,str,index,true);
}
bool cShader::fAddVertexShader(char* filename, int index)
{
	FILE* f=fopen(filename,"rb");
	if (!f) return false;
	fseek(f,0,SEEK_END);
	int size=ftell(f);
	fseek(f,0,SEEK_SET);
	char* str=new char[size+1];
	str[size]=0;
	fread(str,size,1,f);
	fclose(f);
	return AddShaderObject(VertObj,str,index,false);
}
void cShader::DeleteFragmentShader(int index)
{
	DeleteShaderObject(FragObj,index);
}
void cShader::DeleteVertexShader(int index)
{
	DeleteShaderObject(VertObj,index);
}
void cShader::PrintVertexShaderInfo(char* filename, int index)
{
	if (!Supported) return;
	if (!VertObj) return;
	bool choosen=true;
	ShaderObj* buf=VertObj;
	if (index>0)
	{
		choosen=false;
		while(buf)
		{
			if (buf->index==index)
			{
				choosen=true;
				break;
			}
			buf=buf->next;
		}
	}
	if (!choosen) return;
	FILE* f=fopen(filename,"wt");
	if (!f) return;
	if (buf->compiled)
	{
		fprintf(f,"Status: compiled\n");
		if (buf->correct) fprintf(f, "Compiled successfully\n");
		else fprintf(f,"Compilation failed\n");
		if (buf->info) fprintf(f,"%s",buf->info);
	}
	else 
		fprintf(f,"Status: not compilled\n");
	fclose(f);
}
void cShader::PrintFragmentShaderInfo(char* filename, int index)
{
	if (!Supported) return;
	if (!FragObj) return;
	bool choosen=true;
	ShaderObj* buf=FragObj;
	if (index>0)
	{
		choosen=false;
		while(buf)
		{
			if (buf->index==index)
			{
				choosen=true;
				break;
			}
			buf=buf->next;
		}
	}
	if (!choosen) return;
	FILE* f=fopen(filename,"wt");
	if (!f) return;
	if (buf->compiled)
	{
		fprintf(f,"Status: compiled\n");
		if (buf->correct) fprintf(f, "Compiled successfully\n");
		else fprintf(f,"Compilation failed\n");
		if (buf->info) fprintf(f,"%s",buf->info);
	}
	else 
		fprintf(f,"Status: not compilled\n");
	fclose(f);
}
bool cShader::LinkProgram()
{
	if (!Supported) return false;
	glLinkProgramARB(prog);
	linked=true;
	GLint res;
	glGetObjectParameterivARB(prog,GL_OBJECT_LINK_STATUS_ARB,&res);
	if (res==GL_TRUE) correct=true;
	else correct=false;
	//Заполняем строку информации
	if (LinkInfo) delete[] LinkInfo;
	LinkInfo=NULL;
	int size;
	glGetObjectParameterivARB(prog,GL_OBJECT_INFO_LOG_LENGTH_ARB, &size);
	if (size>0)
	{
		LinkInfo=new char[size];
		glGetInfoLogARB(prog,size,NULL,LinkInfo);
	}
	return correct;
}
void cShader::PrintProgramInfo(char* filename)
{
	if (!Supported) return;
	FILE* f=fopen(filename,"wt");
	if (!f) return;
	if (linked)
	{
		fprintf(f,"Status: linked\n");
		if (correct) fprintf(f, "Linked successfully\n");
		else fprintf(f,"Linking failed");
		if (LinkInfo) fprintf(f,"%s",LinkInfo);
	}
	else 
		fprintf(f,"Status: not compilled\n");
	fclose(f);
}
void cShader::MakeCurrent()
{
	if (!Supported) return;
	if (current) return;
	if (!correct) return;
	glUseProgramObjectARB(prog);
	current=true;
}
void cShader::Stop()
{
	if (!Supported) return;
	if (!current) return;
	if (!correct) return;
	glUseProgramObjectARB(NULL);
	current=false;
}
//Функции передачи переменных в шейдеры
bool cShader::BindAttribute(int index, const char* name)
{
	glBindAttribLocationARB(prog,index,name);
	return true;
}
int cShader::GetAttributeIndex(const char* name)
{
	return glGetAttribLocationARB(prog,name);
}
int cShader::GetUniformLocation(const char* name)
{
	return glGetUniformLocationARB(prog,name);
}
//Конец функций передачи переменных в шейдеры
void cShader::FreeMem()
{
	if (!Supported) return;
	DeleteFragmentShader();
	DeleteVertexShader();
	Stop();
	glDeleteObjectARB(prog);
	if (LinkInfo) delete[] LinkInfo;
	LinkInfo=NULL;
}
cShader::~cShader()
{
	FreeMem();
}
//Конец класса шейдерной программы
