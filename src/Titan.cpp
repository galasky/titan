#include <unistd.h>
#include <iostream>
#include "Titan.hh"

Titan::Titan() : _p1(Piece::WHITE), _p2(Piece::BLACK), _window(sf::VideoMode(992, 992), "SFML window")
{
  _end = false;

  _actions.resize(4);
  _stape = 0;
  _keyRelease = true;

  _p1.setType(Player::HUMAN);
  _p2.setType(Player::AI);

  _black.resize(6);
  _black2.resize(6);
  _white.resize(6);
  _white2.resize(6);
  _sprite.resize(8);
  for (int i = 0; i < 8; i++)
    _sprite[i].resize(8);

  _iSelection.LoadFromFile("materials/texture/selection/selection.png");
  _sSelection.SetImage(_iSelection);
  _sSelection.SetColor(sf::Color(125, 0, 0, 255));

  _iOk.LoadFromFile("materials/texture/selection/ok.png");
  _sOk.SetImage(_iOk);
  _sOk.SetColor(sf::Color(255, 0, 0, 100));

  _emptyBlack.LoadFromFile("materials/texture/black/black.jpg");
  _emptyWhite.LoadFromFile("materials/texture/white/white.jpg");

  _black[0].LoadFromFile("materials/texture/black/pion.jpg");
  _black[1].LoadFromFile("materials/texture/black/tour.jpg");
  _black[2].LoadFromFile("materials/texture/black/fou.jpg");
  _black[3].LoadFromFile("materials/texture/black/cavalier.jpg");
  _black[4].LoadFromFile("materials/texture/black/reine.jpg");
  _black[5].LoadFromFile("materials/texture/black/roi.jpg");

  _white[0].LoadFromFile("materials/texture/white/pion.jpg");
  _white[1].LoadFromFile("materials/texture/white/tour.jpg");
  _white[2].LoadFromFile("materials/texture/white/fou.jpg");
  _white[3].LoadFromFile("materials/texture/white/cavalier.jpg");
  _white[4].LoadFromFile("materials/texture/white/reine.jpg");
  _white[5].LoadFromFile("materials/texture/white/roi.jpg");

  _black2[0].LoadFromFile("materials/texture/black/pion2.jpg");
  _black2[1].LoadFromFile("materials/texture/black/tour2.jpg");
  _black2[2].LoadFromFile("materials/texture/black/fou2.jpg");
  _black2[3].LoadFromFile("materials/texture/black/cavalier2.jpg");
  _black2[4].LoadFromFile("materials/texture/black/reine2.jpg");
  _black2[5].LoadFromFile("materials/texture/black/roi2.jpg");

  _white2[0].LoadFromFile("materials/texture/white/pion2.jpg");
  _white2[1].LoadFromFile("materials/texture/white/tour2.jpg");
  _white2[2].LoadFromFile("materials/texture/white/fou2.jpg");
  _white2[3].LoadFromFile("materials/texture/white/cavalier2.jpg");
  _white2[4].LoadFromFile("materials/texture/white/reine2.jpg");
  _white2[5].LoadFromFile("materials/texture/white/roi2.jpg");

  _buffSound.LoadFromFile("materials/sound/no.ogg");
  _soundNo.SetBuffer(_buffSound);

  _buffEchec.LoadFromFile("materials/sound/echecetmat.ogg");
  _soundEchec.SetBuffer(_buffEchec);

  _buffPop.LoadFromFile("materials/sound/pop.ogg");
  _soundPop.SetBuffer(_buffPop);

  for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
        {
	  _sprite[y][x].SetPosition(x * 124, y * 124);
	}
    }
}

Titan::~Titan()
{

}

