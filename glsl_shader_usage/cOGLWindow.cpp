#include "cOGLWindow.h"

cOGLWindow::cOGLWindow()
{
	CurhWnd=NULL;
	NewhWnd=NULL; 
	dc=NULL;
	hrc=NULL;
	current=false;
	SetDefaultPixelFormat();
	CurPFD=NewPFD;
}
cOGLWindow::cOGLWindow(HWND window)
{
	cOGLWindow();
	NewhWnd=window;
}
void cOGLWindow::SetDefaultPixelFormat()
{
	NewPFD.nSize = sizeof (PIXELFORMATDESCRIPTOR); // ������ ���������
	NewPFD.nVersion = 1;                            // ����� ������
	NewPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; // ��������� ������� ������, ������������ ���������� � ���������
	NewPFD.iPixelType = PFD_TYPE_RGBA; // ����� ��� ����������� ������
	NewPFD.cColorBits = 24;            // ����� ������� ���������� � ������ ������ �����
	NewPFD.cRedBits = 0;               // ����� ������� ���������� �������� � ������ ������ RGBA
	NewPFD.cRedShift = 0;              // �������� �� ������ ����� ������� ���������� �������� � ������ ������ RGBA
	NewPFD.cGreenBits = 0;             // ����� ������� ���������� ������� � ������ ������ RGBA
	NewPFD.cGreenShift = 0;            // �������� �� ������ ����� ������� ���������� ������� � ������ ������ RGBA
	NewPFD.cBlueBits = 0;              // ����� ������� ���������� ������ � ������ ������ RGBA
	NewPFD.cBlueShift = 0;             // �������� �� ������ ����� ������� ���������� ������ � ������ ������ RGBA
	NewPFD.cAlphaBits = 0;             // ����� ������� ���������� ����� � ������ ������ RGBA
	NewPFD.cAlphaShift = 0;            // �������� �� ������ ����� ������� ���������� ����� � ������ ������ RGBA
	NewPFD.cAccumBits = 0;             // ����� ����� ������� ���������� � ������ ������������
	NewPFD.cAccumRedBits = 0;          // ����� ������� ���������� �������� � ������ ������������
	NewPFD.cAccumGreenBits = 0;        // ����� ������� ���������� ������� � ������ ������������
	NewPFD.cAccumBlueBits = 0;         // ����� ������� ���������� ������ � ������ ������������
	NewPFD.cAccumAlphaBits = 0;        // ����� ������� ���������� ����� � ������ ������������
	NewPFD.cDepthBits = 32;            // ������ ������ ������� (��� z)
	NewPFD.cStencilBits = 0;           // ������ ������ ���������
	NewPFD.cAuxBuffers = 0;            // ����� ��������������� �������
	NewPFD.iLayerType = PFD_MAIN_PLANE;// ��� ���������
	NewPFD.bReserved = 0;              // ����� ���������� ��������� � ������� �����
	NewPFD.dwLayerMask = 0;            // ������������
	NewPFD.dwVisibleMask = 0;          // ������ ��� ���� ������������ ������ ���������
	NewPFD.dwDamageMask = 0;           // ������������
}
void cOGLWindow::SetWindow(HWND window)
{
	NewhWnd=window;
}
//���������� ����� pfd
void cOGLWindow::SetFlags(DWORD v)
{
	NewPFD.dwFlags=v;
}
void cOGLWindow::SetPixelType(BYTE v)
{
	NewPFD.iPixelType=v;
}
void cOGLWindow::SetColorBits(BYTE v)
{
	NewPFD.cColorBits=v;
}
void cOGLWindow::SetRedBits(BYTE v)
{
	NewPFD.cRedBits=v;
}
void cOGLWindow::SetRedShift(BYTE v)
{
	NewPFD.cRedShift=v;
}
void cOGLWindow::SetGreenBits(BYTE v)
{
	NewPFD.cGreenBits=v;
}
void cOGLWindow::SetGreenShift(BYTE v)
{
	NewPFD.cGreenShift=v;
}
void cOGLWindow::SetBlueBits(BYTE v)
{
	NewPFD.cBlueBits=v;
}
void cOGLWindow::SetBlueShift(BYTE v)
{
	NewPFD.cBlueShift=v;
}
void cOGLWindow::SetAlphaBits(BYTE v)
{
	NewPFD.cAlphaBits=v;
}
void cOGLWindow::SetAlphaShift(BYTE v)
{
	NewPFD.cAlphaShift=v;
}
void cOGLWindow::SetAccumBits(BYTE v)
{
	NewPFD.cAccumBits=v;
}
void cOGLWindow::SetAccumRedBits(BYTE v)
{
	NewPFD.cAccumRedBits=v;
}
void cOGLWindow::SetAccumGreenBits(BYTE v)
{
	NewPFD.cAccumGreenBits=v;
}
void cOGLWindow::SetAccumBlueBits(BYTE v)
{
	NewPFD.cAccumBlueBits=v;
}
void cOGLWindow::SetAccumAlphaBits(BYTE v)
{
	NewPFD.cAccumAlphaBits=v;
}
void cOGLWindow::SetStencilBits(BYTE v)
{
	NewPFD.cStencilBits=v;
}
void cOGLWindow::SetAuxBuffers(BYTE v)
{
	NewPFD.cAuxBuffers=v;
}
void cOGLWindow::SetLayerType(BYTE v)
{
	NewPFD.iLayerType=v;
}
void cOGLWindow::SetLayerMask(DWORD v)
{
	NewPFD.dwLayerMask=v;
}
void cOGLWindow::SetVisibleMask(DWORD v)
{
	NewPFD.dwVisibleMask=v;
}
void cOGLWindow::SetDamageMask(DWORD v)
{
	NewPFD.dwDamageMask=v;
}
//����� ���������� ���������� pfd
PIXELFORMATDESCRIPTOR cOGLWindow::GetCurrentFormat()
{
	return CurPFD;
}
PIXELFORMATDESCRIPTOR cOGLWindow::GetRequiredFormat()
{
	return NewPFD;
}
void cOGLWindow::PrintFormat(char* filename, bool Current)
{
	PIXELFORMATDESCRIPTOR p;
	if (Current) p=CurPFD;
	else p=NewPFD;
	FILE* f=fopen(filename,"wt");
	if ((Current && hrc)||(!Current))
	{
		fprintf(f,"Pixel format is: \n");
		fprintf(f,"Size: %d\n",p.nSize);
		fprintf(f,"Version: %d\n",p.nVersion);
		fprintf(f,"Flags:\n");
			if (p.dwFlags&PFD_DRAW_TO_WINDOW) fprintf(f,"	PFD_DRAW_TO_WINDOW\n");
			if (p.dwFlags&PFD_DRAW_TO_BITMAP) fprintf(f,"	PFD_DRAW_TO_BITMAP\n");
			if (p.dwFlags&PFD_SUPPORT_GDI) fprintf(f,"	PFD_SUPPORT_GDI\n");
			if (p.dwFlags&PFD_SUPPORT_OPENGL) fprintf(f,"	PFD_SUPPORT_OPENGL\n");
			if (p.dwFlags&PFD_GENERIC_ACCELERATED) fprintf(f,"	PFD_GENERIC_ACCELERATED\n");
			if (p.dwFlags&PFD_GENERIC_FORMAT) fprintf(f,"	PFD_GENERIC_FORMAT\n");
			if (p.dwFlags&PFD_NEED_PALETTE) fprintf(f,"	PFD_NEED_PALETTE\n");
			if (p.dwFlags&PFD_NEED_SYSTEM_PALETTE) fprintf(f,"	PFD_NEED_SYSTEM_PALETTE\n");
			if (p.dwFlags&PFD_DOUBLEBUFFER) fprintf(f,"	PFD_DOUBLEBUFFER\n");
			if (p.dwFlags&PFD_STEREO) fprintf(f,"	PFD_STEREO\n");
			if (p.dwFlags&PFD_SWAP_LAYER_BUFFERS) fprintf(f,"	PFD_SWAP_LAYER_BUFFERS\n");
			if (p.dwFlags&PFD_DEPTH_DONTCARE) fprintf(f,"	PFD_DEPTH_DONTCARE\n");
			if (p.dwFlags&PFD_DOUBLEBUFFER_DONTCARE) fprintf(f,"	PFD_DOUBLEBUFFER_DONTCARE\n");
			if (p.dwFlags&PFD_STEREO_DONTCARE) fprintf(f,"	PFD_STEREO_DONTCARE\n");
			if (p.dwFlags&PFD_SWAP_COPY) fprintf(f,"	PFD_SWAP_COPY\n");
			if (p.dwFlags&PFD_SWAP_EXCHANGE) fprintf(f,"	PFD_SWAP_EXCHANGE\n");
		fprintf(f,"PixelType:\n");
			if (p.iPixelType&PFD_TYPE_RGBA) fprintf(f,"	PFD_TYPE_RGBA\n");
			if (p.iPixelType&PFD_TYPE_COLORINDEX) fprintf(f,"	PFD_TYPE_COLORINDEX\n");
		fprintf(f,"ColorBits: %d\n",p.cColorBits);
		fprintf(f,"RedBits: %d\n",p.cRedBits);
		fprintf(f,"RedShift: %d\n",p.cRedShift);
		fprintf(f,"GreenBits: %d\n",p.cGreenBits);
		fprintf(f,"GreenShift: %d\n",p.cGreenShift);
		fprintf(f,"BlueBits: %d\n",p.cBlueBits);
		fprintf(f,"BlueShift: %d\n",p.cBlueShift);
		fprintf(f,"AlphaBits: %d\n",p.cAlphaBits);
		fprintf(f,"AlphaShift: %d\n",p.cAlphaShift);
		fprintf(f,"AccumBits: %d\n",p.cAccumBits);
		fprintf(f,"AccumRedBits: %d\n",p.cAccumRedBits);
		fprintf(f,"AccumGreenBits: %d\n",p.cAccumGreenBits);
		fprintf(f,"AccumBlueBits: %d\n",p.cAccumBlueBits);
		fprintf(f,"AccumAlphaBits: %d\n",p.cAccumAlphaBits);
		fprintf(f,"DepthBits: %d\n",p.cDepthBits);
		fprintf(f,"StencilBits: %d\n",p.cStencilBits);
		fprintf(f,"AuxBuffers: %d\n",p.cAuxBuffers);
		fprintf(f,"LayerType: %d\n",p.iLayerType);
		fprintf(f,"Reserved: %d\n",p.bReserved);
		fprintf(f,"LayerMask: %d\n",p.dwLayerMask);
		fprintf(f,"VisibleMask: %d\n",p.dwVisibleMask);
		fprintf(f,"DamageMask: %d\n",p.dwDamageMask);		
	}
	else fprintf(f,"Current pixel format not defined");
	fclose(f);
}
void cOGLWindow::FreeContext()
{
	if (CurhWnd && hrc && current)
	{
		wglMakeCurrent (dc, 0); 
		EndPaint (CurhWnd, &ps);
		ReleaseDC (CurhWnd, dc);
		current=false;
	}
}
void cOGLWindow::RestoreContext()
{
	if (CurhWnd && hrc && !current)
	{
		dc = BeginPaint(CurhWnd, &ps);
		wglMakeCurrent (dc, hrc);
		current=true;
	}
}
bool cOGLWindow::IsContextCreated()
{
	if (CurhWnd && hrc) return true;
	return false;
}
bool cOGLWindow::IsContextCurrent()
{
	return current;
}
void cOGLWindow::Swap()
{
	SwapBuffers(dc);
}
void cOGLWindow::Create()
{
	if (CurhWnd) return;
	if (hrc) return;
	CurhWnd=NewhWnd;
	dc = GetDC(CurhWnd);
	//������������� ������ �������
    int nPixelFormat;
	CurPFD=NewPFD;
	nPixelFormat=ChoosePixelFormat(dc, &CurPFD);
	SetPixelFormat (dc, nPixelFormat, &CurPFD); 

    hrc=wglCreateContext(dc); 
    ReleaseDC (CurhWnd, dc);
}
void cOGLWindow::Update()
{
	Release();
	Create();
}
void cOGLWindow::Release()
{
	FreeContext();
	if (hrc) wglDeleteContext (hrc);
	CurhWnd=NULL;
	hrc=NULL;
}
cOGLWindow::~cOGLWindow()
{
	Release();
}