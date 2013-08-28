#include <stdlib.h>
#include <iostream>
#include "Plate.hh"

Plate::Plate()
{
  _plate.resize(8);
  for (int i = 0; i < 8; i++)
    _plate[i].resize(8);

  for (int x = 0; x < 8; x++)
    {
      _plate[1][x] = Piece(Piece::PION, Piece::BLACK);
      _plate[6][x] = Piece(Piece::PION, Piece::WHITE);
    }

  _plate[0][0] = Piece(Piece::TOUR, Piece::BLACK);
  _plate[0][1] = Piece(Piece::CAVALIER, Piece::BLACK);
  _plate[0][2] = Piece(Piece::FOU, Piece::BLACK);
  _plate[0][3] = Piece(Piece::REINE, Piece::BLACK);
  _plate[0][4] = Piece(Piece::ROI, Piece::BLACK);
  _plate[0][5] = Piece(Piece::FOU, Piece::BLACK);
  _plate[0][6] = Piece(Piece::CAVALIER, Piece::BLACK);
  _plate[0][7] = Piece(Piece::TOUR, Piece::BLACK);

  _plate[7][0] = Piece(Piece::TOUR, Piece::WHITE);
  _plate[7][1] = Piece(Piece::CAVALIER, Piece::WHITE);
  _plate[7][2] = Piece(Piece::FOU, Piece::WHITE);
  _plate[7][3] = Piece(Piece::REINE, Piece::WHITE);
  _plate[7][4] = Piece(Piece::ROI, Piece::WHITE);
  _plate[7][5] = Piece(Piece::FOU, Piece::WHITE);
  _plate[7][6] = Piece(Piece::CAVALIER, Piece::WHITE);
  _plate[7][7] = Piece(Piece::TOUR, Piece::WHITE);

  _points.resize(7);
  _points[Piece::EMPTY] = NULL;
  _points[Piece::PION] = &Plate::getPointsPION;
  _points[Piece::TOUR] = &Plate::getPointsTOUR;
  _points[Piece::CAVALIER] = &Plate::getPointsCAVALIER;
  _points[Piece::FOU] = &Plate::getPointsFOU;
  _points[Piece::REINE] = &Plate::getPointsREINE;
  _points[Piece::ROI] = &Plate::getPointsROI;

  _buffSword.LoadFromFile("materials/sound/sword.ogg");
  _soundSword.SetBuffer(_buffSword);
}

void
Plate::draw()
{
  for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
	std::cout << _plate[y][x].getType();
      std::cout << std::endl;
    }
}

void
Plate::reverse()
{
  Piece	tmp;

  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 8; x++)
	{
	  tmp = _plate[y][x];
	  _plate[y][x] = _plate[7 - y][7 - x];
	  _plate[7 - y][7 - x] = tmp;
	}
    }
}

Plate::~Plate()
{
}

point
Plate::findKing(Piece::Color color)
{
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      if ( _plate[y][x].getType() == Piece::ROI && _plate[y][x].getColor() == color)
	return (point(y, x));
  return (point(-1, -1));
}

bool
Plate::echec(Piece::Color color)
{
  std::list<point>	listSave = _listPoints;
  point			pKing = this->findKing(color);

  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      if (_plate[y][x].getColor() != color && _plate[y][x].getType() != Piece::EMPTY)
	{
	  getPoints(x, y);
	  for (std::list<point>::iterator it = _listPoints.begin(); it != _listPoints.end(); ++it)
	    {
	      if (*it == pKing)
		{
		  _listPoints = listSave;
		  return true;
		}
	    }
	}
  _listPoints = listSave;
  return false;
}

bool
Plate::echecEtMat(Piece::Color color)
{
  std::list<point>			listSave = _listPoints;
  std::vector<std::vector<Piece> >	pSave;

  pSave << _plate;
  if (this->echec(color))
    {
      for (int y = 0; y < 8; y++)
	{
	  for (int x = 0; x < 8; x++)
	    {
	      if (_plate[y][x].getColor() == color)
		{
		  std::cout << x << " " << y << std::endl;
		  std::cout << "i got points\n";
		  for (std::list<point>::iterator it = _listPoints.begin(); it != _listPoints.end(); ++it)
		    {
		      this->getPoints(y, x);
		      std::cout << "next move " << it->x << " " << it->y << "\n";
		      this->move(color, action(point(y, x), *it));
		      std::cout << "move\n";
		      if (this->echec(color))
			_plate << pSave;
		      else
			{
			  _listPoints = listSave;
			  return false;
			}
		    }
		}
	    }
	}
      _listPoints = listSave;
      return true;
    }
  _listPoints = listSave;
  return false;
}


