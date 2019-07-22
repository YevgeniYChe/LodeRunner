//===============================================================( UsingGL )===
// ���������� ��� ������� ���������� OpenGL
//=============================================================================
#include <string>

class UsingGL
{
private:
	_STATION temp;                                // ��������� ��������� ����
	_STATIC obj;								  // ��. ��������� ���. ����. ��.
	GLuint texture_menu[number_menu_texture];     // �������� ��� ����
	GLuint texture_field[number_field_texture];   // �������� ��� �����
	GLuint texture_static[number_static_texture]; // �������� ����������� ��������
	GLuint base;                                  // ������ ��� ����������� ������ 
	GLuint Beg;
	GLuint Dev;
	
	GLvoid _square(int _x1, int _y1, int _x2, int y2, GLuint Tex);
	GLvoid _KillFont();

public:
	UsingGL();
	~UsingGL();
	GLvoid   _BuildFont();
	GLuint _LoadOneTexture(char *name); // �������� ������������ ��������

	GLvoid   glPrint(int x, int y, const char *fmt, ...); // �������� �����
	void     picture();									  // ������� ��������
	void     develop();
	_STATION menu(int _x, int _y);		// ����� ���� � ������� �������� ����
	void     show_field(int _field[][24]);  // �������� ����
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
// ��������� ������������ ��������
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
// ���������� ������������� � ���������� �� ���� ���������
//-----------------------------------------------------------------------------
GLvoid UsingGL::_square(int _x1, int _y1, int _x2, int _y2, GLuint Tex)
{
	glEnable(GL_TEXTURE_2D);      // ���������� ��������� ��������
	glTranslated(0,0,0);
	glBindTexture(GL_TEXTURE_2D, Tex);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(_x1, _y2, 0);  // ��� ����
		glTexCoord2f(1.0f, 0.0f); glVertex3d(_x2, _y2, 0);	// ��� �����
		glTexCoord2f(1.0f, 1.0f); glVertex3d(_x2, _y1, 0);	// ���� �����
		glTexCoord2f(0.0f, 1.0f); glVertex3d(_x1, _y1, 0);  // ���� ����
	glEnd();
	glDisable(GL_TEXTURE_2D); 
}

//------------------------------------------------------------( _BuildFont )---
// ���������� ������ ���������� ������
//-----------------------------------------------------------------------------
GLvoid UsingGL::_BuildFont()  
{

    HFONT  font;             // ������������� �����
    base = glGenLists(256);  // ������� ����� ��� 96 �������� ( ����� )
	font = CreateFont( -20,                          // ������ ����� ( ����� )
					     0,                          // ������ �����
		                 0,                          // ���� ���������
                         0,                          // ���� �������
						 FW_BOLD,                    // ������ ������
						 FALSE,                      // ������
						 FALSE,                      // �������������
						 FALSE,                      // ��������������
					     ANSI_CHARSET,              // ������������� ��������
						 OUT_TT_PRECIS,              // �������� ������
						 CLIP_DEFAULT_PRECIS,        // �������� ���������
						 ANTIALIASED_QUALITY,        // �������� ������
						 FF_DONTCARE|DEFAULT_PITCH,  // ��������� � ���
						 "Courier New");             // ��� ������	
	SelectObject(hDC, font);             // ������� �����, ��������� ����
	wglUseFontBitmaps(hDC, 0, 256, base); // ��������� 256 ��������
}

//-------------------------------------------------------------( _KillFont )---
// �������� ������
//-----------------------------------------------------------------------------
GLvoid UsingGL::_KillFont()            
{
   glDeleteLists(base, 256);   // �������� ���� 96 ������� ����������� ( ����� )
}

//---------------------------------------------------------------( glPrint )---
// �������� ������� ������� GL � ������ �����������
//-----------------------------------------------------------------------------
GLvoid UsingGL::glPrint(int x, int y, const char *fmt, ...)        
{
	glRasterPos2d(x,y);

	char    text[256];      // ����� ��� ����� ������
    va_list    ap;          // ��������� �� ������ ����������
	if (fmt == NULL)  return;

	va_start(ap, fmt);               // ������ ������ ����������
        vsprintf(text, fmt, ap);     // � ��������������� �������� � �������� ����
    va_end(ap);                      // ��������� ���������� � ������
	glPushAttrib(GL_LIST_BIT);       // ����������� ���� ������ �����������
	glListBase(base);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib(); // ������� ����� ������ ����������� ( ����� )
}

//---------------------------------------------------------------( picture )---
// ���������� �������� ��� ������ ����
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
// ������� �������������
//------------------------------------------------------------------------------
void UsingGL::develop()
{
	_square(0,0,800,600,Dev);
}

//-------------------------------------------------------------------( menu )---
// ���������� ����
//------------------------------------------------------------------------------
_STATION UsingGL::menu(int _x, int _y)
{
	temp=MENU;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glLoadIdentity();
	glTranslated(0,0,0);

	// ���������� ��������� ������ ����
	_square(0,0,800,600,texture_menu[0]);

	// ��������� ������ � �� ���������
	if((_x>50&&_x<310)&&(_y>300&&_y<364))           // ����� 
	{
		  _square(50,300,310,364,texture_menu[2]);
		  temp=GAME;
	}
	else  _square(50,300,310,364,texture_menu[1]);

	if((_x>50&&_x<310)&&(_y>369&&_y<433))           // ��������
	{
		  _square(50,369,310,433,texture_menu[4]);  
		  temp=REDACTION;
	}
	else  _square(50,369,310,433,texture_menu[3]);

	if((_x>50&&_x<310)&&(_y>438&&_y<501))           // ���������
	{
		  _square(50,438,310,501,texture_menu[6]);
		  temp=RESULT;
	}
	else  _square(50,438,310,501,texture_menu[5]);

	if((_x>50&&_x<310)&&(_y>506&&_y<570))           // �����
	{
		  _square(50,506,310,570,texture_menu[8]);
		  temp=EXIT;
	}
	else  _square(50,506,310,570,texture_menu[7]);

	return temp;	
}

//------------------------------------------------------------( show_field )---
// �������� ����. � ���������� ����� � �����������
//-----------------------------------------------------------------------------
void UsingGL::show_field(int _field[][24])
{
	glDisable(GL_DEPTH_TEST);
	
	// ������� ������ ����� ������ ����                    ���������!!!
	static int x,y,i,j;
	for(i=0, x=41 ;i<24;i++,x+=30) _square(x,60,x+30,74,texture_field[0]);
	for(i=0, x=41 ;i<24;i++,x+=30) _square(x,526,x+30,540,texture_field[0]);
	for(i=0, y=76 ;i<15;i++,y+=30) _square(25,y,39,y+30,texture_field[1]);
	for(i=0, y=76 ;i<15;i++,y+=30) _square(761,y,775,y+30,texture_field[1]);
	_square( 25,60,  40, 75,texture_field[2]);
	_square(760,60, 775, 75,texture_field[3]);
	_square( 25,525, 40,540,texture_field[4]);
	_square(760,525,775,540,texture_field[5]);
	// ����� ���� ���� �� �������� �����
	// ����� ������������� ����� � ������� �����
	for(i=0;i<15;i++)
		for(j=0;j<24;j++)
			_square(40+j*30,75+i*30,40+((j+1)*30),75+((i+1)*30),texture_static[_field[i][j]]);
}


//-------------------------------------------------------------( show_down )---
// �������� ������ ����� ��������� � ���������
//-----------------------------------------------------------------------------
_STATIC UsingGL::show_down(int _x, int _y, bool _LM_DOWN)
{
	glDisable(GL_DEPTH_TEST);
	
	// ������������ ���� �����
	for(int i=0; i<number_static_texture-1; i++) 
	    _square(225+i*40,555,255+i*40,585,texture_static[i]);

	// �������� ��������� � ������
	if(_LM_DOWN) 
		for(i=0; i<number_static_texture-1; i++)
			if((_x>225+i*40)&&(_x<255+i*40)&&(_y>555&&_y<585))
				return obj=(_STATIC)i;
	return obj;
}
