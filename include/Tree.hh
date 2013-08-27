#ifndef	__TREE_H__
#define	__TREE_H__

#include "Plate.hh"
#include "Arbor.hh"

class	Tree
{
public:
  Tree();
  ~Tree();

  void	make(const Plate &, int lvl);
private:
  Arbor	_arbor;
};

#endif