void
Titan::draw()
{
  for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
	{
	  if ((x + y) % 2 == 0)
	    _sprite[y][x].SetImage(_emptyWhite);
	  else
	    _sprite[y][x].SetImage(_emptyBlack);

	  if (_plate.access(y, x).getType() != Piece::EMPTY)
	    {
	      if (_plate.access(y, x).getColor() == Piece::WHITE)
		{
		  if ((x + y) % 2 == 0)
		    _sprite[y][x].SetImage(_white[_plate.access(y, x).getType() - 1]);
		  else
		    _sprite[y][x].SetImage(_white2[_plate.access(y, x).getType() - 1]);
		}
	      else
		{
		  if ((x + y) % 2 == 0)
		    _sprite[y][x].SetImage(_black[_plate.access(y, x).getType() - 1]);
		  else
		    _sprite[y][x].SetImage(_black2[_plate.access(y, x).getType() - 1]);
		}
	    }
	}
    }



  _window.Clear();

  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      _window.Draw(_sprite[y][x]);

  if (_stape == 1)
    {
      std::list<point> list =  _plate.getPoints(_actions[0].x, _actions[0].y);

      for (std::list<point>::iterator it = list.begin(); it != list.end(); ++it)
	{
	  _sOk.SetPosition(it->x * 124, it->y * 124);
	  _window.Draw(_sOk);
	}

      _sSelection.SetPosition(_actions[0].x * 124, _actions[0].y * 124);
      _window.Draw(_sSelection);
    }
  else if (_stape == 3)
    {
      std::list<point> list =  _plate.getPoints(_actions[2].x, _actions[2].y);

      for (std::list<point>::iterator it = list.begin(); it != list.end(); ++it)
	{
	  _sOk.SetPosition(it->x * 124, it->y * 124);
	  _window.Draw(_sOk);
	}

      _sSelection.SetPosition(_actions[2].x * 124, _actions[2].y * 124);
      _window.Draw(_sSelection);
    }

  if (_plate.echec(Piece::WHITE))
    {
      _sOk.SetPosition(_plate.findKing(Piece::WHITE).x * 124, _plate.findKing(Piece::WHITE).y * 124);
      _window.Draw(_sOk);
    }
  if (_plate.echec(Piece::BLACK))
    {
      _sOk.SetPosition(_plate.findKing(Piece::BLACK).x * 124, _plate.findKing(Piece::BLACK).y * 124);
      _window.Draw(_sOk);
    }
  _window.Display();
}

void
Titan::doStape()
{
  Plate pSave = _plate;

  if (_stape == 0 && _plate.access(_actions[_stape].y, _actions[_stape].x).getColor() != _p1.getColor())
    return ;
  if (_stape == 2 && _plate.access(_actions[_stape].y, _actions[_stape].x).getColor() != _p2.getColor())
    return ;
  if (_stape == 1 && _plate.move(_p1.getColor(), action(_actions[0], _actions[1])) == false)
    {
      _stape = 0;
      return ;
    }

  if (_stape == 3 && _plate.move(_p2.getColor(), action(_actions[2], _actions[3])) == false)
    {
      _stape = 2;
      return ;
    }

  /*  if (_stape == 3 && _plate.echec(Piece::WHITE) && _plate.echecEtMat(Piece::WHITE))
      {
      _end = true;
      return;
      }

      if (_stape == 1 && _plate.echec(Piece::BLACK) && _plate.echecEtMat(Piece::BLACK))
      {
      _end = true;
      return;
      }
  */

  if (_stape == 1 || _stape == 3)
    _soundPop.Play();

  if (_stape == 1 && _plate.echec(Piece::WHITE))
    {
      _soundNo.Play();
      _plate = pSave;
      return ;
    }

  if (_stape == 3 && _plate.echec(Piece::BLACK))
    {
      _soundNo.Play();
      _plate = pSave;
      return ;
    }

  _stape++;
  if (_stape == 4)
    _stape = 0;

  if (_stape == 0 || _stape == 2)
    {
      this->draw();
      usleep(500000);
      _plate.reverse();
    }
}

void
Titan::catchEvent()
{
  while (_window.GetEvent(_event))
    {
      if (_event.Type == sf::Event::Closed)
	_window.Close();
      if (_end == false)
	{
	  if (_keyRelease == true && _event.Type == sf::Event::MouseButtonPressed)
	    {
	      _keyRelease = false;
	      _actions[_stape] = point(_event.MouseButton.Y / 124, _event.MouseButton.X / 124);
	      //	  std::cout << _event.MouseButton.Y / 124 << " " <<  _event.MouseButton.X / 124  << std::endl;
	      this->doStape();
	    }
	  if ( _event.Type == sf::Event::MouseButtonReleased)
	    {
	      _keyRelease = true;
	    }
	}
    }
}

void
Titan::run()
{
  action a;

  while (_window.IsOpened())
    {
      this->draw();
      this->catchEvent();	
      usleep(10000);
    }
}
