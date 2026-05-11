#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"

//  Reset toàn bộ game state + UI setup trước khi vào PLAYER_SETUP
static void StartSetup(AppContext& ctx, bool withBot)
{
    ctx.playWithBot = withBot;
    ctx.gameState = GameState();
    ctx.player1.name.clear();
    ctx.player2.name.clear();
    ctx.player1.moveCount = 0;
    ctx.player2.moveCount = 0;
    ctx.currentInput.clear();
    ctx.setupStage = 0;
    ctx.selectedChar = 0;
    ctx.cursorX = BOARD_SIZE / 2;
    ctx.cursorY = BOARD_SIZE / 2;
    ctx.prevScreen = SCREEN_MODE_SELECTION;

    if (withBot)
        ctx.screen = SCREEN_DIFFICULTY;   // PVE: chọn độ khó trước
    else
        ctx.screen = SCREEN_PLAYER_SETUP; // PVP: vào setup luôn
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
    Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
    if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        ctx.screen = SCREEN_MENU;
}

//  DrawModeSelection
void DrawModeSelection(AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.originBg);

    int dialogX = GetScreenWidth() / 2 - tex.dialogModeGame.width / 2;
    int dialogY = GetScreenHeight() / 2 - tex.dialogModeGame.height / 2;
    DrawTexture(tex.dialogModeGame, dialogX, dialogY, WHITE);

<<<<<<< HEAD
    DrawPixelTextStyled("MODE GAME", dialogX + 610, dialogY + 170, 6);
=======
    DrawPixelTextStyled(getText("Mode.mode_game",*ctx.curLanguage), dialogX + 630, dialogY + 165, 5);
>>>>>>> 426b846 (Adjust UI location)

    Color pvpColor = (ctx.selectedMode == 0) ? YELLOW : WHITE;
    Color pveColor = (ctx.selectedMode == 1) ? YELLOW : WHITE;
    DrawPixelText(getText("Mode.pvp",*ctx.curLanguage), dialogX + 475, dialogY + 700, 6, pvpColor);
    DrawPixelText(getText("Mode.pve",*ctx.curLanguage), dialogX + 965, dialogY + 700, 6, pveColor);

    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}