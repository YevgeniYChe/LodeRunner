#include "UsingGL.h"
#include "Coordinator.h"
#include "Human.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

//===========================================================( GameManager )===
// ����� ��������� ���� ����� ��� ������ �������������� � ���� ���������� �����
//=============================================================================
class GameManager
{
private:
	int      field[15][24];         // ����� �������� ����
	int      double_field[15][24];  // �������� ����� ������ ����
	time_t   timers[15][24];        // �� ������ ������ ���� ������
	time_t   humanoid[15][24];      // �� ������� ���� � ������ ���� ������
	bool     Pause;             
	bool     firestarter;           // ������������ ��� �������� 

	_STATION _status;         // ��������� ���������� ������� ���������
	bool UseRedaction;
	bool GAME_OVER;           // ���� ����� ���� 
	bool CONUS;               // ���� ����������� ������
	bool HITMAN;              // ���� ����� ��������� � ���� � ����� 

	Human eugene;
	_DIRECT eug_dir;
	_DIRECT eug_last_dir;
	int cool;

	int col_real_enemy;
	Human enemy[360];
	_DIRECT enemy_dir[360];
	_DIRECT enemy_last_dir[360];

	void     _clear_field();  // ������� ����
	void     load(char *name,int _field[][24]); // �������� �����
	void     save(char *name,int _field[][24]); // ���������� �����
public:
	UsingGL GLobus;      // ����� �������� ������ � ��������� OpenGL
	Coordinator cori;    // ����� �������� �� ����� ������������ ����

	GameManager();
	void     game_begin();// 1 - ��������
	void     game_result();
	_STATION game_menu(int x, int y, bool _LM_DOWN);                       // 2 - ������� ����
	void     game_redaction(int _x, int _y, bool _LM_DOWN, bool _RM_DOWN); // 3 - �������� ����
	void     game();                                                       // 4 - ���� ����
	void     _core(); // ����
	_DIRECT  falling(Human &man, int _step, _DIRECT dir);  // �������� �� �������
	_DIRECT  block(Human &man, _DIRECT dir);               // �������� �� ���������
	_DIRECT  inblock(Human &man, _DIRECT dir);
	void     key(Human &man, _DIRECT dir, int _step);      // ��������� ������� �� �������� 
}; 

//-----------------------------------------------------------( Constructor )---
GameManager::GameManager()
{
	_clear_field();
	eugene.person=1;
	eugene.abc=6;
	eugene.cba=0;
	eugene.stepping=eugene.takt=eugene.nono=0;
	for(int i=0;i<360;i++)
	{
		enemy[i].person=0;
		enemy[i].abc=6;
		enemy[i].cba=0;
		enemy[i].stepping=enemy[i].takt=enemy[i].nono=0;
	}
	for(i=0;i<15;i++)
	   for(int j=0;j<24;j++) 
	   {
		   timers[i][j]=0;
		   humanoid[i][j]=0;
	   }
	GAME_OVER=false;
	CONUS=false;
	HITMAN=false;

	score.best_gold=0;
	score.current_gold=0;
	score.current_score=0;
	score.min_lev=1;
	score.max_lev=number_level;
	score.cur_lev=1;
	strcpy(score.name_best,"NoName");

	col_real_enemy=0;
	cool=0;
	firestarter=false;

	UseRedaction=false;
}

//---------------------------------------------------------( _clear_fieald )---
// ������� ���� ������. �.� ���� ���������� �����
//-----------------------------------------------------------------------------
void GameManager::_clear_field()
{
	for(int i=0;i<15;i++) for(int j=0;j<24;j++) field[i][j]=0;
}

//------------------------------------------------------------------( load )---
// �������� ���� �� �����
//-----------------------------------------------------------------------------
void GameManager::load(char *name,int _field[][24])
{
	ifstream in(name);
	ifstream result(res);
	for(int i=0; i<15; i++)
		for(int j=0; j<24; j++)
			in>>_field[i][j];
	result>>score.best_gold;
	result>>score.name_best;
	in.close();
	result.close();
}

//------------------------------------------------------------------( save )---
// ���������� �� ����
//-----------------------------------------------------------------------------
void GameManager::save(char *name,int _field[][24])
{
	ofstream out(name);
	ofstream result(res);
	for(int i=0; i<15; i++)
		for(int j=0; j<24; j++)
			out<<_field[i][j]<<endl;
	result<<score.best_gold<<endl;
	result<<score.name_best;
	out.close();
	result.close();
}

