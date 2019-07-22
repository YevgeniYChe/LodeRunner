//===============================================================( UsingGL )===
// Использует все функции библиотеки OpenGL
//=============================================================================
#include <string>

class UsingGL
{
private:
	_STATION temp;                                // Временное состояние игры
	_STATIC obj;								  // Вр. состояние тек. стат. об.
	GLuint texture_menu[number_menu_texture];     // Текстуры для меню
	GLuint texture_field[number_field_texture];   // Текстуры для сетки
	GLuint texture_static[number_static_texture]; // Текстуры статических обьектов
	GLuint base;                                  // Массив для отображения шрифта 
	GLuint Beg;
	GLuint Dev;
	
	GLvoid _square(int _x1, int _y1, int _x2, int y2, GLuint Tex);
	GLvoid _KillFont();

public:
	UsingGL();
	~UsingGL();
	GLvoid   _BuildFont();
	GLuint _LoadOneTexture(char *name); // Загрузит единственную текстуру

	GLvoid   glPrint(int x, int y, const char *fmt, ...); // Написать текст
	void     picture();									  // Покажет заставку
	void     develop();
	_STATION menu(int _x, int _y);		// Вернёт меню в которое попадает мышь
	void     show_field(int _field[][24]);  // Показать поле
	_STATIC  show_down(int _x, int _y, bool _LM_DOWN);
	
};

//----------------------------------------------------------( Constructor )---
UsingGL::UsingGL()
{
	obj=SPACE;
}

//------------------------------------------------------------( Destructor )---
UsingGL::~UsingGL()
{
	_KillFont();
}

//-------------------------------------------------------( _LoadOneTexture )---
// Загрузить единственную текстуру
//-----------------------------------------------------------------------------
GLuint UsingGL::_LoadOneTexture(char *name)
{
	GLuint temp=NULL;
	AUX_RGBImageRec *texture=NULL;
	texture = auxDIBImageLoad(name);
	glGenTextures(1, &temp);     

	glBindTexture(GL_TEXTURE_2D, temp);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->sizeX, texture->sizeY,
	GL_RGB, GL_UNSIGNED_BYTE, texture->data);

	return temp;
}

//---------------------------------------------------------------( _square )---
// Отобразить прямоугольник с наложенной на него текстурой
//-----------------------------------------------------------------------------
GLvoid UsingGL::_square(int _x1, int _y1, int _x2, int _y2, GLuint Tex)
{
	glEnable(GL_TEXTURE_2D);      // Разрешение наложение текстуры
	glTranslated(0,0,0);
	glBindTexture(GL_TEXTURE_2D, Tex);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(_x1, _y2, 0);  // Низ лево
		glTexCoord2f(1.0f, 0.0f); glVertex3d(_x2, _y2, 0);	// Низ право
		glTexCoord2f(1.0f, 1.0f); glVertex3d(_x2, _y1, 0);	// Верх право
		glTexCoord2f(0.0f, 1.0f); glVertex3d(_x1, _y1, 0);  // Верх лево
	glEnd();
	glDisable(GL_TEXTURE_2D); 
}

//------------------------------------------------------------( _BuildFont )---
// Построение нашего растрового шрифта
//-----------------------------------------------------------------------------
GLvoid UsingGL::_BuildFont()  
{

    HFONT  font;             // Идентификатор фонта
    base = glGenLists(256);  // Выделим место для 96 символов ( НОВОЕ )
	font = CreateFont( -20,                          // Высота фонта ( НОВОЕ )
					     0,                          // Ширина фонта
		                 0,                          // Угол отношения
                         0,                          // Угол наклона
						 FW_BOLD,                    // Ширина шрифта
						 FALSE,                      // Курсив
						 FALSE,                      // Подчеркивание
						 FALSE,                      // Перечеркивание
					     ANSI_CHARSET,              // Идентификатор символов
						 OUT_TT_PRECIS,              // Точность вывода
						 CLIP_DEFAULT_PRECIS,        // Точность отсечения
						 ANTIALIASED_QUALITY,        // Качество вывода
						 FF_DONTCARE|DEFAULT_PITCH,  // Семейство и шаг
						 "Courier New");             // Имя шрифта	
	SelectObject(hDC, font);             // Выбрать шрифт, созданный нами
	wglUseFontBitmaps(hDC, 0, 256, base); // Построить 256 символов
}

//-------------------------------------------------------------( _KillFont )---
// Удаление шрифта
//-----------------------------------------------------------------------------
GLvoid UsingGL::_KillFont()            
{
   glDeleteLists(base, 256);   // Удаление всех 96 списков отображения ( НОВОЕ )
}

//---------------------------------------------------------------( glPrint )---
// Заказная функция «Печати» GL в нужных координатах
//-----------------------------------------------------------------------------
GLvoid UsingGL::glPrint(int x, int y, const char *fmt, ...)        
{
	glRasterPos2d(x,y);

	char    text[256];      // Место для нашей строки
    va_list    ap;          // Указатель на список аргументов
	if (fmt == NULL)  return;

	va_start(ap, fmt);               // Разбор строки переменных
        vsprintf(text, fmt, ap);     // И конвертирование символов в реальные коды
    va_end(ap);                      // Результат помещается в строку
	glPushAttrib(GL_LIST_BIT);       // Протолкнуть биты списка отображения
	glListBase(base);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib(); // Возврат битов списка отображения ( НОВОЕ )
}