int
Plate::getValue(Piece::Color color)
{
  int	value = 0;

  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      if (_plate[y][x].getColor() == color)
	value += _plate[y][x].getValue();
  return (value);
}

bool
Plate::move(Piece::Color color, action a)
{
  if (_plate[a.p1.y][a.p1.x].getColor() != color)
    return false;
  getPoints(a.p1.x, a.p1.y);
  for (std::list<point>::iterator it = _listPoints.begin(); it != _listPoints.end(); ++it)
    {
      if (a.p2 == *it)
	{
	  if (_plate[it->y][it->x].getType() != Piece::EMPTY)
	    _soundSword.Play();
	  _plate[a.p2.y][a.p2.x] = _plate[a.p1.y][a.p1.x];
	  _plate[a.p1.y][a.p1.x] = Piece();
	  return true;
	}
    }
  return false;
}

std::list<point>
Plate::getPoints(int x, int y)
{
  _listPoints.clear();
  if (_points[_plate[y][x].getType()] != NULL)
    (this->*_points[_plate[y][x].getType()])(x, y);
  return (_listPoints);
}

void
Plate::getPointsPION(int x, int y)
{
  if (y > 0 && _plate[y - 1][x] == Piece())
    {
      _listPoints.push_back(point(y - 1, x));
      if (y == 6)
	if (_plate[y - 2][x] == Piece())
	  _listPoints.push_back(point(y - 2, x));
    }
  if (y > 0 && x < 7 && _plate[y - 1][x + 1].getColor() != _plate[y][x].getColor() && _plate[y - 1][x + 1].getColor() != Piece::NON)
    _listPoints.push_back(point(y - 1, x + 1));
  if (y > 0 && x > 0 && _plate[y - 1][x - 1].getColor() != _plate[y][x].getColor() && _plate[y - 1][x - 1].getColor() != Piece::NON)
    _listPoints.push_back(point(y - 1, x - 1));      
}

void
Plate::getPointsTOUR(int x, int y)
{
  int	b;

  for (b = 1; y + b < 8 && _plate[y + b][x] == Piece(); b++)
    _listPoints.push_back(point(y + b, x));
  if (y + b < 8 && _plate[y + b][x] != Piece() && _plate[y + b][x].getColor() != _plate[y][x].getColor())
    _listPoints.push_back(point(y + b, x));
  for (b = 1; y - b >= 0 && _plate[y - b][x] == Piece(); b++)
    _listPoints.push_back(point(y - b, x));
  if (y - b >= 0 && _plate[y - b][x] != Piece() && _plate[y - b][x].getColor() != _plate[y][x].getColor())
    _listPoints.push_back(point(y - b, x));

  for (b = 1; x + b < 8 && _plate[y][x + b] == Piece(); b++)
    _listPoints.push_back(point(y, x + b));
  if (x + b < 8 && _plate[y][x + b] != Piece() && _plate[y][x + b].getColor() != _plate[y][x].getColor())
    _listPoints.push_back(point(y, x + b));
  for (b = 1; x - b >= 0 && _plate[y][x - b] == Piece(); b++)
    _listPoints.push_back(point(y, x - b));
  if (x - b >= 0 && _plate[y][x - b] != Piece() && _plate[y][x - b].getColor() != _plate[y][x].getColor())
    _listPoints.push_back(point(y, x - b));
}