//------------------------------------------------------------( game_begin )---
// ���������� ��� ������ ����. �������� ��������
//----------------------------------------------------------------------------- 
void GameManager::game_begin()
{
	GLobus.picture();
}

//-----------------------------------------------------------( game_result )---
// ���������� ���������� ����
//-----------------------------------------------------------------------------
void GameManager::game_result()
{
	GLobus.develop();
	if(keys[VK_ESCAPE])             // ��������� ������
	{
		status=MENU;
		keys[VK_ESCAPE]=false;
	}
}
//-------------------------------------------------------------( game_menu )---
// ���������� � �������� ������� ����
//-----------------------------------------------------------------------------
_STATION GameManager::game_menu(int x, int y, bool _LM_DOWN)
{
	_status=GLobus.menu(x,y);  // ��������� ����� � ����������� �� ��������� ����
	if(_LM_DOWN) return _status;
	return MENU;
}

//--------------------------------------------------------( game_redaction )---
// ���������� ������ (�������� ����)
//-----------------------------------------------------------------------------
void GameManager::game_redaction(int _x, int _y, bool _LM_DOWN, bool _RM_DOWN)
{
	
	// ��������� //////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������-�� ��� �������
	// ���������� ��� �������
	glColor3f(0.3f,1.0f,1.0f);
	GLobus.glPrint(340,16,"LEVEL N%d", score.cur_lev);
	glColor3f(1.0f,1.0f,1.0f);
	GLobus.glPrint(25,40,"<- = Dec Lev");
	GLobus.glPrint(25,56,"-> = Inc Lev");
	GLobus.glPrint(300,40,"L = Load Level");
	GLobus.glPrint(300,56,"S = Save Level");
	GLobus.glPrint(570,40,"Spc = Begin game");
	GLobus.glPrint(570,56,"Esc = Exit editor");

	// ����������� ������ ������� �� ����� ��������� ����. ���� OpenGL
	stat=GLobus.show_down(_x, _y, _LM_DOWN); // ���������

	// ��������� ������ ///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// ��������� ������� ������ � �������� ����
	if(cori.pole(_x,_y)) 
	{ 
		cori.pix_to_game(_x,_y);
		if(_LM_DOWN) field[_y][_x]=stat;
		if(_RM_DOWN) field[_y][_x]=SPACE;
	}

	if(keys[VK_ESCAPE])             // ��������� ������
	{
		status=MENU;
		UseRedaction=false;
		keys[VK_ESCAPE]=false;
	}
	
	if(keys['L']) // ��������
	{
		char fname[255];
		char lc[5];
		strcpy(fname,levels);
		itoa(score.cur_lev, lc ,10);
		strcat(fname,lc);
		strcat(fname,".lev");
		load(fname, field);
	}

	if(keys['S']) // ����������
	{
		char fname[255];
		char lc[5];
		strcpy(fname,levels);
		itoa(score.cur_lev, lc ,10);
		strcat(fname,lc);
		strcat(fname,".lev");
		save(fname, field);
	}

	if(keys[VK_LEFT])
		if(score.cur_lev>score.min_lev)
		{	
			score.cur_lev--;
			Sleep(100);
		}

	if(keys[VK_RIGHT])
		if(score.cur_lev<score.max_lev)
		{
			score.cur_lev++;
			Sleep(100);
		}

	if(keys[VK_SPACE])
	{
		status=GAME;
		score.current_score=0;
		UseRedaction=true;
	}
	// ���������� ����������� ����
	// + ���� ����� ������ ������ ����������� �� ���� ���� ����� ������ ���� � �.�.
	GLobus.show_field(field);
}

