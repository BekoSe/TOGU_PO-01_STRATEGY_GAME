// example00.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "example00.h"
#include "cOGLWindow.h"
#include "cGLShader.h"
#include "libExt.h"
#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

cOGLWindow glw; //������ ��� ������������� opengl
cShader* program; //��������� ��������� - ���������� ������ ����������, ���������������� ����������� libExt
float tm; //��������, �������� �����

HWND hWnd; //��������� ����
//������� ���������� ������� ����
int ClientWidth=0;
int ClientHeight=0;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EXAMPLE00, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_EXAMPLE00);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_EXAMPLE00);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_EXAMPLE00;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   //����� ���������� ��������� ����
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	float frw,cf;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		tm=0.0;
		glw.SetWindow(hWnd); //������� ��������� ����
		glw.Create();  //������ �������� OpenGL
		glw.RestoreContext(); //������ �������� ������� (����� ����� ������������ ������ OpenGL)
			
		initExtensions();//�������������� ������� ���������� OpenGL
		//�������������� opengl
		glClearColor(0.1,0.1,0.7,1.0); //���� ���������� (���� ����)

		glEnable(GL_DEPTH_TEST);

		//��������� ������
		program=new cShader; //����� ������� ������ ��� ��������� ��������� Opengl
		program->fAddVertexShader("vertex.vert");		
		program->fAddFragmentShader("fragment.frag");
		program->LinkProgram();
		program->PrintProgramInfo("shader_debug.txt");
		
		
		glw.FreeContext(); //��������� �������� OpenGL (���� ������� opengl ������������ ������)
		break;
	case WM_PAINT:
		glw.RestoreContext(); //��������������� �������� OpenGl
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //������� ����� �����

		glLoadIdentity();
		gluLookAt(0.0,0.0,80.0,200.0,200.0,0.0,0.0,0.0,1.0);

		program->MakeCurrent();

		//��������� uniform ��������� - ������� ����������	
		glUniform1fARB(program->GetUniformLocation("tm"),tm);				
		//������ �����
		glBegin(GL_QUADS);		
		for (int i=0; i<400; i++)
		{
			for (int j=0; j<400; j++)
			{
				glVertex3f((float)i,(float)j,0.0);
				glVertex3f((float)(i+1),(float)j,0.0);
				glVertex3f((float)(i+1),(float)(j+1),0.0);
				glVertex3f((float)i,(float)(j+1),0.0);
			}
		}		
		glEnd();
		

		program->Stop();

		glw.Swap(); //������ ������� �������
		glw.FreeContext(); //����������� �������� OpenGL
		break;
	case WM_SIZE: //��������� �������� ����		
		ClientWidth=LOWORD(lParam);
		ClientHeight=HIWORD(lParam);
		glw.RestoreContext(); //��������������� �������� ���������
		glViewport (0, 0, ClientWidth, ClientHeight); //��������� ���� (����� ������ ���� � ������ ���� � ��������)
		glMatrixMode(GL_PROJECTION); //������ ������� ������� ��������
		glLoadIdentity();		//������������� ������� �������� ���������
		//������������� ���������� ��������� ������
		gluPerspective(60.0,(float)ClientWidth/(float)ClientHeight,5,1000.0);
		//����� �����, ������, ������, �������, �������, ������� ������� ���� ������
		glMatrixMode(GL_MODELVIEW);//������� ������� - ������� ������
		glLoadIdentity(); //�������� ��������� ������� ������
		glw.FreeContext(); //����������� ��������
		InvalidateRect(hWnd,NULL,false); //�������������� ����
		break;
	case WM_DESTROY:	
		glw.RestoreContext();
		delete program; //����������� ����������� �� ��� �������
		glw.FreeContext();
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam==32)
		{
			tm+=2.0;
			InvalidateRect(hWnd,NULL,false);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