void
Plate::getPointsCAVALIER(int x, int y)
{
  if ((y < 7 && x < 6) && (_plate[y + 1][x + 2] == Piece() || _plate[y + 1][x + 2].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 1, x + 2));
  if ((y > 0 && x < 6) && (_plate[y - 1][x + 2] == Piece() || _plate[y - 1][x + 2].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 1, x + 2));
  if ((y < 7 && x > 1) && (_plate[y + 1][x - 2] == Piece() || _plate[y + 1][x - 2].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 1, x - 2));
  if ((y > 0 && x > 1) && (_plate[y - 1][x - 2] == Piece() || _plate[y - 1][x - 2].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 1, x - 2));

  if ((y < 6 && x < 7) && (_plate[y + 2][x + 1] == Piece() || _plate[y + 2][x + 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 2, x + 1));

  if ((y > 1 && x < 7) && (_plate[y - 2][x + 1] == Piece() || _plate[y - 2][x + 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 2, x + 1));

  if ((y < 6 && x > 0) && (_plate[y + 2][x - 1] == Piece() || _plate[y + 2][x - 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 2, x - 1));
  if ((y > 1 && x > 0) && (_plate[y - 2][x - 1] == Piece() || _plate[y - 2][x - 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 2, x - 1));
}

void
Plate::getPointsFOU(int x, int y)
{
  int	a, b;

  for (a = 1, b = 1; x + a < 8 && y + b < 8 && _plate[y + b][x + a] == Piece(); a++, b++)
    _listPoints.push_back(point(y + b, x + a));
  if (y + b < 8 && x + a < 8 && _plate[y + b][x + a] != Piece() && _plate[y + b][x + a].getColor() != _plate[y][x].getColor())  
    _listPoints.push_back(point(y + b, x + a));

  for (a = 1, b = 1; x - a >= 0 && y + b < 8 && _plate[y + b][x - a] == Piece(); a++, b++)
    _listPoints.push_back(point(y + b, x - a));
  if (y + b < 8 && x - a >= 0 && _plate[y + b][x - a] != Piece() && _plate[y + b][x - a].getColor() != _plate[y][x].getColor())  
    _listPoints.push_back(point(y + b, x - a));

  for (a = 1, b = 1; x - a >= 0 && y - b >= 0 && _plate[y - b][x - a] == Piece(); a++, b++)
    _listPoints.push_back(point(y - b, x - a));
  if (y - b >= 0 && x - a >= 0 && _plate[y - b][x - a] != Piece() && _plate[y - b][x - a].getColor() != _plate[y][x].getColor())  
    _listPoints.push_back(point(y - b, x - a));

  for (a = 1, b = 1; x + a < 8 && y - b >= 0 && _plate[y - b][x + a] == Piece(); a++, b++)
    _listPoints.push_back(point(y - b, x + a));
  if (y - b >= 0 && x + a < 8 && _plate[y - b][x + a] != Piece() && _plate[y - b][x + a].getColor() != _plate[y][x].getColor())  
    _listPoints.push_back(point(y - b, x + a));
}

void
Plate::getPointsREINE(int x, int y)
{
  getPointsTOUR(x, y);
  getPointsFOU(x, y);
}

void
Plate::getPointsROI(int x, int y)
{
  if (x < 7 && (_plate[y][x + 1] == Piece() || _plate[y][x + 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y, x + 1));
  if ((x < 7 && y < 7) && (_plate[y + 1][x + 1] == Piece() || _plate[y + 1][x + 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 1, x + 1));
  if (y < 7 && (_plate[y + 1][x] == Piece() || _plate[y + 1][x].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 1, x));
  if (x > 0 && (_plate[y][x - 1] == Piece() || _plate[y][x - 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y, x - 1));
  if ((y > 0 && x > 0) && (_plate[y - 1][x - 1] == Piece() || _plate[y - 1][x - 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 1, x - 1));
  if (y > 0 && (_plate[y - 1][x] == Piece() || _plate[y - 1][x].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 1, x));
  if ((y < 7 && x > 0) && (_plate[y + 1][x - 1] == Piece() || _plate[y + 1][x - 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y + 1, x - 1));
  if ((y > 0 && x < 7) && (_plate[y - 1][x + 1] == Piece() || _plate[y - 1][x + 1].getColor() != _plate[y][x].getColor()))
    _listPoints.push_back(point(y - 1, x + 1));
}

action::action(const std::string &str)
{
  std::string c;

  p1.x = 0;
  p1.y = 0;
  p2.x = 0;
  p2.y = 0;

  if (str.size() == 4)
    {
      c = str[0];
      p1.x = atoi(c.c_str());
      c = str[1];
      p1.y = atoi(c.c_str());
      c = str[2];
      p2.x = atoi(c.c_str());
      c = str[3];
      p2.y = atoi(c.c_str());
    }


  (void) str;
}

Piece
Plate::access(int y, int x) const
{
  return (_plate[y][x]);
}

void	
Plate::operator=(const Plate &other)
{
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      _plate[y][x] = other.access(y, x);	  
}

void	operator<<(std::vector<std::vector<Piece> > &a, const std::vector<std::vector<Piece> > &b)
{
  a.resize(b.size());
  for (int y = 0; y < 8; y++)
    {
      a[y].resize(b[y].size());
      for (int x = 0; x < 8; x++)
	{
	  a[y][x] = b[y][x];
	}
    }
}