//------------------------------------------------------------------( game )---
// ��������� ��� ������� ���� (������� �� ������� � �.�)
//-----------------------------------------------------------------------------
void GameManager::game()
{
	//����������� ������ ����������� ���� (TRUE-��������, FALSE-�� ���. ����) 
	if(UseRedaction) 
	{
		if(HITMAN)
		{
			HITMAN=false;
			score.current_score-=1000;
		}
		if(GAME_OVER||CONUS)
		{
			UseRedaction=false;
			GAME_OVER=false;
			CONUS=false;
			if(score.current_score>score.best_gold) 
				score.best_gold=score.current_score;
			status=REDACTION;
		}
		else
		{
			status=_CORE;
		}
		// ��������� �� ������ �� ������� ���������� �����
	}
	else
	{
		if(CONUS)
		{
			score.cur_lev++;
			CONUS=false;
		}
		if(HITMAN)
		{
			HITMAN=false;
			score.current_score-=1000;
		}
		// ���������������� � ��������� � ����
		char fname[255];
		char lc[5];
		strcpy(fname,levels);
		itoa(score.cur_lev, lc ,10);
		strcat(fname,lc);
		strcat(fname,".lev");
		if(score.cur_lev==score.max_lev+1-15) GAME_OVER=true;
		else load(fname,field);
		if(score.current_score>score.best_gold)
		{
				score.best_gold=score.current_score;
				save(fname,field);
		}
		
		// ������ ���� ������� �� ������������� ����� ����

		if(GAME_OVER)
		{
			GAME_OVER=false;
			status=MENU;                 // ���� ������ �� ��� �����
		}
		else
		{
			status=_CORE;
		}
		// ��������� ������� ��������� ���� � �.�.
	}

	// 1) ������ �����
	// a) ���������� ���������� ����������� �� �����
	// �) ������� �� ��������� ���������
	// �) �������� 2 ������� ����
	col_real_enemy=0;
	score.current_gold=0;
	firestarter=FALSE;
	for(int i=0;i<15;i++)
		for(int j=0;j<24;j++)
		{
			double_field[i][j]=field[i][j];
			if((field[i][j])==7) // ���� ����� ����
			{
				eugene.coord(i,j);  
				double_field[i][j]=0;
			}
			if((field[i][j])==8) // ���� ����� ����������
			{
				enemy[col_real_enemy].coord(i,j);
				col_real_enemy++;
				double_field[i][j]=0;
			}
			if((field[i][j])==5)double_field[i][j]=0; // ��������� ��������
			if((field[i][j])==6)score.current_gold++;
		}
	Pause=TRUE;
	// 4) ������ ����
}

