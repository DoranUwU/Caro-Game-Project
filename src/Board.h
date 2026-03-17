#ifndef BOARD_H
#define BOARD_H
#include "raylib.h"
#include "Constants.h"
#include "Position.h"
#include "Player.h"

struct Board {
    Player cell[TOTAL_CELLS]{ Player::NONE };
};

constexpr inline int getIndex(const Position& pos) {
    return pos.row * BOARD_SIZE + pos.column;
}

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

struct GameState;   // forward declare, tránh include vòng tròn

void DrawBoard(int startX, int startY,
    const GameState& gs,
    Texture2D tileLight,
    Texture2D tileDark,
    Texture2D spriteX,
    Texture2D spriteO,
    int curX, int curY);

#endif // BOARD_H