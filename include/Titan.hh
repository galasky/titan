#ifndef	__TITAN_HH__
#define	__TITAN_HH__

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Plate.hh"
#include "Player.hh"

class	Titan
{
public:
  Titan();
  ~Titan();

  void	run();
private:

  void	draw();
  void	catchEvent();
  void	doStape();

  Plate		_plate;
  Player	_p1;
  Player	_p2;

  sf::RenderWindow		_window;
  sf::Event			_event;

  sf::Image			_emptyBlack;
  sf::Image			_emptyWhite;

  sf::Image			_iSelection;
  sf::Sprite			_sSelection;
  sf::Image			_iOk;
  sf::Sprite			_sOk;

  sf::Sound			_soundNo;
  sf::SoundBuffer		_buffSound;

  sf::Sound			_soundEchec;
  sf::SoundBuffer		_buffEchec;

  sf::Sound			_soundPop;
  sf::SoundBuffer		_buffPop;

  std::vector<sf::Image>	_black;
  std::vector<sf::Image>	_black2;
  std::vector<sf::Image>	_white;
  std::vector<sf::Image>	_white2;

  std::vector<std::vector<sf::Sprite> > _sprite;

  std::vector<point>		_actions;
  int				_stape;
  bool				_keyRelease;

  bool				_end;
};

#endif
