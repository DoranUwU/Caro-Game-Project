#pragma once
#include "Board.h"
#include "Player.h"
#include "Position.h"
#include "Constants.h"
#include "GameState.h"

constexpr inline int countConsecutive(const Board& board, const Position& startPos, const Position& direction, Player player) {
    int count = 0;
    Position currentPos = startPos + direction;
    while (!isOutsideBound(currentPos) && getPlayerAt(board, currentPos) == player) {
        count++;
        currentPos = currentPos + direction;
    }
    return count;
}

inline bool checkWin(const Board& board, const Position& pos, Player player) {
    if (player == Player::NONE) return false;

    if (countConsecutive(board, pos, Direction::North, player) + countConsecutive(board, pos, Direction::South, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    if (countConsecutive(board, pos, Direction::East, player) + countConsecutive(board, pos, Direction::West, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    if (countConsecutive(board, pos, Direction::Northeast, player) + countConsecutive(board, pos, Direction::Southwest, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    if (countConsecutive(board, pos, Direction::Northwest, player) + countConsecutive(board, pos, Direction::Southeast, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    return false;
}

inline bool isBoardFull(const Board& board) {
    for (int i = 0; i < TOTAL_CELLS; i++) {
        if (board.cell[i] == Player::NONE) {
            return false;
        }
    }
    return true;
}

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