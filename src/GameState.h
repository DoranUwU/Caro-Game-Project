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

struct GameState {
    Board board = Board();
    Player currentPlayer = Player::PlayerX;
    GameStatus status = GameStatus::ONGOING;

    Position lastMove = Position(-1, -1); // Track the last move for win checking
};

#endif // GAMESTATE_H