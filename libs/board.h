#ifndef BOARD_H
#define BOARD_H
#include "raylib.h"
#include "types.h"

// ============================================================
//  BOARD LOGIC
// ============================================================

// Reset bàn cờ về trạng thái ban đầu
void InitBoard(AppContext& ctx);

// Đặt quân tại ô (x, y). Trả về false nếu ô đã có quân.
bool PlacePiece(AppContext& ctx, int x, int y);

//  BOARD DRAWING
void DrawBoard(int startX, int startY,
    const int board[BOARD_SIZE][BOARD_SIZE],
    Texture2D tileLight,
    Texture2D tileDark,
    Texture2D spriteX,
    Texture2D spriteO,
    int curX, int curY);

#endif // BOARD_H