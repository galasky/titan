#include "Piece.hh"

Piece::Piece()
{
  _type = EMPTY;
  _color = NON;
}

Piece::Piece(Piece::Type type, Piece::Color color)
{
  _type = type;
  _color = color;
}

Piece::~Piece()
{

}

void
Piece::operator=(const Piece &piece)
{
  _type = piece._type;
  _color = piece._color;
}

bool
Piece::operator==(const Piece &piece)
{
  if (_type == piece._type && _color == piece._color)
    return true;
  return false;
}

bool
Piece::operator!=(const Piece &piece)
{
  if (_type != piece._type || _color != piece._color)
    return true;
  return false;
}

Piece::Type
Piece::getType() const
{
  return (_type);
}

Piece::Color
Piece::getColor() const
{
  return (_color);
}

int
Piece::getValue() const
{
  return (10 * _type);
}
