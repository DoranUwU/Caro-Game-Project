#include "Board.h"      
#include "GameState.h" 
#include <math.h>

//  DrawBoard
void DrawBoard(int startX, int startY,
    const GameState& gs,
    Texture2D tileLight,
    Texture2D tileDark,
    Texture2D spriteX,
    Texture2D spriteO,
    int curX, int curY)
{

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            int       drawX = startX + col * CELL_SIZE;
            int       drawY = startY + row * CELL_SIZE;
            bool      light = (row + col) % 2 == 0;
            Texture2D tile = light ? tileLight : tileDark;
            float     scale = (float)CELL_SIZE / tile.width;
            DrawTextureEx(tile, Vector2{ (float)drawX, (float)drawY }, 0, scale, WHITE);
        }
    }

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            Player cell = gs.board.cell[row * BOARD_SIZE + col];
            if (cell == Player::NONE) continue;

            int       drawX = startX + col * CELL_SIZE;
            int       drawY = startY + row * CELL_SIZE;
            Texture2D sprite = (cell == Player::PlayerX) ? spriteX : spriteO;
            float     scale = (float)CELL_SIZE / sprite.width;
            DrawTextureEx(sprite, Vector2{ (float)drawX, (float)drawY }, 0, scale, WHITE);
        }
    }

    float blink = (sinf((float)GetTime() * 5.0f) + 1.0f) / 2.0f;
    if (blink > 0.4f)
    {
        int mx = startX + curX * CELL_SIZE + CELL_SIZE / 2;
        int my = startY + curY * CELL_SIZE + CELL_SIZE / 2;
        DrawRectangle(mx - 10, my + 10, 20, 4, BLACK);
    }
}