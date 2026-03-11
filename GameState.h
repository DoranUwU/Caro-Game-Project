#pragma once
#include "GameLogic.h"

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

inline GameState playMove(const GameState& currState, const Position& movePos) {
    if (currState.status != GameStatus::ONGOING || !isValidMove(currState.board, movePos)) {
        return currState; // Invalid move or game already ended, return unchanged state
    }

    GameState nextState;
    nextState.board = makeMove(currState.board, movePos, currState.currentPlayer);
    nextState.currentPlayer = getOpponent(currState.currentPlayer);
    nextState.lastMove = movePos;

    if (checkWin(nextState.board, movePos, currState.currentPlayer)) {
        nextState.status = (currState.currentPlayer == Player::PlayerX) ? GameStatus::WIN_X : GameStatus::WIN_O;
    }
    else if (isBoardFull(nextState.board)) {
        nextState.status = GameStatus::DRAW;
    }
    else {
        nextState.status = GameStatus::ONGOING;
    }

    return nextState;
}