//-----------------------------------------------------------------( _core )---
// ������� ����
//-----------------------------------------------------------------------------
void GameManager::_core()
{	
	// ������� ��� ������ ��� ������ ���� ��� � �����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������-�� ��� �������
	GLobus.glPrint(40,37,"LEVEL N%d", score.cur_lev);
	GLobus.glPrint(300,37,"BEST %d %s",score.best_gold,score.name_best);
	GLobus.glPrint(640,37,"SCORE %d",score.current_score);
	// ��� ������ �����
	// 1) ����� ��������                 (field)
	// 2) ����� �������� ������          (double_field)
	// 3) ���������� � ����� �������     (col_real_enemy)
	// 4) ��� �������������� ������ �.�. (score)
	// 5) ���-�� ������                  (score.current_gold)
	

	if(Pause)
	{
		GLobus.show_field(double_field);
		// �������� ����������
		eugene.run(STOP,0);
		for(int i=0;i<col_real_enemy;i++)
			enemy[i].run(STOP,0);
		// �������� ����������
		eug_dir=eug_last_dir=NODOWN;
		for(i=0;i<col_real_enemy;i++) enemy_dir[i]=STOP;
		// �������� �������
		GLobus.glPrint(150,578,"Please press the game button for CONTINIUS!!!");
		if(keys[VK_RIGHT]||keys[VK_UP]||keys[VK_DOWN]||keys[VK_LEFT]||
		   keys['X']||keys['C']) Pause=FALSE;
		if(keys[VK_ESCAPE])
		{
			keys[VK_ESCAPE]=false;
			GAME_OVER=true;
			status=GAME;
		}
	}
	else
	{
		eug_dir=STOP;
		if(cool==1) cool=-1;
		cool++;
		// ���������� ������� �������� ����������� ��
		// ����� ([LEFT][RIGHT][UP][DOWN][STOP]) ��� ������� ���������
		for(int i=0;i<col_real_enemy;i++) enemy_dir[i]=STOP;
		for(i=0; i<col_real_enemy;i++)
			enemy_dir[i]=enemy[i].find_human(eugene,double_field,enemy_last_dir[i]);

		GLobus.show_field(double_field);


// ���������� ����� �������
		if(keys[VK_ESCAPE])
		{ 
			keys[VK_ESCAPE]=false;
			GAME_OVER=true;
			status=GAME;
		}
		if(keys['P'])      Pause=true;

		if(keys[VK_LEFT])  eug_dir=LEFT;
		if(keys[VK_RIGHT]) eug_dir=RIGHT;
		if(keys[VK_UP])    eug_dir=UP;
		if(keys[VK_DOWN])  eug_dir=DOWN;
		if(keys['X'])      eug_dir=FIRE_LEFT;
		if(keys['C'])	   eug_dir=FIRE_RIGHT;
		

// ��������� ������������ ������� �������
// ����������� 
		static int tx=0,ty=0;
		for(i=0;i<col_real_enemy;i++)
		{
			tx=enemy[i].x;
			ty=enemy[i].y;
		    cori.pix_to_game(tx,ty);
			if(double_field[ty][tx]==9) double_field[ty][tx]=25;  
		}

		for( i=0;i<col_real_enemy;i++) 
		if( (inblock(enemy[i],enemy_dir[i])!=NODOWN)&&
			(falling(enemy[i],enemy_step[cool],enemy_dir[i])!=NODOWN)&&
			(block(enemy[i],enemy_dir[i])!=NODOWN))
				key(enemy[i],enemy_dir[i],enemy_step[cool]);
		else enemy_dir[i]=NODOWN;

		for(i=0;i<col_real_enemy;i++)
		{
			tx=enemy[i].x;
			ty=enemy[i].y;
		    cori.pix_to_game(tx,ty);
			if(double_field[ty][tx]==25) double_field[ty][tx]=9;  
		}
		// ���������� ����������
		
		for(i=0;i<col_real_enemy;i++)
		{
			tx=enemy[i].x;
			ty=enemy[i].y;
		    cori.pix_to_game(tx,ty);
		    if(double_field[ty][tx]==0) double_field[ty][tx]=8;
			if(double_field[ty][tx]==9) double_field[ty][tx]=10;  //10- ���� � �������
			if(double_field[ty][tx]==3) double_field[ty][tx]=11;  //11- ���� � �����������
			if(double_field[ty][tx]==4) double_field[ty][tx]=12;  //12- ���� � ��������

		}

		// ��������� �� �������� �� ��� ���������
		if(firestarter==true) eug_dir=eug_last_dir;
			
		if((falling(eugene,main_step[cool],eug_dir)!=NODOWN)&&  
		   (block(eugene,eug_dir)!=NODOWN))
				 key(eugene,eug_dir,main_step[cool]);
		else eug_dir=NODOWN;
		
		// ������������� ����������� ���������� ������������
		eug_last_dir=eug_dir;
		for(i=0;i<col_real_enemy;i++) enemy_last_dir[i]=enemy_dir[i];

/*������������ ��� ��������� ���� �� ������� ������*/   

		int ex=eugene.x,
			ey=eugene.y; // ���������� ����

	// 1) ������� ���������� ���� ���������� ������
		cori.pix_to_game(ex,ey);
		if(double_field[ey][ex]==6)
		{
			cori.game_to_pix(ex,ey);
			if(eugene.x-5<ex && eugene.x+5>ex) 
			{
				score.current_gold--;
				score.current_score+=100;
				cori.pix_to_game(ex,ey);
				double_field[ey][ex]=0;
			}
		}
		if(score.current_gold==0)
			for(i=0;i<15;i++)
				for(int j=0;j<24;j++)
				{
					if(field[i][j]==5) double_field[i][j]=4;
					score.current_gold=-1;
				}
		ex=ey=0;
		cori.game_to_pix(ex,ey);
		if(eugene.y<ey-10)
		{
			CONUS=true;
			status=GAME;
			Sleep(800);
		}

	// (2) ����� �������� ������. �������� ������� ������� �� ���� ���������
		ex=eugene.x;
		ey=eugene.y;
		cori.pix_to_game(ex,ey);
		if(double_field[ey][ex]==8||
		   double_field[ey][ex]==10||
		   double_field[ey][ex]==11||
		   double_field[ey][ex]==12)
		{
			HITMAN=true;
			status=GAME;
			Sleep(500);
		}

	// (3) � ���������� ����������� ������ ��������� � ����������� ������� ��� ?
		
		for(i=0;i<15;i++)
			for(int j=0;j<24;j++)
			    if(double_field[i][j]==9||double_field[i][j]==10) //������� ��� ��� ����
				{
					// ���� ������ ��� �� ������� ��������� ���
					// ��������� ������ �� brick_time ������
					if(timers[i][j]==0) timers[i][j]=clock();
					if((clock()-timers[i][j]>6.25*CLOCKS_PER_SEC)&&
					   (clock()-timers[i][j]<6.5*CLOCKS_PER_SEC)) 
					   eugene.brick_up(j,i,2);
					if((clock()-timers[i][j]>6.5*CLOCKS_PER_SEC)&&
					   (clock()-timers[i][j]<6.75*CLOCKS_PER_SEC)) 
					   eugene.brick_up(j,i,1);
					if((clock()-timers[i][j]>6.75*CLOCKS_PER_SEC)&&
					   (clock()-timers[i][j]<7*CLOCKS_PER_SEC)) 
					   eugene.brick_up(j,i,0);
					if(clock()-timers[i][j]>7*CLOCKS_PER_SEC)
					{
						// �������� �� ������� � ���� ������ �����
						// ������ �������� �� ������� ����
						ex=eugene.x;
						ey=eugene.y;
						cori.pix_to_game(ex,ey);
						if(j==ex && i==ey) // �.� ��� �������� �
						{
							HITMAN=true;
							status=GAME;
							Sleep(500);
						}

						// ������ �������� �� ������� � ����� ������
						if(double_field[i][j]==10) // �.�. ��� ��������� ����
						{
							// ������� ����� � ������ ��� ����������
							for(int k=0;k<col_real_enemy;k++)
							{
								ex=enemy[k].x;
								ey=enemy[k].y;
								cori.pix_to_game(ex,ey);
								if(j==ex && i==ey)
								{
									// ����������� ���������� ���������
									srand(clock());
									enemy[k].x=rand()%24;
									enemy[k].y=0;
									cori.game_to_pix(enemy[k].x,enemy[k].y);
								}
							}
						}
						double_field[i][j]=1;
						timers[i][j]=0;
					}
					// �������� ������ ������� ����� ��������� �������
					if((double_field[i][j]==10)&&(humanoid[i][j]==0)) humanoid[i][j]=clock();
					// ������� ���� �������� � ������� 3 �������
					if(clock()-humanoid[i][j]>3.5*CLOCKS_PER_SEC)
					{
						for(int k=0;k<col_real_enemy;k++)
						{
							ex=enemy[k].x;
							ey=enemy[k].y;
							cori.pix_to_game(ex,ey);
							if(j==ex && i==ey)
							{
								if(eugene.x>=enemy[k].x)
								{
									
									if((double_field[i-1][j+1]==SPACE)||
									   (double_field[i-1][j+1]==LODE)||
									   (double_field[i-1][j+1]==BEAM)||
									   (double_field[i-1][j+1]==LADDER))
									     enemy[k].coord(i-1,j+1);
									else enemy[k].coord(i-1,j-1);
								}
								if(eugene.x<enemy[k].x)
								{
									if((double_field[i-1][j-1]==SPACE)||
									   (double_field[i-1][j-1]==LODE)||
									   (double_field[i-1][j-1]==BEAM)||
									   (double_field[i-1][j-1]==LADDER))
									     enemy[k].coord(i-1,j-1);
									else enemy[k].coord(i-1,j+1);
								}
								break;
							}
							humanoid[i][j]=0;
						}
					}
				}

		// ������ ���������� � �����
		for(i=0;i<15;i++)
			for(int j=0;j<24;j++)
			{
				if(double_field[i][j]==8 ) double_field[i][j]=SPACE;
				if(double_field[i][j]==10) double_field[i][j]=9;
				if(double_field[i][j]==11) double_field[i][j]=3;
				if(double_field[i][j]==12) double_field[i][j]=4;
			}

		// ������ FPS
		static time_t FPS=clock();
		static int CADR=0;
		CADR++;
		if(clock()-FPS>1*CLOCKS_PER_SEC)
		{
			GLobus.glPrint(0,15,"%d",CADR);
			FPS=clock();
			CADR=0;
		}
	}
}

