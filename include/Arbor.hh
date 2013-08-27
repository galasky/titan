#ifndef	__ARBOR_H__
#define	__ARBOR_H__

#include <list>

class	Arbor
{
public:
  Arbor();
  ~Arbor();
private:
  int			_turn;
  int			_profondeur;
  int			_value;
  std::list<Arbor *>	_ram;
};

#endif
