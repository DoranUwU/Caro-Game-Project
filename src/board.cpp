#include "../libs/board.h"
#include <math.h>

//  InitBoard
void InitBoard(AppContext& ctx)
{
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            ctx.board[y][x] = 0;

    ctx.currentPlayer = 1;
    ctx.cursorX = 0;
    ctx.cursorY = 0;
}

//  PlacePiece
bool PlacePiece(AppContext& ctx, int x, int y)
{
    if (x < 0 || x >= BOARD_SIZE) return false;
    if (y < 0 || y >= BOARD_SIZE) return false;
    if (ctx.board[y][x] != 0)     return false;

    ctx.board[y][x] = ctx.currentPlayer;
    ctx.currentPlayer = (ctx.currentPlayer == 1) ? 2 : 1;
    return true;
}

//  DrawBoard
void DrawBoard(int startX, int startY,
    const int board[BOARD_SIZE][BOARD_SIZE],
    Texture2D tileLight,
    Texture2D tileDark,
    Texture2D spriteX,
    Texture2D spriteO,
    int curX, int curY)
{
    const int cellSize = 55;

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            int       drawX = startX + x * cellSize;
            int       drawY = startY + y * cellSize;
            bool      light = (x + y) % 2 == 0;
            Texture2D tile = light ? tileLight : tileDark;
            float     scale = (float)cellSize / tile.width;
            DrawTextureEx(tile, Vector2{ (float)drawX, (float)drawY }, 0, scale, WHITE);
        }
    }

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (board[y][x] == 0) continue;
            int       drawX = startX + x * cellSize;
            int       drawY = startY + y * cellSize;
            Texture2D sprite = (board[y][x] == 1) ? spriteX : spriteO;
            float     scale = (float)cellSize / sprite.width;
            DrawTextureEx(sprite, Vector2{ (float)drawX, (float)drawY }, 0, scale, WHITE);
        }
    }

    float blink = (sinf((float)GetTime() * 5.0f) + 1.0f) / 2.0f;
    if (blink > 0.4f)
    {
        int mx = startX + curX * cellSize + cellSize / 2;
        int my = startY + curY * cellSize + cellSize / 2;
        DrawRectangle(mx - 14, my + 14, 28, 6, BLACK);
    }
}