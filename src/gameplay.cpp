#include "../libs/states.h"
#include "../libs/ui.h"
#include "../libs/board.h"
#include "../libs/text_renderer.h"

static const int CELL_SIZE = 55;

// Tính toạ độ góc trên-trái của bàn cờ (căn giữa màn hình)
static void GetBoardOrigin(int& startX, int& startY)
{
    int boardPx = BOARD_SIZE * CELL_SIZE;
    startX = (GetScreenWidth() - boardPx) / 2;
    startY = (GetScreenHeight() - boardPx) / 2;
}

//  UpdateGameplay
void UpdateGameplay(AppContext& ctx)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ctx.state = MENU;
        return;
    }

    int startX, startY;
    GetBoardOrigin(startX, startY);

    // Di chuyển cursor bằng phím
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) { ctx.cursorY--; if (ctx.cursorY < 0)           ctx.cursorY = BOARD_SIZE - 1; }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) { ctx.cursorY++; if (ctx.cursorY >= BOARD_SIZE)  ctx.cursorY = 0; }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) { ctx.cursorX--; if (ctx.cursorX < 0)           ctx.cursorX = BOARD_SIZE - 1; }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) { ctx.cursorX++; if (ctx.cursorX >= BOARD_SIZE)  ctx.cursorX = 0; }

    // Đánh bằng phím
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        PlacePiece(ctx, ctx.cursorX, ctx.cursorY);

    // Cập nhật cursor theo chuột
    {
        Vector2 mouse = GetMousePosition();
        int mx = (int)(mouse.x - startX) / CELL_SIZE;
        int my = (int)(mouse.y - startY) / CELL_SIZE;
        if (mx >= 0 && mx < BOARD_SIZE && my >= 0 && my < BOARD_SIZE)
        {
            ctx.cursorX = mx;
            ctx.cursorY = my;
        }
    }

    // Đánh bằng chuột
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        int x = (int)(mouse.x - startX) / CELL_SIZE;
        int y = (int)(mouse.y - startY) / CELL_SIZE;
        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
            PlacePiece(ctx, x, y);
    }

    // Nút Setting 
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { (float)(GetScreenWidth() - 210), 30, 180, 60 };
        if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ctx.prevState = GAMEPLAY;
            ctx.state = SETTINGS;
        }
    }
}

//  DrawGameplay
void DrawGameplay(const AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.gameplayBg);

    int startX, startY;
    GetBoardOrigin(startX, startY);

    DrawBoard(startX, startY,
        ctx.board,
        tex.tileLight, tex.tileDark,
        tex.spriteX, tex.spriteO,
        ctx.cursorX, ctx.cursorY);

    // Tên người chơi
    const std::string& left = ctx.player1.name.empty() ? "Player 1" : ctx.player1.name;
    const std::string& right = ctx.player2.name.empty()
        ? (ctx.playWithBot ? "Bot" : "Player 2")
        : ctx.player2.name;

    DrawPixelText(left.c_str(), 350, 68, 4, WHITE);
    DrawPixelText(right.c_str(), 860 + 240, 68, 4, WHITE);

    // Nút Setting
    bool hovered, clicked;
    DrawSettingButton(tex.buttonSettingNormal, tex.buttonSettingHover,
        GetScreenWidth(), hovered, clicked);
}