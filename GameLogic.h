#pragma once
#include "Board.h"

constexpr inline int countConsecutive(const Board& board, const Position& startPos, const Position& direction, Player player) {
    int count = 0;
    Position currentPos = startPos + direction;
    while (!isOutsideBound(currentPos) && getPlayerAt(board, currentPos) == player) {
        count++;
        currentPos = currentPos + direction;
    }
    return count;
}

constexpr inline bool checkWin(const Board& board, const Position& pos, Player player) {
    if (player == Player::NONE) return false; // No need to check for empty cell

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

constexpr inline bool isBoardFull(const Board& board) {
    for (int i = 0; i < TOTAL_CELLS; i++) {
        if (board.cell[i] == Player::NONE) {
            return false;
        }
    }
    return true;
}
