#ifndef	__PLAYER_HH__
#define	__PLAYER_HH__

#include "Plate.hh"
#include "Tree.hh"

class	Player
{
public:
  enum	Type
    {
      HUMAN,
      AI
    };

  Player(Piece::Color);
  ~Player();

  action	play(const Plate &);
  void		setType(Player::Type);
  Piece::Color	getColor() const;
private:
  Player::Type	_type;
  Tree		_tree;
  Piece::Color	_color;
};

#endif
