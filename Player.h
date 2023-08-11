#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <string>
#include <iostream>
#include "Side.h"
#include "Board.h"




class Player {
public:
	Player(std::string name);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;
};

class HumanPlayer : public Player {
public:
	HumanPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual ~HumanPlayer();
	virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player {
public:
	BadPlayer(std::string name);
	virtual ~BadPlayer();
	virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player {
public:
	SmartPlayer(std::string name);
	virtual ~SmartPlayer();
	virtual int chooseMove(const Board& b, Side s) const;
private:
	void minimax(int& bestHole, int& value, const Board& b, Side s, int depth, Side mySide) const;
	int evaluate(const Board&, Side mySide, Side s) const;
};




#endif //PLAYER_INCLUDED
