#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include "Side.h"
#include <iostream>
#include <vector>

class Board {
public:
	Board(int nHoles, int nInitialBeansPerHole);
	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
private:
	int m_nHoles;
	int m_nBPH;
	std::vector<int> m_northSide;
	std::vector<int> m_southSide;

};


#endif //BOARD_INCLUDED