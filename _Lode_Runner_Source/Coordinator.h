//===========================================================( Coordonator )===
// ����� �������������� ��� ��� ������������ ������������ ����������:
// --> �������� (field[15][24])
// --> ���������� (800�600)
//=============================================================================
class Coordinator
{
protected:

public:
	// ������� ��������� �� ���������� � �������� 
	void pix_to_game(int &_x, int &_y) {_x=(_x-40)/30; _y=(_y-75)/30;}
	// ������� ��������� �� �������� � ����������
	void game_to_pix(int &_x, int &_y) {_x=_x*30+40+15; _y=_y*30+75+15;}
	// ��������� ��������� � ����
	bool pole(int _x, int _y); 
	// ������: 
	// - ����� st (�������) ��������� ��:
	// - ���������� x,y
	// - c ����������� _step �����
	// - � _dir �������
	// - �� ���� _field[][24]
	bool there(int x, int y, int _step, _DIRECT dir, int _field[][24], _STATIC st);
};

//------------------------------------------------------------------( pole )---
// �������� �� ����� � ����
//-----------------------------------------------------------------------------
bool Coordinator::pole(int _x, int _y)
{
	return ((_x>39&&_x<760)&&(_y>74&&_y<525))? true: false; 
}

//-----------------------------------------------------------------( there )---
// ������ ����� ������� ���������
//-----------------------------------------------------------------------------
bool Coordinator::there(int x, int y, int _step, _DIRECT dir,
						int _field[][24], _STATIC st)
{
	int _tx=0, _ty=0;
	switch(dir)
	{
	case RIGHT:
		_ty=y;
		_tx=x+(14+_step);
		break;
	case LEFT:
		_ty=y;
		_tx=x-(15+_step);
		break;
	case UP:
		_ty=y-(15+_step);
		_tx=x;
		break;
	case DOWN:
		_ty=y+(14+_step);
		_tx=x;
		break;
	case STOP:
		_ty=y;
		_tx=x;
		break;
	}
	// ������� �������� ����
	if(!pole(_tx,_ty)) return false;

	pix_to_game(_tx, _ty);
	
	if(_field[_ty][_tx]==(int)st) return true;
	else                          return false;
}
