//===============================================================( UsingGL )===
// ������ ��������� ������������ ���������� ������
//=============================================================================

class Human:private UsingGL
{
public:
	Coordinator cori;
	void _square_human(int x,int y,GLuint tex_mask,GLuint tex_image); // ������ ���� �������� ��������� (1 �� 4)
	int x,y;   // ������� �������� ������
	int person; // 0 - �����; 1 - � 
	int stepping;
	float takt;
	int abc;
	int cba;
	int nono;
	void coord(int _y, int _x) {x=_x*30+40+15; y=_y*30+75+15;} // ���������� ��������� ����������
	void run(_DIRECT ok, int _step);// ������������� �� ������� ������� � ������ �����������  
	void fall(int _step);           // ������ �� step ������
	void climb(_DIRECT dir, int _step);            // ����� �� ����������� �� step ������
	bool fire(_DIRECT dir,int pole[][24]);         // ������ ��������� ����� ����
	_DIRECT find_human(Human &eug, int pole[][24], _DIRECT direct);
	void brick_up(int tx,int ty,int faza);
	void whois();
};

//-----------------------------------------------------------------( whois )---
// ������ �������� � ���������� �� ������������
//-----------------------------------------------------------------------------
void Human::whois()
{
	int i=0,j=0;
	switch(person)
	{
	case 1:
		for(i=0;i<50; texture_human[i]=tex_main[i],i++);
		for(i=50;i<68;texture_human[i]=tex_main[i],i++);
		break;
	case 0:
		for(i=0;i<50;texture_human[i]=tex_enemy[i],i++);
		break;
	}
}
//---------------------------------------------------------( _square_human )---
// ����������� ���������. ������������ ����������.
//-----------------------------------------------------------------------------
void Human::_square_human(int x,int y,GLuint tex_mask,GLuint tex_image)
{
	glEnable(GL_TEXTURE_2D);  // ��������� ��������� ��������
	glEnable(GL_BLEND);       // ���������� ����������
    glDisable(GL_DEPTH_TEST); // ������ ����� �������
	
	glBlendFunc(GL_DST_COLOR,GL_ZERO); 

	glBindTexture(GL_TEXTURE_2D, tex_mask); 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(x-15, y+15, 0);  // ��� ����
		glTexCoord2f(1.0f, 0.0f); glVertex3d(x+15, y+15, 0);  // ��� �����
		glTexCoord2f(1.0f, 1.0f); glVertex3d(x+15, y-15, 0);  // ���� �����
		glTexCoord2f(0.0f, 1.0f); glVertex3d(x-15, y-15, 0);  // ���� ����
	glEnd();
	glBlendFunc(GL_ONE, GL_ONE); 
    glBindTexture(GL_TEXTURE_2D, tex_image); 
    glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(x-15, y+15, 0);  // ��� ����
		glTexCoord2f(1.0f, 0.0f); glVertex3d(x+15, y+15, 0);  // ��� �����
		glTexCoord2f(1.0f, 1.0f); glVertex3d(x+15, y-15, 0);  // ���� �����
		glTexCoord2f(0.0f, 1.0f); glVertex3d(x-15, y-15, 0);  // ���� ����
	glEnd();             

    glEnable(GL_DEPTH_TEST);  
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------------------------------( run )---
// ��������� � ��������� ����������� �� ������ ���-�� ��������(������)
//-----------------------------------------------------------------------------
void Human::run(_DIRECT ok, int _step)
{
	// ����� ��� ���������� ���������
	if(!tex_main[0]||!tex_enemy[0]) // ��������� ��� �������
	{
		for(int i=0,j=0; i<50;  tex_main[i]=_LoadOneTexture(name_human_texture[i]),i++);
		for(i=50; i<68; tex_main[i]=_LoadOneTexture(name_human_texture[i]),i++);
		for(i=0,j=68; i<50; tex_enemy[i]=_LoadOneTexture(name_human_texture[j]),i++,j++);
	}

	whois();

	switch(stepping) // ������ ���������
	{
	case 0: takt= 1; break;
	case 11: takt=-1; break;
	}
	stepping+=takt;
	

	switch(ok)
	{
	case STOP:
		_square_human(x,y,texture_human[abc],texture_human[cba]);
		break;
	case RIGHT:
		x+=_step;
		// ��������� ������ ���������
		_square_human(x,y,texture_human[abc= 6+(stepping/2)],texture_human[cba=0+(stepping/2)]);
		break;
	case LEFT:
		x-=_step;
		_square_human(x,y,texture_human[abc=18+(stepping/2)],texture_human[cba=12+(stepping/2)]);
		break;
	case UP:
		y-=_step;
		_square_human(x,y,texture_human[abc=30+(stepping/2)],texture_human[cba=24+(stepping/2)]);
		break;
	case DOWN:
		y+=_step;
		_square_human(x,y,texture_human[abc=30+(stepping/2)],texture_human[cba=24+(stepping/2)]);
		break;
	}
	
}

//------------------------------------------------------------------( fall )---
// ������ ���� �� step ������
//-----------------------------------------------------------------------------
void Human::fall(int _step)
{
	whois();
	y+=_step;
	_square_human(x,y,texture_human[37],texture_human[36]);
}

//-----------------------------------------------------------------( climb )---
// ������� �� ������������
//-----------------------------------------------------------------------------
void Human::climb(_DIRECT dir, int _step)
{
	whois();
	switch(stepping)
	{
	case 0: takt= 1; break;
	case 11: takt=-1; break;
	}
	stepping+=takt;

	switch(dir)
	{
	case RIGHT:
		x+=_step;
		_square_human(x,y,texture_human[41+(stepping/4)],texture_human[38+(stepping/4)]);
		break;
	case LEFT:
		x-=_step;
		_square_human(x,y,texture_human[47+(stepping/4)],texture_human[44+(stepping/4)]);
		break;
	case DOWN:
		y+=_step;
		_square_human(x,y,texture_human[37],texture_human[36]);
		break;
	}
}

//------------------------------------------------------------------( fire )---
// ����������� ��������� ������� ��������� ���� � �����/������ ����
//-----------------------------------------------------------------------------
bool Human::fire(_DIRECT dir,int pole[][24])
{
	whois();
	int tx=x;
	int ty=y;
	static int ok=0; 
	switch(dir)
	{
	case FIRE_LEFT:
		if(ok==7) // ���������� �������� ���������
		{
			_square_human(x,y,texture_human[67],texture_human[64]); // ������ �����
			cori.pix_to_game(tx,ty);
			pole[ty+1][tx-1]=9;
			ok=0;
			return false; 
		}
		else
		{
			ok++;
			_square_human(x,y,texture_human[65+(ok/3)],texture_human[62+(ok/3)]); // ������������ ��� ���������

			cori.pix_to_game(tx,ty);
			tx--;
			ty++;
			cori.game_to_pix(tx,ty);
		
			_square_human(tx,ty,texture_human[53+(ok/3)],texture_human[50+(ok/3)]);
			return true; 
		}
	break;
	case FIRE_RIGHT:
		if(ok==7) // ���������� �������� ���������
		{
			_square_human(x,y,texture_human[61],texture_human[58]); // ������ �����
			cori.pix_to_game(tx,ty);
			pole[ty+1][tx+1]=9;
			ok=0;
			return false; 
		}
		else
		{
			ok++;
			_square_human(x,y,texture_human[59+(ok/3)],texture_human[56+(ok/3)]); // ������������ ��� ���������
																      
			cori.pix_to_game(tx,ty);
			tx++;
			ty++;
			cori.game_to_pix(tx,ty);

			_square_human(tx,ty,texture_human[53+(ok/3)],texture_human[50+(ok/3)]);
			return true; 
		}
	default:
			return false;
	break;
	}
}

//------------------------------------------------------------( find_human )---
// ����� ������������ ���� ����� ����� �����������
//-----------------------------------------------------------------------------
_DIRECT Human::find_human(Human &eug, int pole[][24], _DIRECT direct)
{
	// x,y          - ���������� �����
	// eug.x, eug.y - ���������� ����
	// ��� ���������� � ��������

	// � ����� ������� �������������� ������������� ��������� (��)

	// ��������������� ����������
	int enix=     x, eniy=     y;
	int eugx= eug.x, eugy= eug.y;
	//1) ��c������� ���������� �� ����� ���� � �����������
	//   ������ ����� ��� �� �� �����
	cori.pix_to_game(enix,eniy);
	cori.pix_to_game(eugx,eugy);
	// � ���������� ��� ��������
	// a) ��������� �� � �� ����� ������ �� ����
	// ���� �� �� ������� ���� �� �����.
	// ���� �� ����� � ������ ��������� �� ������ �� ���� �� ������
	   if(eniy==eugy)
	   {
		   if(pole[eniy][enix]==LADDER&&
			  !(pole[eniy+1][enix]==BRICK||
			    pole[eniy+1][enix]==IMBRICK)) return direct;
		   if(enix>eugx)
		   {
				for(int i=enix;enix>eugx;i--)
					if(!((pole[eniy][i]==SPACE)||
					     (pole[eniy][i]==LODE)||
					     (pole[eniy][i]==LADDER)||
					     (pole[eniy][i]==BEAM))) break; //������ ���������
				return LEFT; 
		   }
		   else//(enix<eugx)
		   {
			    for(int i=enix;enix<eugx;i++)
					if(!((pole[eniy][i]==SPACE)||
					     (pole[eniy][i]==LODE)||
					     (pole[eniy][i]==LADDER)||
					     (pole[eniy][i]==BEAM))) break; //������ ���������
				return RIGHT; 
		   }
		   
	   }

	   bool left=TRUE;
	   bool right=TRUE;
	   //int nono=0;
	// b) ��������� ���� ����
	   if(eniy<eugy)
	   {
		   if(pole[eniy][enix]==LADDER&&
			  !(pole[eniy+1][enix]==BRICK||
			    pole[eniy+1][enix]==IMBRICK)) return direct;
		   //if(pole[eniy][enix]==LADDER) return direct;
		   if(pole[eniy+1][enix]==LADDER&&direct==UP) nono=30;
		   if(nono==0)
		   {
			   if(pole[eniy+1][enix]==LADDER) return DOWN;
		   }
		   else nono--;
		   for(int i=1;i<25;i++)
		   {
			   if(left)
			   {
			     if(pole[eniy+1][enix-i]==LADDER||
					pole[eniy+1][enix-i]==SPACE) 
				 {
					 if((pole[eugy+1][enix-i]==BRICK||pole[eugy+1][enix-i]==IMBRICK)) return LEFT;
					 else left=false; 
				 }
			   }
			   if(right)
			   {
				 if(pole[eniy+1][enix+i]==LADDER||
					pole[eniy+1][enix+i]==SPACE) 
				 {
					 if((pole[eugy+1][enix+i]==BRICK||pole[eugy+1][enix+i]==IMBRICK)) return RIGHT;
					 else right=false;
				 }
			   }
			   if(!left&&!right)
			   {
					if(eniy<=eugx) return RIGHT;
					else           return LEFT;
			   }
		   }
	   }

	// c) ��������� ���� ���� �.� � ����
	   left=TRUE;
	   right=TRUE;
	   _DIRECT kuda=STOP;
	   if(eniy>eugy)
	   {
		   // ��������� ����� ������������ ���� ������
		   // ���������� �����
		   // enix, eniy
		   for(int i=1;i<25;i++) // ������� ����� � ������ � ������� ��������� ��������
		   {
			   if(left)
			   {
				   if(pole[eniy][enix-i]==LADDER) {kuda=LEFT;break;}
				   if(eniy<14)
				   if((pole[eniy+1][enix-i]==SPACE)||
					  (pole[eniy+1][enix-i]==LODE)||
					  (pole[eniy+1][enix-i]==BEAM)) left=false;
			   }
			   if(right)
			   {
				   
				   if(pole[eniy][enix+i]==LADDER) {kuda=RIGHT;break;}
				   if(eniy<14)
				   if((pole[eniy+1][enix+i]==SPACE)||
					  (pole[eniy+1][enix+i]==LODE)||
					  (pole[eniy+1][enix+i]==BEAM)) right=false;
			   }
		   }
		   if(pole[eniy][enix]==LADDER) kuda=UP; // ��� �� ��� �� �������
		   if(kuda!=STOP) return kuda; 
		   // ���� ����� �� ���� ����� �� ��� ������ ���� ���
		   if(enix<=eugx) return RIGHT;
		   else           return LEFT;
		   
		   // ����������
		   // ������� ��� �� �� ��������� ������� ������� ���� ���� ���� ����������
	   }
	return STOP;
}

//--------------------------------------------------------------( brick_up )---
// ���������� ������ ���� �� �� ������� ����� �����
//-----------------------------------------------------------------------------
void Human::brick_up(int tx, int ty,int faza)
{
	whois();
	cori.game_to_pix(tx,ty);
	_square_human(tx,ty,texture_human[53+faza],texture_human[50+faza]);
}
