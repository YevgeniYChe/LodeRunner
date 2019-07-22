// Lode Runner.cpp : Defines the entry point for the application.
//

#include "stdafx.h"      // �������� <windows.h>
#include <gl\gl.h>       // ������������ ���� ��� OpenGL32 ����������
#include <gl\glu.h>      // ������������ ���� ��� GLu32 ����������
#include <gl\glaux.h>    // ������������ ���� ��� GLaux ����������
#include <time.h>        // ��������� ��� ������������� �������
#include <math.h>
#include <stdlib.h>
#include <mmsystem.h>


//--------------------------------------------( ���������� ���������� ���� )---
// ���������� ��������� ���� ///////////////////////////// ////( ��������� )///
enum _STATION   
{
    BEGIN,      // ������ ���� (������������ ��������)
	MENU,       // ����� ����
	GAME,       // ������� ������ ������ ����� �������� ���������
	_CORE,      // �������� ��������
	REDACTION,  // �������� �������
	RESULT,     // ����� ����������� (High Score)
	EXIT        // ����� �� ���� �� ���� ��������
} status=BEGIN;

// ��������� ����������� ������� ������� ������������ � ���������
enum _STATIC{
	SPACE,              // �������
	BRICK,	    		// ������
	IMBRICK,	    	// ������������� ������
	BEAM,	    		// �����������
	LADDER,			    // ��������
	FINISH_LADDER,  	// ��������� ��������
	LODE,				// ������ 
	RUNNER,			    // ������� �������� ����
	ENEMY,				// �����
    POLE,}				// ����
stat=SPACE;  

// �������� ��������� (��� ����� ��� � ����������)
enum _DIRECT{
	RIGHT,      // ������
	LEFT,       // �����
	UP,			// �����
	DOWN,		// ����
	NODOWN,     // ������
	STOP,       // ������������
	FIRE_LEFT,	// ����� �����  (������ �������)
	FIRE_RIGHT	// ����� ������ (������ �������)
};


// ������ ��� ���������� ���� ������� ������������ � ���
struct _SCORE{
	int  best_gold;
	int  current_gold;
	int  current_score;
	int  min_lev;
	int  max_lev;
	int  cur_lev;
	char name_best[255];
} score;

// ��������� �������������� ���� //////////////////////////////////( ����� )///
float begin_time=6;   // ����� ������ �������� � �������� 
float brick_time=3;   // ����� ���������� ������
   
int   enemy_step[3]={2,1};  // ����� ������ ���� ���������
int   main_step[3] ={2,2};  // ����� ������ ���� ����� ������
// ��������� ��������� ���� ////////////////////////////////////////( ���� )///
POINT mpos;
bool LM_Down=false;  // ������ ������� ����� ������ ����
bool RM_Down=false;

// ��������� ��������� ���������� ////////////////////////////( ���������� )///
bool keys[256]; // ������ ��� ��������� ��������� ����������

// �� ��������� ������� ///////////////////////////////////////( �������� )///
// 1 - �������� ����
const int  number_menu_texture=9;
char name_menu_texture[number_menu_texture][255]={
	"Screen/screen.bmp",     // ��� ��� ������ ����
	"Screen/start0.bmp",     // ������ start � ������� ����������
	"Screen/start1.bmp",     // ������ start
	"Screen/redaction0.bmp",
	"Screen/redaction1.bmp",
	"Screen/result0.bmp",
	"Screen/result1.bmp",
	"Screen/exit0.bmp",
	"Screen/exit1.bmp"};

// 2 - �������� ����� ����
const int number_field_texture=6;
char name_field_texture[number_field_texture][255]={
	"Field/up.bmp",
	"Field/down.bmp",
	"Field/1.bmp",
	"Field/2.bmp",
	"Field/3.bmp",
	"Field/4.bmp"};

// 3 - ��� ����������� �������� 
const int number_static_texture=10;
char name_static_texture[number_static_texture][255]={
	"Static/0.bmp",
	"Static/1.bmp",
	"Static/2.bmp",
	"Static/3.bmp",
	"Static/4.bmp",
	"Static/5.bmp",
	"Static/6.bmp",
	"Static/7.bmp",
	"Static/8.bmp",
	"Static/9.bmp"};

// 4 - ��������� ���� �� ���� �������
const int number_level=25; // 20-������� 5-�����
char levels[255]="Level/";
char res[255]="Level/result.txt"; // �������� ������ ��������� ����

