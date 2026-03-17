#include "States.h"
#include "UI.h"
#include "TextRenderer.h"

//  Reset toàn bộ game state + UI setup trước khi vào PLAYER_SETUP
static void StartSetup(AppContext& ctx, bool withBot)
{
    ctx.playWithBot = withBot;
    ctx.gameState = GameState();   // reset board, currentPlayer, status
    ctx.player1.name.clear();
    ctx.player2.name.clear();
    ctx.currentInput.clear();
    ctx.setupStage = 0;
    ctx.selectedChar = 0;
    ctx.cursorX = BOARD_SIZE / 2;
    ctx.cursorY = BOARD_SIZE / 2;
    ctx.prevScreen = SCREEN_MODE_SELECTION;
    ctx.screen = SCREEN_PLAYER_SETUP;
}

//  UpdateModeSelection
void UpdateModeSelection(AppContext& ctx)
{
    bool keyLeft = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
    bool keyRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);

    if (keyLeft || keyRight)
        ctx.selectedMode = !ctx.selectedMode; 

    if (IsKeyPressed(KEY_ENTER))
        StartSetup(ctx, ctx.selectedMode == 1);

    if (IsKeyPressed(KEY_ESCAPE))
        ctx.screen = SCREEN_MENU;

    Vector2   mouse = GetMousePosition();
    Rectangle rect = { 30, 30, 180, 60 };
    if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        ctx.screen = SCREEN_MENU;
}

//  DrawModeSelection
void DrawModeSelection(const AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.originBg);

    int dialogX = GetScreenWidth() / 2 - tex.dialogModeGame.width / 2;
    int dialogY = GetScreenHeight() / 2 - tex.dialogModeGame.height / 2;
    DrawTexture(tex.dialogModeGame, dialogX, dialogY, WHITE);

    DrawPixelTextStyled("MODE GAME", dialogX + 610, dialogY + 170, 6);

    Color pvpColor = (ctx.selectedMode == 0) ? YELLOW : WHITE;
    Color pveColor = (ctx.selectedMode == 1) ? YELLOW : WHITE;
    DrawPixelText("PVP", dialogX + 475, dialogY + 700, 6, pvpColor);
    DrawPixelText("PVE", dialogX + 965, dialogY + 700, 6, pveColor);

    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}