//---------------------------------------------------------------( falling )---
// �������� �� ������� 
//-----------------------------------------------------------------------------
_DIRECT GameManager::falling(Human &man, int _step, _DIRECT dir)
{ 
	int tx=0;
	int ty=0;
	for(int i=_step;i>0;i--)
	    if(cori.there(man.x, man.y, i, DOWN, double_field,SPACE)||
		   cori.there(man.x, man.y, i, DOWN, double_field,LODE) ||
		   cori.there(man.x, man.y, i, DOWN, double_field,BEAM) ||
		   cori.there(man.x, man.y, i, DOWN, double_field,POLE) ||
		   cori.there(man.x, man.y, 0, STOP, double_field,(_STATIC)25))
		{
			// ���������� �� �������� �� ����������� � ��������
			if((cori.there(man.x, man.y, 0, UP, double_field, BEAM)&&
			    cori.there(man.x, man.y, 0, DOWN, double_field, BEAM))||
				cori.there(man.x, man.y, 0, STOP, double_field, LADDER))
			{
					return dir;
					break;
			}
			// ��������� �� X
		
			tx=man.x;
			ty=man.y;
			cori.pix_to_game(tx,ty);
			cori.game_to_pix(tx,ty);
			if(man.x<tx) man.x+=_step;
			if(man.x>tx) man.x-=_step;
			man.fall(i);
			return dir=NODOWN;
		}

	return dir;
}

