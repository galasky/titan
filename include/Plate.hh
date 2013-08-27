
#ifndef	__PLATE_HH__
#define	__PLATE_HH__

#include <SFML/Audio.hpp>
#include <vector>
#include <list>
#include <string>
#include "Piece.hh"

class	point
{
public:
  point(int Y, int X) {x = X; y = Y;}
  point(const point &p) {x = p.x; y = p.y;}
  point() {}

  bool	operator==(const point &p) {if (x == p.x && y == p.y) return true; return false;}
  void	operator=(const point &p) {x = p.x; y = p.y;}

  int		x;
  int		y;
};

class	action
{
public:
  action() {}
  action(const std::string &);
  action(point P1, point P2) : p1(P1), p2(P2) {}
  ~action() {}

  void	operator=(const action &a) {p1 = a.p1; p2 = a.p2;}

  point p1;
  point p2;
};

class	Plate
{
public:
  Plate();
  ~Plate();

  void			draw();
  void			reverse();

  point			findKing(Piece::Color);
  bool			echec(Piece::Color);

  int			getValue(Piece::Color);
  std::list<point>	getPoints(int x, int y);
  bool			move(Piece::Color, action);
  Piece			access(int y, int x) const;

  void  operator=(const Plate &);
private:
  std::vector<std::vector<Piece> >		_plate;
  std::vector<void (Plate::*)(int x, int y)>	_points;
  std::list<point>				_listPoints;

  sf::Sound                     _soundSword;
  sf::SoundBuffer               _buffSword;


  void	getPointsPION(int x, int y);
  void	getPointsTOUR(int x, int y);
  void	getPointsCAVALIER(int x, int y);
  void	getPointsFOU(int x, int y);
  void	getPointsREINE(int x, int y);
  void	getPointsROI(int x, int y);
};

#endif	//__PLATE_HH__
