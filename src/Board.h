#pragma once
#include "Constants.h"
#include "Position.h"
#include "Player.h"

struct Board {
    Player cell[TOTAL_CELLS]{ Player::NONE };
};

constexpr inline int getIndex(const Position& pos) {
    return pos.row * BOARD_SIZE + pos.column;
} //translate 2D to 1D index

constexpr inline Player getPlayerAt(const Board& board, const Position& pos) {
    return board.cell[getIndex(pos)];
}

constexpr inline bool isOutsideBound(const Position& pos) {
    return pos.row < 0 || pos.row >= BOARD_SIZE || pos.column < 0 || pos.column >= BOARD_SIZE;
}

constexpr inline bool isCellEmpty(const Board& board, const Position& pos) {
    return getPlayerAt(board, pos) == Player::NONE;
}

constexpr inline bool isValidMove(const Board& board, const Position& pos) {
    return !isOutsideBound(pos) && isCellEmpty(board, pos);
}

inline Board makeMove(const Board& currBoard, const Position& pos, Player player) {
    Board newBoard = currBoard;
    newBoard.cell[getIndex(pos)] = player;
    return newBoard;
}