// 5 - ��� �������� ������
const int number_human_texture=118; //50-18-50
char name_human_texture[number_human_texture][255]={
    "Human/main/right_image1.bmp",
	"Human/main/right_image2.bmp",
	"Human/main/right_image3.bmp",
	"Human/main/right_image4.bmp",
	"Human/main/right_image5.bmp",
	"Human/main/right_image6.bmp",
	"Human/main/right_mask1.bmp",
	"Human/main/right_mask2.bmp",
	"Human/main/right_mask3.bmp",
	"Human/main/right_mask4.bmp",
	"Human/main/right_mask5.bmp",
	"Human/main/right_mask6.bmp",
	"Human/main/left_image1.bmp",
	"Human/main/left_image2.bmp",
	"Human/main/left_image3.bmp",
	"Human/main/left_image4.bmp",
	"Human/main/left_image5.bmp",
	"Human/main/left_image6.bmp",
	"Human/main/left_mask1.bmp",
	"Human/main/left_mask2.bmp",
	"Human/main/left_mask3.bmp",
	"Human/main/left_mask4.bmp",
	"Human/main/left_mask5.bmp",
	"Human/main/left_mask6.bmp",
	"Human/main/du_image1.bmp",
	"Human/main/du_image2.bmp",
	"Human/main/du_image3.bmp",
	"Human/main/du_image4.bmp",
	"Human/main/du_image5.bmp",
	"Human/main/du_image6.bmp",
	"Human/main/du_mask1.bmp",
	"Human/main/du_mask2.bmp",
	"Human/main/du_mask3.bmp",
	"Human/main/du_mask4.bmp",
	"Human/main/du_mask5.bmp",
	"Human/main/du_mask6.bmp",
	"Human/main/nodown_image.bmp",
	"Human/main/nodown_mask.bmp",
	"Human/main/climb_right_image1.bmp",
	"Human/main/climb_right_image2.bmp",
	"Human/main/climb_right_image3.bmp",
	"Human/main/climb_right_mask1.bmp",
	"Human/main/climb_right_mask2.bmp",
	"Human/main/climb_right_mask3.bmp",
	"Human/main/climb_left_image1.bmp",
	"Human/main/climb_left_image2.bmp",
	"Human/main/climb_left_image3.bmp",
	"Human/main/climb_left_mask1.bmp",
	"Human/main/climb_left_mask2.bmp",
	"Human/main/climb_left_mask3.bmp",
	"Human/main/brick_image1.bmp",
	"Human/main/brick_image2.bmp",
	"Human/main/brick_image3.bmp",
	"Human/main/brick_mask1.bmp",
	"Human/main/brick_mask2.bmp",
	"Human/main/brick_mask3.bmp",
	"Human/main/fire_right_image1.bmp",
	"Human/main/fire_right_image2.bmp",
	"Human/main/fire_right_image3.bmp",
	"Human/main/fire_right_mask1.bmp",
	"Human/main/fire_right_mask2.bmp",
	"Human/main/fire_right_mask3.bmp",
	"Human/main/fire_left_image1.bmp",
	"Human/main/fire_left_image2.bmp",
	"Human/main/fire_left_image3.bmp",
	"Human/main/fire_left_mask1.bmp",
	"Human/main/fire_left_mask2.bmp",
	"Human/main/fire_left_mask3.bmp",
	"Human/enemy/right_image1.bmp",
	"Human/enemy/right_image2.bmp",
	"Human/enemy/right_image3.bmp",
	"Human/enemy/right_image4.bmp",
	"Human/enemy/right_image5.bmp",
	"Human/enemy/right_image6.bmp",
	"Human/enemy/right_mask1.bmp",
	"Human/enemy/right_mask2.bmp",
	"Human/enemy/right_mask3.bmp",
	"Human/enemy/right_mask4.bmp",
	"Human/enemy/right_mask5.bmp",
	"Human/enemy/right_mask6.bmp",
	"Human/enemy/left_image1.bmp",
	"Human/enemy/left_image2.bmp",
	"Human/enemy/left_image3.bmp",
	"Human/enemy/left_image4.bmp",
	"Human/enemy/left_image5.bmp",
	"Human/enemy/left_image6.bmp",
	"Human/enemy/left_mask1.bmp",
	"Human/enemy/left_mask2.bmp",
	"Human/enemy/left_mask3.bmp",
	"Human/enemy/left_mask4.bmp",
	"Human/enemy/left_mask5.bmp",
	"Human/enemy/left_mask6.bmp",
	"Human/enemy/du_image1.bmp",
	"Human/enemy/du_image2.bmp",
	"Human/enemy/du_image3.bmp",
	"Human/enemy/du_image4.bmp",
	"Human/enemy/du_image5.bmp",
	"Human/enemy/du_image6.bmp",
	"Human/enemy/du_mask1.bmp",
	"Human/enemy/du_mask2.bmp",
	"Human/enemy/du_mask3.bmp",
	"Human/enemy/du_mask4.bmp",
	"Human/enemy/du_mask5.bmp",
	"Human/enemy/du_mask6.bmp",
	"Human/enemy/nodown_image.bmp",
	"Human/enemy/nodown_mask.bmp",
	"Human/enemy/climb_right_image1.bmp",
	"Human/enemy/climb_right_image2.bmp",
	"Human/enemy/climb_right_image3.bmp",
	"Human/enemy/climb_right_mask1.bmp",
	"Human/enemy/climb_right_mask2.bmp",
	"Human/enemy/climb_right_mask3.bmp",
	"Human/enemy/climb_left_image1.bmp",
	"Human/enemy/climb_left_image2.bmp",
	"Human/enemy/climb_left_image3.bmp",
	"Human/enemy/climb_left_mask1.bmp",
	"Human/enemy/climb_left_mask2.bmp",
	"Human/enemy/climb_left_mask3.bmp"};

