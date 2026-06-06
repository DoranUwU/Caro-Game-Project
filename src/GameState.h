#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Board.h"
#include "Player.h"
#include "Position.h"

enum class GameStatus {
    ONGOING,
    DRAW,
    WIN_X,
    WIN_O
};

constexpr int MAX_WIN_LINE = 10;

struct GameState {
    Board board = Board();
    Player currentPlayer = Player::PlayerX;
    GameStatus status = GameStatus::ONGOING;

    Position lastMove = Position(-1, -1);

    Position winLine[MAX_WIN_LINE];
    int winLineCount = 0;

    long long saveTime = 0;
};


#endif // GAMESTATE_H