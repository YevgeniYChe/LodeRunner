// Lode Runner.cpp : Defines the entry point for the application.
//

#include "stdafx.h"      // Включает <windows.h>
#include <gl\gl.h>       // Заголовочный файл для OpenGL32 библиотеки
#include <gl\glu.h>      // Заголовочный файл для GLu32 библиотеки
#include <gl\glaux.h>    // Заголовочный файл для GLaux библиотеки
#include <time.h>        // Заголовок для использования времени
#include <math.h>
#include <stdlib.h>
#include <mmsystem.h>


//--------------------------------------------( Глобальные переменные игры )---
// Определяет состояние игры ///////////////////////////// ////( СОСТОЯНИЯ )///
enum _STATION   
{
    BEGIN,      // Начало игры (Показывается заставка)
	MENU,       // Вывод меню
	GAME,       // Игровой анализ уровня перед реальным действием
	_CORE,      // Реальное действие
	REDACTION,  // Редактор уровней
	RESULT,     // Показ результатов (High Score)
	EXIT        // Выход из игры по всем правилам
} status=BEGIN;

// Описывает статические обьекты которые показываются в редакторе
enum _STATIC{
	SPACE,              // Пустота
	BRICK,	    		// Кирпич
	IMBRICK,	    	// Непробиваемый кирпич
	BEAM,	    		// Перекладина
	LADDER,			    // Лестница
	FINISH_LADDER,  	// Финальная лестница
	LODE,				// Золото 
	RUNNER,			    // Главный персонаж игры
	ENEMY,				// Враги
    POLE,}				// поле
stat=SPACE;  

// Действия человечка (как моего так и противника)
enum _DIRECT{
	RIGHT,      // Вправо
	LEFT,       // Влево
	UP,			// Вверх
	DOWN,		// Вниз
	NODOWN,     // Падать
	STOP,       // Остановиться
	FIRE_LEFT,	// Огонь влево  (только главный)
	FIRE_RIGHT	// Огонь вправо (только главный)
};


// Хранит все переменные игры которые присутствуют в ней
struct _SCORE{
	int  best_gold;
	int  current_gold;
	int  current_score;
	int  min_lev;
	int  max_lev;
	int  cur_lev;
	char name_best[255];
} score;

// Временные характеристики игры //////////////////////////////////( ВРЕМЯ )///
float begin_time=6;   // Время показа заставки в секундах 
float brick_time=3;   // Время заростания кубика
   
int   enemy_step[3]={2,1};  // Время одного шага соперника
int   main_step[3] ={2,2};  // Время одного шага моего игрока
// Описывают состояние мыши ////////////////////////////////////////( МЫШЬ )///
POINT mpos;
bool LM_Down=false;  // Статус нажатия обоих кнопок мыши
bool RM_Down=false;

// Описывает состояние клавиатуры ////////////////////////////( КЛАВИАТУРА )///
bool keys[256]; // Массив для процедуры обработки клавиатуры

// Всё множество текстур ///////////////////////////////////////( ТЕКСТУРЫ )///
// 1 - Текстуры меню
const int  number_menu_texture=9;
char name_menu_texture[number_menu_texture][255]={
	"Screen/screen.bmp",     // Фон при выборе меню
	"Screen/start0.bmp",     // Кнопка start в отжатом положениее
	"Screen/start1.bmp",     // Кнопка start
	"Screen/redaction0.bmp",
	"Screen/redaction1.bmp",
	"Screen/result0.bmp",
	"Screen/result1.bmp",
	"Screen/exit0.bmp",
	"Screen/exit1.bmp"};

// 2 - текстуры сетки игры
const int number_field_texture=6;
char name_field_texture[number_field_texture][255]={
	"Field/up.bmp",
	"Field/down.bmp",
	"Field/1.bmp",
	"Field/2.bmp",
	"Field/3.bmp",
	"Field/4.bmp"};

// 3 - Все статические текстуры 
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

// 4 - Начальный путь ко всем уровням
const int number_level=25; // 20-Игровых 5-Своих
char levels[255]="Level/";
char res[255]="Level/result.txt"; // Хранится лучший результат игры

// 5 - Все текстуры игрока
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

//------------------------------------------( Глобальные переменные OpenGL )---

LRESULT CALLBACK WndProc(HWND    hWnd,   UINT    message,
                         WPARAM  wParam, LPARAM  lParam);
static HGLRC hRC;               // Постоянный контекст рендеринга
static HDC hDC;                 // Приватный контекст устройства GDI

GLuint texture_human[number_human_texture];
GLuint tex_main[68];
GLuint tex_enemy[50];

//------------------------------------------------------------( Мои классы )---
#include "GameManager.h"
GameManager manager;