//------------------------------------------( ���������� ���������� OpenGL )---

LRESULT CALLBACK WndProc(HWND    hWnd,   UINT    message,
                         WPARAM  wParam, LPARAM  lParam);
static HGLRC hRC;               // ���������� �������� ����������
static HDC hDC;                 // ��������� �������� ���������� GDI

GLuint texture_human[number_human_texture];
GLuint tex_main[68];
GLuint tex_enemy[50];

//------------------------------------------------------------( ��� ������ )---
#include "GameManager.h"
GameManager manager;

//----------------------------------------------------------------( InitGL )---
// ������� ����� �������� ���� GL
//-----------------------------------------------------------------------------
GLvoid InitGL(GLsizei Width, GLsizei Height)    
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // ������� ������ � ������ ����
	glClearDepth(1.0);                      // ��������� ������� ������ �������
    glDepthFunc(GL_LESS);                   // ��� ����� �������
    glEnable(GL_DEPTH_TEST);                // ��������� ���� �������
	glShadeModel(GL_SMOOTH);        // ��������� ������� �������� �����������
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // ����������� �����
    glMatrixMode(GL_PROJECTION);    // ����� ������� ��������
    glLoadIdentity();               // ����� ������� ��������
	// ��������� ����������� �������������� �������� ��� ����
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);    
    glMatrixMode(GL_MODELVIEW);     // ����� ������� ��������� ������

	manager.GLobus._BuildFont();
}

//---------------------------------------------------------( ReSizeGLScene )---
// ������� ��� ��������� �������� ����
//-----------------------------------------------------------------------------
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height==0) Height=1; // �������������� ������� �� 0, ���� ���� ����
    // ����� ������� ������� ������ � ������������� ��������������       
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);            // ����� ������� ��������
    glLoadIdentity();                       // ����� ������� ��������

	glOrtho(0.0f,Width,Height,0.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);     // ����� ������� ��������� ������
	glLoadIdentity();
}

//-----------------------------------------------------------( DrawGLScene )---
// � ����������� ����� �������� �� ������ �������
//-----------------------------------------------------------------------------
GLvoid GameLand(GLvoid)
{
	switch(status) // ������������ ���� ����
	{
	case BEGIN:
		manager.game_begin();
		if(!((clock()/CLOCKS_PER_SEC)<begin_time)) status=MENU; // ����� ��������
		break;
	case MENU:
		status=manager.game_menu(mpos.x, mpos.y ,LM_Down);
		if(status==GAME)
		{
			score.cur_lev=1;
			score.current_score=0;
		}
		break;                                   
	case GAME:
		manager.game();
		break;
	case _CORE:	
		// ����� � ���������� ������� ��������, �� ���� � ����������� 
		// � ��������� �� ����� ������� �������
		static DWORD begin;
		timeBeginPeriod(1);
		begin=timeGetTime();
		manager._core();
		for(;timeGetTime()-begin<15;); // �������� ������ ��������� ���� �� ����� ������� �������
		timeEndPeriod(1);
		break;
	case REDACTION:
		manager.game_redaction(mpos.x, mpos.y ,LM_Down, RM_Down);
		break;
	case RESULT:
		manager.game_result();
		break;
	case EXIT:
		keys[VK_ESCAPE]=true;
		break;
	}
}


