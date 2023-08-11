#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Side.h"
#include "Player.h"
#include "Board.h"


class Game {
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move(Side s);
	void play();
	int beans(Side s, int hole) const;
private:
	Player* m_north;
	Player* m_south;
	Board m_board;
	Side m_turn;
};

#endif //GAME_INCLUDED
