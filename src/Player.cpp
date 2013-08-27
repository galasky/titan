#include <iostream>
#include <unistd.h>
#include "Player.hh"

Player::Player(Piece::Color color)
{
  _type = Player::AI;
  _color = color;
}

Player::~Player()
{

}

action
Player::play(const Plate &plate)
{
  std::string	str;

  (void) plate;
  _tree.make(plate, 3);
  //  plate.goTo(0, 1, point(2, 0));
  usleep(500000);
  std::cin >> str;

  return (action(str));
  return (action(point(1, 0), point(2, 0)));
}

void
Player::setType(Player::Type type)
{
  _type = type;
}

Piece::Color
Player::getColor() const
{
  return (_color);
}