/////////////////////////////////////////////////////////////////( WinMain )///
// �������������� ������������ � ���������� ������� ����
///////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine, int       nCmdShow)
{
 	MSG             msg;    // ��������� ��������� Windows
    WNDCLASS        wc;     // ��������� ������ Windows ��� ��������� ���� ����
    HWND            hWnd;   // ���������� ����������� ����

	wc.style                = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc          = (WNDPROC) WndProc;
    wc.cbClsExtra           = 0;
    wc.cbWndExtra           = 0;
    wc.hInstance            = hInstance;
    wc.hIcon                = NULL;
    wc.hCursor              = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground        = NULL;
    wc.lpszMenuName         = NULL;
    wc.lpszClassName        = "Lode Runner";
	if(!RegisterClass(&wc))
    {
        MessageBox(0,"Failed To Register The Window Class.",
		"Error",MB_OK|MB_ICONERROR);
        return FALSE;
    }


	hWnd = CreateWindow("Lode Runner",
						"Lode Runner",

						WS_POPUP |
						WS_CLIPCHILDREN |
						WS_CLIPSIBLINGS,

						0, 0,                   // ������� ���� �� ������
						800, 600,               // ������ � ������ ����

						NULL,
						NULL,
						hInstance,
						NULL);
	if(!hWnd)
    {
        MessageBox(0,"Window Creation Error.","Error",MB_OK|MB_ICONERROR); 
        return FALSE;
    }

	// ������������ � ������ �����
	DEVMODE dmScreenSettings;                       // ����� ������

	memset(&dmScreenSettings, 0, sizeof(DEVMODE));  // ������� ��� ���������
	dmScreenSettings.dmSize = sizeof(DEVMODE);      // ������ ��������� Devmode
	dmScreenSettings.dmPelsWidth    = 800;          // ������ ������
	dmScreenSettings.dmPelsHeight   = 600;          // ������ ������
	dmScreenSettings.dmFields       = DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        
	ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

	for(;;)
    {
		// ��������� ���� ���������
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
			if (GetMessage(&msg, NULL, 0, 0))
            {
				TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else return TRUE;
        }
		SwapBuffers(hDC);     // ����������� ����� ������
		GameLand();           // ����� ����� � ��� ����
		if (keys[VK_ESCAPE]) SendMessage(hWnd,WM_CLOSE,0,0);  // ���� ESC - �����
    }
	return 0;
}

//---------------------------------------------------------------( WndProc )---
// ���������� ���� ���������
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND    hWnd,   UINT    message,
						 WPARAM  wParam, LPARAM  lParam)
{
	RECT    Screen;         // ������������ ������� ��� �������� ����
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���� ���������
        1,                              // ����� ������ (?)
        PFD_DRAW_TO_WINDOW |            // ������ ��� ����
        PFD_SUPPORT_OPENGL |            // ������ ��� OpenGL
        PFD_DOUBLEBUFFER,               // ������ ��� �������� ������
        PFD_TYPE_RGBA,                  // ��������� RGBA ������
        16,                             // ����� 16 ��� ������� �����
        0, 0, 0, 0, 0, 0,               // ������������� �������� ����� (?)
        0,                              // ��� ������ ������������
        0,                              // ��������� ��� ������������ (?)
        0,                              // ��� ������ �����������
        0, 0, 0, 0,                     // ���� ����������� ������������ (?)
        16,                             // 16 ������ Z-����� (����� �������) 
        0,                              // ��� ������ ����������
        0,                              // ��� ��������������� ������� (?)
        PFD_MAIN_PLANE,                 // ������� ���� ���������
        0,                              // ������ (?)
        0, 0, 0                         // ����� ���� ������������ (?)
    };

	switch (message)
    {
		case WM_CREATE:
            hDC = GetDC(hWnd);      // �������� �������� ���������� ��� ����
			// ����� ��������� ���������� ��� ������ ������� ��������
            PixelFormat = ChoosePixelFormat(hDC, &pfd);

			if (!PixelFormat)
            {
				MessageBox(0,"Can't Find A Suitable PixelFormat.",
				"Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

			if(!SetPixelFormat(hDC,PixelFormat,&pfd))
            {
                MessageBox(0,"Can't Set The PixelFormat.","Error",
				MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

			hRC = wglCreateContext(hDC);
			if(!hRC)
            {
                MessageBox(0,"Can't Create A GL Rendering Context.",
				"Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

			if(!wglMakeCurrent(hDC, hRC))
            {
                MessageBox(0,"Can't activate GLRC.",
				"Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

			GetClientRect(hWnd, &Screen);
            InitGL(Screen.right, Screen.bottom);
            break;

		case WM_DESTROY:
        case WM_CLOSE:
            ChangeDisplaySettings(NULL, 0);

            wglMakeCurrent(hDC,NULL);
            wglDeleteContext(hRC);
            ReleaseDC(hWnd,hDC);

            PostQuitMessage(0);
            break;

		case WM_KEYDOWN:
            keys[wParam] = TRUE;
            break;

		case WM_KEYUP:
            keys[wParam] = FALSE;
            break;

		case WM_LBUTTONDOWN:
			LM_Down=true;
			break;
			
		case WM_LBUTTONUP:
			LM_Down=false;
			break;

		case WM_RBUTTONDOWN:
			RM_Down=true;
			break;
			
		case WM_RBUTTONUP:
			RM_Down=false;
			break;

		case WM_MOUSEMOVE:
			mpos.x=LOWORD(lParam);
			mpos.y=HIWORD(lParam);
			break;

		case WM_SIZE:
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
            break;

		default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}









                        





