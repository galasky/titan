#ifndef	__PIECE_HH__
#define	__PIECE_HH__

class	Piece
{
public:

enum	Type
  {
    EMPTY,
    PION,
    TOUR,
    FOU,
    CAVALIER,
    REINE,
    ROI
  };

  enum	Color
    {
      NON,
      WHITE,
      BLACK
    };

  Piece();
  Piece(Piece::Type, Piece::Color);
  ~Piece();

  void operator=(const Piece &);
  bool operator==(const Piece &);
  bool operator!=(const Piece &);

  Piece::Type	getType() const;
  Piece::Color	getColor() const;
  int		getValue() const;

private:
  Type	_type;
  Color	_color;
};

#endif