//----------------------------------------------------------------( InitGL )---
// Вызвать после создания окна GL
//-----------------------------------------------------------------------------
GLvoid InitGL(GLsizei Width, GLsizei Height)    
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Очистка экрана в черный цвет
	glClearDepth(1.0);                      // Разрешить очистку буфера глубины
    glDepthFunc(GL_LESS);                   // Тип теста глубины
    glEnable(GL_DEPTH_TEST);                // разрешить тест глубины
	glShadeModel(GL_SMOOTH);        // разрешить плавное цветовое сглаживание
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Сглаживание линий
    glMatrixMode(GL_PROJECTION);    // Выбор матрицы проекции
    glLoadIdentity();               // Сброс матрицы проекции
	// Вычислить соотношение геометрических размеров для окна
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);    
    glMatrixMode(GL_MODELVIEW);     // Выбор матрицы просмотра модели

	manager.GLobus._BuildFont();
}

//---------------------------------------------------------( ReSizeGLScene )---
// Вызвать при изменении размеров окна
//-----------------------------------------------------------------------------
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height==0) Height=1; // Предотвращение деления на 0, если окно мало
    // Сброс текущей области вывода и перспективных преобразований       
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций
    glLoadIdentity();                       // Сброс матрицы проекции

	glOrtho(0.0f,Width,Height,0.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);     // Выбор матрицы просмотра модели
	glLoadIdentity();
}

//-----------------------------------------------------------( DrawGLScene )---
// В последствии будет заменена на другую функцию
//-----------------------------------------------------------------------------
GLvoid GameLand(GLvoid)
{
	switch(status) // Определяются фазы игры
	{
	case BEGIN:
		manager.game_begin();
		if(!((clock()/CLOCKS_PER_SEC)<begin_time)) status=MENU; // Показ заставки
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
		// Мысль с остановкой времени классная, но надо её реализовать 
		// и проверить на более быстрых машинах
		static DWORD begin;
		timeBeginPeriod(1);
		begin=timeGetTime();
		manager._core();
		for(;timeGetTime()-begin<15;); // Задержка должна тормозить игру на более быстрых машинах
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
// Инициализирует регистрирует и отображает главное окно
///////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine, int       nCmdShow)
{
 	MSG             msg;    // Структура сообщения Windows
    WNDCLASS        wc;     // Структура класса Windows для установки типа окна
    HWND            hWnd;   // Сохранение дискриптора окна

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

						0, 0,                   // Позиция окна на экране
						800, 600,               // Ширина и высота окна

						NULL,
						NULL,
						hInstance,
						NULL);
	if(!hWnd)
    {
        MessageBox(0,"Window Creation Error.","Error",MB_OK|MB_ICONERROR); 
        return FALSE;
    }

	// Переключение в полный экран
	DEVMODE dmScreenSettings;                       // Режим работы

	memset(&dmScreenSettings, 0, sizeof(DEVMODE));  // Очистка для установок
	dmScreenSettings.dmSize = sizeof(DEVMODE);      // Размер структуры Devmode
	dmScreenSettings.dmPelsWidth    = 800;          // Ширина экрана
	dmScreenSettings.dmPelsHeight   = 600;          // Высота экрана
	dmScreenSettings.dmFields       = DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        
	ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

	for(;;)
    {
		// Обработка всех сообщений
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
			if (GetMessage(&msg, NULL, 0, 0))
            {
				TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else return TRUE;
        }
		SwapBuffers(hDC);     // Переключить буфер экрана
		GameLand();           // Точка входа в мою игру
		if (keys[VK_ESCAPE]) SendMessage(hWnd,WM_CLOSE,0,0);  // Если ESC - выйти
    }
	return 0;
}

//---------------------------------------------------------------( WndProc )---
// Обработчик моих сообщений
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND    hWnd,   UINT    message,
						 WPARAM  wParam, LPARAM  lParam)
{
	RECT    Screen;         // используется позднее для размеров окна
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Размер этой структуры
        1,                              // Номер версии (?)
        PFD_DRAW_TO_WINDOW |            // Формат для Окна
        PFD_SUPPORT_OPENGL |            // Формат для OpenGL
        PFD_DOUBLEBUFFER,               // Формат для двойного буфера
        PFD_TYPE_RGBA,                  // Требуется RGBA формат
        16,                             // Выбор 16 бит глубины цвета
        0, 0, 0, 0, 0, 0,               // Игнорирование цветовых битов (?)
        0,                              // нет буфера прозрачности
        0,                              // Сдвиговый бит игнорируется (?)
        0,                              // Нет буфера аккумуляции
        0, 0, 0, 0,                     // Биты аккумуляции игнорируются (?)
        16,                             // 16 битный Z-буфер (буфер глубины) 
        0,                              // Нет буфера траффарета
        0,                              // Нет вспомогательных буферов (?)
        PFD_MAIN_PLANE,                 // Главный слой рисования
        0,                              // Резерв (?)
        0, 0, 0                         // Маски слоя игнорируются (?)
    };

	switch (message)
    {
		case WM_CREATE:
            hDC = GetDC(hWnd);      // Получить контекст устройства для окна
			// Найти ближайшее совпадение для нашего формата пикселов
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









                        





