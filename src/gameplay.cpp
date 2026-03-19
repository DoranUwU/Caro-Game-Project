#include "States.h"
#include "UI.h"
#include "Board.h"     
#include "TextRenderer.h"
#include "Save.h"
#include "Load.h"
#include "libs.h"

static const char* SAVE_FILE = "saves/current_game.json";

static void GetBoardOrigin(int& startX, int& startY)
{
    int boardPx = BOARD_SIZE * CELL_SIZE;
    startX = (GetScreenWidth() - boardPx) / 2;
    startY = (GetScreenHeight() - boardPx) / 2;
}

//  UpdateGameplay
void UpdateGameplay(AppContext& ctx)
{
    // Game kết thúc — nhấn ENTER để về menu
    if (ctx.gameState.status != GameStatus::ONGOING)
    {
        if (IsKeyPressed(KEY_ENTER))
            ctx.screen = SCREEN_MENU;
        return;
    }

    int startX, startY;
    GetBoardOrigin(startX, startY);

    // Di chuyển cursor bằng phím
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) { ctx.cursorY--; if (ctx.cursorY < 0)          ctx.cursorY = BOARD_SIZE - 1; }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) { ctx.cursorY++; if (ctx.cursorY >= BOARD_SIZE) ctx.cursorY = 0; }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) { ctx.cursorX--; if (ctx.cursorX < 0)          ctx.cursorX = BOARD_SIZE - 1; }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) { ctx.cursorX++; if (ctx.cursorX >= BOARD_SIZE) ctx.cursorX = 0; }

    // Đánh bằng phím Enter/Space — Position(row, col)
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        ctx.gameState = playMove(ctx.gameState, Position(ctx.cursorY, ctx.cursorX));

    // Cập nhật cursor theo chuột
    {
        Vector2 mouse = GetMousePosition();
        int mc = (int)(mouse.x - startX) / CELL_SIZE;
        int mr = (int)(mouse.y - startY) / CELL_SIZE;
        if (mc >= 0 && mc < BOARD_SIZE && mr >= 0 && mr < BOARD_SIZE)
        {
            ctx.cursorX = mc;
            ctx.cursorY = mr;
        }
    }

    // Đánh bằng chuột
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        int mc = (int)(mouse.x - startX) / CELL_SIZE;
        int mr = (int)(mouse.y - startY) / CELL_SIZE;
        if (mc >= 0 && mc < BOARD_SIZE && mr >= 0 && mr < BOARD_SIZE)
            ctx.gameState = playMove(ctx.gameState, Position(mr, mc));
    }

    // L = save nhanh, T = load nhanh
    if (IsKeyPressed(KEY_L))
        saveGameState(ctx.gameState, SAVE_FILE);

    if (IsKeyPressed(KEY_T))
    {
        GameState loaded;
        if (loadGameState(SAVE_FILE, loaded))
            ctx.gameState = loaded;
    }

    // Nút Setting góc phải
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { SCREEN_W - BTN_SETTINGS_OFFSET, BTN_BACK_Y, BTN_MAX_W, BTN_BACK_H };
        if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ctx.prevScreen = SCREEN_GAMEPLAY;
            ctx.screen = SCREEN_SETTINGS;
        }
    }
}

// ============================================================
//  DrawGameplay
// ============================================================
void DrawGameplay(const AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.gameplayBg);

    int startX, startY;
    GetBoardOrigin(startX, startY);

    DrawBoard(startX, startY + 20,
        ctx.gameState,
        tex.tileLight, tex.tileDark,
        tex.spriteX, tex.spriteO,
        ctx.cursorX, ctx.cursorY);

    // Tên người chơi
    const std::string& left = ctx.player1.name.empty() ? "Player 1" : ctx.player1.name;
    const std::string& right = ctx.player2.name.empty()
        ? (ctx.playWithBot ? "Bot" : "Player 2")
        : ctx.player2.name;

    DrawPixelText(left.c_str(), HUD_PLAYER1_X, HUD_Y, FONT_SCALE_MD, WHITE);
    DrawPixelText(right.c_str(), HUD_PLAYER2_X, HUD_Y, FONT_SCALE_MD, WHITE);

    // Lượt đang đi
   /* const char* turnStr = (ctx.gameState.currentPlayer == Player::PlayerX)
        ? "X's turn" : "O's turn";
    DrawPixelText(turnStr, GetScreenWidth() / 2 - 60, 68, 4, GOLD);*/

    // Màn hình kết thúc game
    if (ctx.gameState.status != GameStatus::ONGOING)
    {
        const char* msg = "";
        if (ctx.gameState.status == GameStatus::WIN_X) msg = "X WINS!";
        else if (ctx.gameState.status == GameStatus::WIN_O) msg = "O WINS!";
        else if (ctx.gameState.status == GameStatus::DRAW)  msg = "DRAW!";
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 160 });

        int tw = (int)(strlen(msg) * 8 * 8); DrawPixelText(msg, GetScreenWidth() / 2 - 150,  150, 8, GOLD);

        DrawPixelText("Press ENTER to return",
            GetScreenWidth() / 2 - 250,
            GetScreenHeight() / 2 + 400,
            4, WHITE);
    }

    // Nút Setting
    bool hovered, clicked;
    DrawSettingButton(tex.buttonSettingNormal, tex.buttonSettingHover,
        GetScreenWidth(), hovered, clicked);

    DrawPixelText("L: Save   T: Load", 50, GetScreenHeight() - 100, 2, GRAY);
}