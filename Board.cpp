#include "Board.h"
#include <iostream>
#include <string>
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole){
	if (nHoles < 0)
		nHoles = 1;
	m_nHoles = nHoles;

	if (nInitialBeansPerHole < 0)
		nInitialBeansPerHole = 0;
	m_nBPH = nInitialBeansPerHole;

	//pots
	m_northSide.push_back(0);
	m_southSide.push_back(0);

	for (int i = 1; i <= m_nHoles; i++) // push beans to stack
	{
		m_northSide.push_back(m_nBPH);
		m_southSide.push_back(m_nBPH);
	}

}

int Board::holes() const {
	return m_nHoles;
}

int Board::beans(Side s, int hole) const {
	if (hole < 0 || hole > m_nHoles)
		return -1;
	if (s == NORTH){
		return m_northSide.at(hole);
	}
	else {
		return m_southSide.at(hole);
	}
}

int Board::beansInPlay(Side s) const {
	int count = 0;
	if (s == NORTH) {
		for (int i = 1; i <= m_nHoles; i++) {
			count += m_northSide.at(i);
		}
		return count;
	}
	else {
		for (int i = 1; i <= m_nHoles; i++) {
			count += m_southSide.at(i);
		}
		return count;
	}
}


int Board::totalBeans() const{
	int count = 0;
	for (int i = 0; i <= m_nHoles; i++){
		count = count + m_northSide.at(i) + m_southSide.at(i);
	}
	return count;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
	int count = 0;

	if (hole <= 0 || hole > m_nHoles || beans(s, hole) == 0) {
		return false;
	}

	endHole = hole;
	endSide = s;

	if (s == NORTH) {
		count = m_northSide.at(hole);
	}
	else
		count = m_southSide.at(hole);

	if (s == NORTH) {
		m_northSide.at(hole) = 0; 
	}
	else {
		m_southSide.at(hole) = 0; 
	}

	while (count != 0) {
		if (endHole == 0) { // move to opponents side if in pot
			if (endSide == SOUTH)
				endHole = m_nHoles;
			else
				endHole = 1; 
			endSide = opponent(endSide);
		}
		else if (endHole == m_nHoles && endSide == SOUTH) { // move to south pot if at end of south holes
			if (s == SOUTH) {
				endHole = 0;
			}
			else {
				endHole = m_nHoles; // if we are before opponents pot skip over it back to side
				endSide = opponent(endSide);

			}
		}
		else if (endHole == 1 && endSide == NORTH) { // move to north pot if at end of north holes
			if (s == NORTH) {
				endHole = 0;
			}
			else {

				endHole = 1; // if we are before opponents pot skip over it back to side
				endSide = opponent(endSide);

			}
		}
		//advance position
		else {
			if (endSide == NORTH)
				endHole--;
			else
				endHole++;
		}
		//increment beans
		if (endSide == NORTH) {
			m_northSide.at(endHole)++;
		}
		else {
			m_southSide.at(endHole)++;
		}

		count--;
	}

	return true;
}


bool Board::moveToPot(Side s, int hole, Side potOwner) {
	if (hole <= 0 || hole > m_nHoles) {
		return false;
	}

	int num = 0;
	if (s == NORTH) {
		num = m_northSide.at(hole);
		m_northSide.at(hole) = 0;
	}
	else {
		num = m_southSide.at(hole);
		m_southSide.at(hole) = 0;
	}

	if (potOwner == NORTH) {
		m_northSide.at(0) += num;
		return true;
	}
	else {
		m_southSide.at(0) += num;
		return true;
	}
}


bool Board::setBeans(Side s, int hole, int beans) {
	if (hole < 0 || hole > m_nHoles) {
		return false;
	}
	if (s == NORTH) {
		m_northSide.at(hole) = beans;
		return true;
	}
	else {
		m_southSide.at(hole) = beans;
		return true;
	}
}
