#ifndef C_OGL_WINDOW_H
#define C_OGL_WINDOW_H

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"Glu32.lib")
#include "windows.h"
#include "stdio.h"

#include "GL/gl.h"
#include "GL/glu.h"

class cOGLWindow
{
private:
	HWND CurhWnd; //Дескриптор окна вывода
	HWND NewhWnd; 
	PAINTSTRUCT ps;
	HDC dc;  //Контекст устройства
	HGLRC hrc; //Графический контекст
	PIXELFORMATDESCRIPTOR CurPFD; //Текущая структура формата пикселя
	PIXELFORMATDESCRIPTOR NewPFD; //Требуемая в будущем структура формата пикселя
	bool current;
public:
	cOGLWindow();
	cOGLWindow(HWND window);
	void SetWindow(HWND window);
	void SetDefaultPixelFormat();
	//Заполнение полей pfd
	void SetFlags(DWORD v);
	void SetPixelType(BYTE v);
	void SetColorBits(BYTE v);
	void SetRedBits(BYTE v);
	void SetRedShift(BYTE v);
	void SetGreenBits(BYTE v);
	void SetGreenShift(BYTE v);
	void SetBlueBits(BYTE v);
	void SetBlueShift(BYTE v);
	void SetAlphaBits(BYTE v);
	void SetAlphaShift(BYTE v);
	void SetAccumBits(BYTE v);
	void SetAccumRedBits(BYTE v);
	void SetAccumGreenBits(BYTE v);
	void SetAccumBlueBits(BYTE v);
	void SetAccumAlphaBits(BYTE v);
	void SetStencilBits(BYTE v);
	void SetAuxBuffers(BYTE v);
	void SetLayerType(BYTE v);
	void SetLayerMask(DWORD v);
	void SetVisibleMask(DWORD v);
	void SetDamageMask(DWORD v);
	//Конец заполнения структурры pfd
	PIXELFORMATDESCRIPTOR GetCurrentFormat();
	PIXELFORMATDESCRIPTOR GetRequiredFormat();
	void PrintFormat(char* filename, bool Current=true);

	void FreeContext();
	void RestoreContext();
	bool IsContextCreated();
	bool IsContextCurrent();

	void Swap();
	void Create();
	void Update();
	void Release();
	~cOGLWindow();
};


#endif