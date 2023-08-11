#include "Player.h"
#include <iostream>
#include <string>
#include <climits>

Player::Player(std::string name) {
	m_name = name;
}

std::string Player::name() const{
	return m_name;
}

bool Player::isInteractive() const{
	return false;
}

Player::~Player() {}


//HUMAN PLAYER
HumanPlayer::HumanPlayer(std::string name) : Player(name){}

HumanPlayer::~HumanPlayer(){}

bool HumanPlayer::isInteractive() const {
	return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const {
	if (b.beansInPlay(s) == 0)
		return -1; 
	int m{};

	std::cout << "Select a hole, " << name() << ": ";
	std::cin >> m;

	//for invalid inputs, couts different things depending on input until a valid one is chosen
	while (m > b.holes() || m < 1 || b.beans(s, m) == 0) {
		if (m > b.holes() || (m < 1)) {
			std::cout << "The hole number must be from 1 to " << b.holes() << ". " << std::endl;
		}
		if (b.beans(s, m) == 0 && m != 0) {
			std::cout << "There are no beans in that hole." << std::endl;
		}
		std::cout << "Select a hole, " << name() << ": ";
		std::cin >> m;
	}
	return m;
}

//BAD PLAYER
BadPlayer::BadPlayer(std::string name) : Player(name){}

BadPlayer::~BadPlayer(){}

int BadPlayer::chooseMove(const Board& b, Side s) const {
	if (b.beansInPlay(s) == 0){
		return -1;
	}
	int numHoles = b.holes();

	for (int i = 1; i <= numHoles; i++) {
		if (b.beans(s, i) > 0) {
			return i; // return the first valid move encountered
		}
	}

	return -1; 
}

//SMART PLAYER
//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
	Timer()
	{
		start();
	}
	void start()
	{
		m_time = std::chrono::high_resolution_clock::now();
	}
	double elapsed() const
	{
		std::chrono::duration<double, std::milli> diff =
			std::chrono::high_resolution_clock::now() - m_time;
		return diff.count();
	}
private:
	std::chrono::high_resolution_clock::time_point m_time;
};
//timer class and variable to keep track
Timer timer;

SmartPlayer::SmartPlayer(std::string name) : Player(name){}
SmartPlayer::~SmartPlayer(){}

int SmartPlayer::chooseMove(const Board& b, Side s) const {
	Side mySide = s;
	int bestHole = -1;
	int depth = 8;
	int value = INT_MIN;


	timer.start();
	//depth variable and a mySide variable to keep track of the side as it switches between s and opponent(s) in recursive calls
	minimax(bestHole, value, b, s, depth, mySide);

	return bestHole;

}

void SmartPlayer::minimax(int& bestHole, int& value, const Board& b, Side s, int depth, Side mySide) const {
	
	// base cases for if game is over, timer, and depth
	if (b.beansInPlay(s) == 0) {
		bestHole = -1;
		value = evaluate(b, mySide, s);
		return;
	}
	if (timer.elapsed() >= 4900){
		bestHole = -1;
	 	value = evaluate(b, mySide, s);
		return;
	} 
	if (depth == 0) {
		bestHole = -1;
		value = evaluate(b, mySide, s);
		return;
	}

	else {

			int v2;
			int h2 = -1;
			int firstHole = -1;
			int val;
			int myBestHole = -1;

			if (mySide == s) {
				val = INT_MIN;
			}
			else
				val = INT_MAX;

			//sets it to first hole in case of illegal move(shouldn't matter much though)
			for (int i = 1; i <= b.holes(); i++) {
				if (b.beans(s, i) != 0) {
					firstHole = i;
					break;
				}
			}
			for (int i = 1; i <= b.holes(); i++) {
				if (b.beans(s, i) != 0) {
					// temp board so move is "unmade"
					Board tmp = b;
					Side endside;
					int endhole;
					tmp.sow(s, i, endside, endhole); //sow the board
					//capture logic
					if (endhole != 0 && endside == s && tmp.beans(s, endhole) == 1 && tmp.beans(opponent(s), endhole) != 0) {
						tmp.moveToPot(s, endhole, s);
						tmp.moveToPot(opponent(s), endhole, s);
					}
					//call again for when player gets another turn
					if (endhole == 0 && s == endside) {
						minimax(h2, v2, tmp, s, depth, mySide);
					}
					//make moves for opponents side
					else
						minimax(h2, v2, tmp, opponent(s), depth - 1, mySide);
					if (i == firstHole) {
						myBestHole = i;
					}
					//maximizing side
					if (mySide == s) {
						if (v2 > val){
							val = v2;
							myBestHole = i;
						}

					}
					//minimizing side
					else {
						if (v2 < val){
							val = v2;
							myBestHole = i;
						}
					}
					
				}

			}
			bestHole = myBestHole;
			value = val;

	}
	return;
}




int SmartPlayer::evaluate(const Board& b, Side mySide, Side s) const {
	//evaluates winning move
	int op = b.beansInPlay(opponent(mySide));
	int me = b.beansInPlay(mySide);
	int op_pot = b.beans(opponent(mySide), 0);
	int me_pot = b.beans(mySide, 0);
	if (b.beansInPlay(s) == 0)
	{
		if (me_pot + me > op_pot + op) {
			return (INT_MAX / 2) + ((me_pot + me) - (op_pot + op));
		}
		else if (me_pot + me < op_pot + op) {
			return INT_MIN / 2;
		}
		else { //draw
			return 0;
		}
	}

	//if one side has over half the total beans, they win no matter what
	if (me_pot > (b.totalBeans() / 2))
		return (INT_MAX / 2);
	if (op_pot > (b.totalBeans() / 2))
		return (INT_MIN / 2);
	

	// in normal cases return the difference between mySide's beans + beansinplay minus the opponents
	int value = ((b.beans(mySide, 0) * 10 + b.beansInPlay(mySide))) - (b.beans(opponent(mySide), 0) * 10 - b.beansInPlay(opponent(mySide)));

	return value;
}