//---------------------------------------------------------------( picture )---
// Отображает заставку для начала игры
//-----------------------------------------------------------------------------
void UsingGL::picture()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	static int as=0;
	if(as==0) Beg=_LoadOneTexture("Screen/begin.bmp");
	_square(0,0,800,600,Beg);

	as++;
	if(as>2)
	{
	if(!Dev) Dev=_LoadOneTexture("Screen/developers.bmp");
	if(!texture_menu[0])
	    for(int i=0; i<number_menu_texture; 
		    texture_menu[i++]=_LoadOneTexture(name_menu_texture[i]));

	if(!texture_field[0])  
	for(int i=0; i<number_field_texture; 
		texture_field[i++]=_LoadOneTexture(name_field_texture[i]));

	if(!texture_static[0]) 
	for(int i=0; i<number_static_texture; 
		texture_static[i++]=_LoadOneTexture(name_static_texture[i]));

	if(!texture_static[0])  
	for(int i=0; i<number_static_texture; 
		texture_static[i++]=_LoadOneTexture(name_static_texture[i]));
	}
	
}

//----------------------------------------------------------------( develop )---
// Покажет разработчиков
//------------------------------------------------------------------------------
void UsingGL::develop()
{
	_square(0,0,800,600,Dev);
}

//-------------------------------------------------------------------( menu )---
// Отображает меню
//------------------------------------------------------------------------------
_STATION UsingGL::menu(int _x, int _y)
{
	temp=MENU;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glLoadIdentity();
	glTranslated(0,0,0);

	// Собственно рисование самого меню
	_square(0,0,800,600,texture_menu[0]);

	// Рисование кнопок и их обработка
	if((_x>50&&_x<310)&&(_y>300&&_y<364))           // Старт 
	{
		  _square(50,300,310,364,texture_menu[2]);
		  temp=GAME;
	}
	else  _square(50,300,310,364,texture_menu[1]);

	if((_x>50&&_x<310)&&(_y>369&&_y<433))           // Редактор
	{
		  _square(50,369,310,433,texture_menu[4]);  
		  temp=REDACTION;
	}
	else  _square(50,369,310,433,texture_menu[3]);

	if((_x>50&&_x<310)&&(_y>438&&_y<501))           // Результат
	{
		  _square(50,438,310,501,texture_menu[6]);
		  temp=RESULT;
	}
	else  _square(50,438,310,501,texture_menu[5]);

	if((_x>50&&_x<310)&&(_y>506&&_y<570))           // Выход
	{
		  _square(50,506,310,570,texture_menu[8]);
		  temp=EXIT;
	}
	else  _square(50,506,310,570,texture_menu[7]);

	return temp;	
}

//------------------------------------------------------------( show_field )---
// Показать поле. В дальнейшем нужно её перегрузить
//-----------------------------------------------------------------------------
void UsingGL::show_field(int _field[][24])
{
	glDisable(GL_DEPTH_TEST);
	
	// Сначала рисуем сетку вокруг поля                    НАСТРОИТЬ!!!
	static int x,y,i,j;
	for(i=0, x=41 ;i<24;i++,x+=30) _square(x,60,x+30,74,texture_field[0]);
	for(i=0, x=41 ;i<24;i++,x+=30) _square(x,526,x+30,540,texture_field[0]);
	for(i=0, y=76 ;i<15;i++,y+=30) _square(25,y,39,y+30,texture_field[1]);
	for(i=0, y=76 ;i<15;i++,y+=30) _square(761,y,775,y+30,texture_field[1]);
	_square( 25,60,  40, 75,texture_field[2]);
	_square(760,60, 775, 75,texture_field[3]);
	_square( 25,525, 40,540,texture_field[4]);
	_square(760,525,775,540,texture_field[5]);
	// потом само поле по заданной сетке
	// нужно анализировать сетку и строить карту
	for(i=0;i<15;i++)
		for(j=0;j<24;j++)
			_square(40+j*30,75+i*30,40+((j+1)*30),75+((i+1)*30),texture_static[_field[i][j]]);
}


//-------------------------------------------------------------( show_down )---
// Показать нижнюю часть редактора с обьектами
//-----------------------------------------------------------------------------
_STATIC UsingGL::show_down(int _x, int _y, bool _LM_DOWN)
{
	glDisable(GL_DEPTH_TEST);
	
	// Отрисовываем поля внизу
	for(int i=0; i<number_static_texture-1; i++) 
	    _square(225+i*40,555,255+i*40,585,texture_static[i]);

	// Проверка попадания в обьект
	if(_LM_DOWN) 
		for(i=0; i<number_static_texture-1; i++)
			if((_x>225+i*40)&&(_x<255+i*40)&&(_y>555&&_y<585))
				return obj=(_STATIC)i;
	return obj;
}
