#include "Game.h"
#include <iostream>

using namespace std;

Game::Game(const Board& b, Player* south, Player* north) : m_north(north), m_south(south), m_board(b), m_turn(SOUTH) {}

void Game::display() const {
    //prints out north name, north holes, both pots, south holes, then south name
    int holes = m_board.holes();
    std::cout << m_north->name() << std::endl << "    ";
    for (int i = 1; i <= holes; i++) {
        std::cout << m_board.beans(NORTH, i) << "  ";
    }
    std::cout << std::endl << "  " << m_board.beans(NORTH, 0);
    for (int i = 0; i < holes; i++) {
        std::cout << "   ";
    }
    std::cout << m_board.beans(SOUTH, 0) << std::endl << "    ";
    for (int i = 1; i <= holes; i++) {
        std::cout << m_board.beans(SOUTH, i) << "  ";
    }
    std::cout << std::endl << m_south->name() << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    if (m_board.beansInPlay(NORTH) != 0 || m_board.beansInPlay(SOUTH) != 0) { //game is not over
        over = false;
        hasWinner = false;
        return;
    }
    over = true;
    if (m_board.beans(NORTH, POT) > m_board.beans(SOUTH, POT)) { //north won
        hasWinner = true;
        winner = NORTH;
    }
    else if (m_board.beans(SOUTH, POT) > m_board.beans(NORTH, POT)) { //south won
        hasWinner = true;
        winner = SOUTH;
    }
    else { // its a draw
        hasWinner = false;
    }
    return;
}

bool Game::move(Side S) {
    //check if game is over
    bool over, hasWinner;
    Side winner;

    status(over, hasWinner, winner);

    if (over)
        return false; //return false if the game is over


    Player* tmp;
    if (S == NORTH)
        tmp = m_north;
    else
        tmp = m_south;

    Side endSide;
    int endHole;
    int hole;

    hole = tmp->chooseMove(m_board, S);

    if (hole != -1) {
        cout << tmp->name() << " chooses hole " << hole << endl;
    }
    else
        std::cout << tmp->name() << " has no beans left to sow." << std::endl;


   
    m_board.sow(S, hole, endSide, endHole);

    //for capturing
    if (endHole != 0 && endSide == S && m_board.beans(S, endHole) == 1 && m_board.beans(opponent(S), endHole) != 0) {
        m_board.moveToPot(S, endHole, S);
        m_board.moveToPot(opponent(S), endHole, S);
    }
    //for if player lands on their pot they get extra turn
    while (endHole == 0 && S == endSide) {

        display();
        cout << tmp->name() << " gets another turn." << endl;

        hole = tmp->chooseMove(m_board, S);


        if (hole == -1)
            std::cout << tmp->name() << " has no beans left to sow." << std::endl;

        if (hole == -1)
            break;
        if (!m_board.sow(m_turn, hole, endSide, endHole))
            break;

        if (endHole != 0 && endSide == S && m_board.beans(S, endHole) == 1 && m_board.beans(opponent(S), endHole) != 0) {
            m_board.moveToPot(S, endHole, S);
            m_board.moveToPot(opponent(S), endHole, S);
        }


        cout << tmp->name() << " chooses hole " << hole << endl;
    }

    //sweep the board
    if (m_board.beansInPlay(NORTH) == 0 && hole == -1) { //game is over
       
        cout << "Sweeping remaining beans into " + m_south->name() + "'s pot." << endl;
        for (int i = 1; i <= m_board.holes(); i++) { //loop through the south holes
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        return false;
    }
    else if (m_board.beansInPlay(SOUTH) == 0 && hole == -1) {

        cout << "Sweeping remaining beans into " + m_north->name() + "'s pot." << endl;
        for (int i = 1; i <= m_board.holes(); i++) { //loop through north's holes
            m_board.moveToPot(NORTH, i, NORTH);
        }
        return false;
    }

    return true;


}


void Game::play() {
    bool over, hasWinner;
    Side winner;
    Player* tmp;

    display();

    status(over, hasWinner, winner);
    //make moves and play the game while it is not over
    do {
        move(m_turn);
        m_turn = opponent(m_turn);
        display();
        if (!m_south->isInteractive() && !m_north->isInteractive()) {
            cout << "Press ENTER to continue.";
            cin.ignore(10000, '\n');
        }
        status(over, hasWinner, winner);
    } while (!over);

    //announce game results once game is over
    if (hasWinner) {
        if (winner == NORTH)
            tmp = m_north;
        else
            tmp = m_south;
        cout << "The winner is " << tmp->name() << "." << endl;

    }
    else
        cout << "The game resulted in a tie." << endl;
}



int Game::beans(Side s, int hole) const {
    if (hole < 0 || hole > m_board.holes())
        return -1;
    else
        return m_board.beans(s, hole);
}