//-----------------------------------------------------------------( block )---
// �������� �� ���������
//-----------------------------------------------------------------------------
_DIRECT GameManager::block(Human &man, _DIRECT dir)
{
	if((cori.there(man.x, man.y, 1, LEFT, double_field,BRICK)    ||
	    cori.there(man.x, man.y, 1, LEFT, double_field,IMBRICK)) &&
	   (cori.there(man.x, man.y, 1, RIGHT, double_field,BRICK)   ||
	    cori.there(man.x, man.y, 1, RIGHT, double_field,IMBRICK))&&
	   (cori.there(man.x, man.y, 1, DOWN, double_field,BRICK)   ||
	    cori.there(man.x, man.y, 1, DOWN, double_field,IMBRICK)))
	{ 
		man.fall(0);
		return dir=NODOWN;
	}
	else return dir;
}

//---------------------------------------------------------------( inblock )---
// �������� �� ��������� ����� � ������ �����
//-----------------------------------------------------------------------------
_DIRECT GameManager::inblock(Human &man, _DIRECT dir)
{
	if(cori.there(man.x, man.y, 0, UP,   double_field,(_STATIC)25)&&
	   cori.there(man.x, man.y, 0, DOWN, double_field,(_STATIC)25))
	{ 
		man.fall(0);
		return dir=NODOWN;
	}
	else return dir;
}

