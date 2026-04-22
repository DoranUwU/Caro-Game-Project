#ifndef BOARD_H
#define BOARD_H
#include "raylib.h"
#include "Constants.h"
#include "Position.h"
#include "Player.h"

struct Board {
    Player cell[TOTAL_CELLS]{ Player::NONE };
};

int getIndex(const Position& pos);

Player getPlayerAt(const Board& board, const Position& pos);

bool isOutsideBound(const Position& pos);

bool isCellEmpty(const Board& board, const Position& pos);

bool isValidMove(const Board& board, const Position& pos);

Board makeMove(const Board& currBoard, const Position& pos, Player player);

struct GameState;   // forward declare, tránh include vòng tròn

void DrawBoard(int startX, int startY,
    const GameState& gs,
    Texture2D tileLight,
    Texture2D tileDark,
    Texture2D spriteX,
    Texture2D spriteO,
    int curX, int curY);

#endif // BOARD_H