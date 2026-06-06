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

    // --- Highlight đường thắng ---
    if (gs.winLineCount > 0 && (gs.status == GameStatus::WIN_X || gs.status == GameStatus::WIN_O))
    {
        float t = (float)GetTime();
        float pulse = (sinf(t * 6.0f) + 1.0f) / 2.0f;  // nhấp nháy nhanh

        for (int i = 0; i < gs.winLineCount; i++)
        {
            int drawX = startX + gs.winLine[i].column * CELL_SIZE;
            int drawY = startY + gs.winLine[i].row * CELL_SIZE;

            // Lớp highlight vàng chớp nháy
            unsigned char alpha1 = (unsigned char)(100 + pulse * 120);
            DrawRectangle(drawX, drawY, CELL_SIZE, CELL_SIZE, { 255, 215, 0, alpha1 });

            // Viền sáng hơn
            unsigned char alpha2 = (unsigned char)(160 + pulse * 95);
            DrawRectangleLinesEx(
                { (float)drawX, (float)drawY, (float)CELL_SIZE, (float)CELL_SIZE },
                2, { 255, 240, 80, alpha2 });
        }

        // Vẽ lại quân cờ trên ô thắng để quân cờ nổi trên highlight
        for (int i = 0; i < gs.winLineCount; i++)
        {
            int r = gs.winLine[i].row;
            int c = gs.winLine[i].column;
            Player cell = gs.board.cell[r * BOARD_SIZE + c];
            if (cell == Player::NONE) continue;

            int       drawX = startX + c * CELL_SIZE;
            int       drawY = startY + r * CELL_SIZE;
            Texture2D sprite = (cell == Player::PlayerX) ? spriteX : spriteO;
            float     scale = (float)CELL_SIZE / sprite.width;
            DrawTextureEx(sprite, Vector2{ (float)drawX, (float)drawY }, 0, scale, WHITE);
        }
    }

    // --- Cursor nhấp nháy ---
    float blink = (sinf((float)GetTime() * 5.0f) + 1.0f) / 2.0f;
    if (blink > 0.4f)
    {
        int mx = startX + curX * CELL_SIZE + CELL_SIZE / 2;
        int my = startY + curY * CELL_SIZE + CELL_SIZE / 2;
        DrawRectangle(mx - 10, my + 10, 20, 4, BLACK);
    }
}