//-------------------------------------------------------------------( key )---
// ��������� �������� ��������
//-----------------------------------------------------------------------------
void GameManager::key(Human &man, _DIRECT dir, int _step)
{
	int i=0;
	int tx=0;
	int ty=0;
	switch(dir)
	{
	case RIGHT:
	case LEFT:
		tx=ty=0;
		for(i=_step;i>0;i--)
		{			
			if(cori.there(man.x, man.y, i, dir, double_field, SPACE) || // ���� ���� �������
			   cori.there(man.x, man.y, i, dir, double_field, LADDER)|| // �������
			   cori.there(man.x, man.y, i, dir, double_field, LODE)  || // ������
			   cori.there(man.x, man.y, i, dir, double_field, BEAM)  || // �����������
			   cori.there(man.x, man.y, i, dir, double_field, POLE)  ||
			   cori.there(man.x, man.y, i, dir, double_field, (_STATIC)11) ||  // ���� � �����������
			   cori.there(man.x, man.y, i, dir, double_field, (_STATIC)12) ||  // ���� � �������
 			   cori.there(man.x, man.y, i, dir, double_field, (_STATIC)8))     // ��� ����� ����
			{ 
				if(cori.there(man.x, man.y, 0, UP, double_field, BEAM))
				{
					man.climb(dir,i);
					break;
				}
				// ��������� �� Y
				tx=man.x;
				ty=man.y;
				cori.pix_to_game(tx,ty);
				cori.game_to_pix(tx,ty);
				if(man.y<ty) man.y+=_step;
				if(man.y>ty) man.y-=_step;
				man.run(dir, i);
				break;
			}
			man.run(STOP,0);
		}
		
		break;
	case UP:
		tx=ty=0;
		for(i=_step;i>0;i--) // �� ������� ����� �����
		{
			if(cori.there(man.x, man.y, 0, DOWN, double_field, LADDER)&&   // ���� ���� ��� ������ �������
			 !(cori.there(man.x, man.y, i, UP, double_field, BRICK)||      // - � ��� ��� ������� ������� ���
			   cori.there(man.x, man.y, i, UP, double_field, IMBRICK)))    // - �������������� �������
			{
				// ��������� �� X
				tx=man.x;
				ty=man.y;
				cori.pix_to_game(tx,ty);
				cori.game_to_pix(tx,ty);
				if(man.x<tx) man.x+=_step;
				if(man.x>tx) man.x-=_step;
				man.run(UP, i);
				break;
			}
			man.run(STOP,0);
		}
		break;
	case DOWN:
		tx=ty=0;
		for(i=_step;i>0;i--) // �� ������� ����� �����
		{
			if( cori.there(man.x, man.y, i, DOWN, double_field, LADDER) ||   // ���� ��� ������ ��������
				cori.there(man.x, man.y, i, DOWN, double_field, LODE) ||
			   (cori.there(man.x, man.y, 0, STOP, double_field, LADDER)&&   // ��� � �� �������
			    cori.there(man.x, man.y, i, DOWN, double_field, SPACE))||   // � ��� ������ �������
			   (cori.there(man.x, man.y, 0, DOWN, double_field, BEAM) &&    // ��� � �� ����������� � ���� ���� ����
			  !(cori.there(man.x, man.y, i, DOWN, double_field, BRICK)||    // - �������
			    cori.there(man.x, man.y, i, DOWN, double_field, IMBRICK)))) // - �������������� �������
			{
				if(cori.there(man.x, man.y, 0, DOWN, double_field, BEAM)) 
				{
					 man.climb(DOWN,i);
					 break;
				}
				// ��������� �� X
				tx=man.x;
				ty=man.y;
				cori.pix_to_game(tx,ty);
				cori.game_to_pix(tx,ty);
				if(man.x<tx) man.x+=_step;
				if(man.x>tx) man.x-=_step;
				man.run(DOWN, i);
				break;
			}
			man.run(STOP,0);
		}
		break;
	case FIRE_LEFT:
		tx=ty=0;
		if(cori.there(man.x-30, man.y, 5,  DOWN, double_field, BRICK)&&
		   (cori.there(man.x,    man.y, 15, LEFT, double_field, SPACE)||
		    cori.there(man.x,    man.y, 15, LEFT, double_field, BEAM) ||
			cori.there(man.x,    man.y, 15, LEFT, double_field, POLE))) 
		{          
			firestarter=TRUE;  // ����� ��� ��������
			// ��������� �� X 
			tx=man.x;
			ty=man.y;
			cori.pix_to_game(tx,ty);
			cori.game_to_pix(tx,ty);
			if(man.x<tx) man.x+=_step;
			if(man.x>tx) man.x-=_step;
			firestarter=man.fire(dir,double_field);
		}
		else man.run(STOP,0);
		break;

	case FIRE_RIGHT:
		tx=ty=0;
		if( cori.there(man.x+30, man.y, 5,  DOWN,  double_field, BRICK)&&
		   (cori.there(man.x,    man.y, 15, RIGHT, double_field, SPACE)||
		    cori.there(man.x,    man.y, 15, RIGHT, double_field, BEAM) ||
			cori.there(man.x,    man.y, 15, RIGHT, double_field, POLE))) 
		{          
			firestarter=TRUE;  // ����� ��� ��������
			// ��������� �� X 
			tx=man.x;
			ty=man.y;
			cori.pix_to_game(tx,ty);
			cori.game_to_pix(tx,ty);
			if(man.x<tx) man.x+=_step;
			if(man.x>tx) man.x-=_step;
			firestarter=man.fire(dir,double_field);
		}
		else man.run(STOP,0);
		break;
	case STOP:
		if(cori.there(man.x, man.y, 0, UP, double_field, BEAM))
		{
			man.climb(DOWN,0);
			break;
		}
		man.run(dir,0);
		break;
	}
}

