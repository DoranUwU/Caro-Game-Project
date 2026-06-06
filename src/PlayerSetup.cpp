#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "GameState.h"
#include <math.h>
#include <cstring>
#include <vector>

void UpdatePlayerSetup(AppContext& ctx)
{
    bool keyLeft = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
    bool keyRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);

    // ESC / Back
    auto goBack = [&]() {
        if (ctx.setupStage == 0) ctx.screen = SCREEN_MODE_SELECTION;
        else if (ctx.setupStage == 1) { ctx.setupStage = 0; ctx.currentInput.clear(); }
        else if (ctx.setupStage == 2) { ctx.setupStage = 1; ctx.currentInput.clear(); }
        else if (ctx.setupStage == 3) { ctx.setupStage = 2; ctx.currentInput.clear(); }
        };

    if (IsKeyPressed(KEY_ESCAPE)) { goBack(); return; }

    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
        if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            goBack(); return;
        }
    }

    // Stage nhập tên
    if (ctx.setupStage == 0 || ctx.setupStage == 2)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 125 && ctx.currentInput.size() < 16)
                ctx.currentInput.push_back((char)key);
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !ctx.currentInput.empty())
            ctx.currentInput.pop_back();

        if (IsKeyPressed(KEY_ENTER))
        {
            PlaySfx(ctx.sound->clickSfx);
            if (ctx.setupStage == 0)
            {
                ctx.player1.name = ctx.currentInput.empty() ? getText("Gameplay.player_1", *ctx.curLanguage) : ctx.currentInput;
                ctx.currentInput.clear();
                ctx.setupStage = 1;
            }
            else
            {
                ctx.player2.name = ctx.currentInput.empty() ? getText("Gameplay.player_2", *ctx.curLanguage) : ctx.currentInput;
                ctx.currentInput.clear();
                ctx.setupStage = 3;
            }
        }
    }

    // Stage chọn nhân vật — keyboard
    else if (ctx.setupStage == 1 || ctx.setupStage == 3)
    {
        if (keyRight) ctx.selectedChar++;
        if (keyLeft)  ctx.selectedChar--;
        if (ctx.selectedChar < 0) ctx.selectedChar = 2;
        if (ctx.selectedChar > 2) ctx.selectedChar = 0;

        if (IsKeyPressed(KEY_ENTER))
        {
            ctx.hasPlayedWinsfx = false;
            PlaySfx(ctx.sound->clickSfx);
            if (ctx.setupStage == 1)
            {
                ctx.player1.character = ctx.selectedChar;
                ctx.selectedChar = 0;
                if (ctx.playWithBot)
                {
                    ctx.player2.name = getText("PlayerSetup.bot", *ctx.curLanguage);
                    ctx.player2.character = 0;
                    ctx.gameState = GameState();
                    ctx.cursorX = ctx.cursorY = BOARD_SIZE / 2;
                    ctx.turnTimer = TURN_TIME;
                    ctx.prevScreen = SCREEN_PLAYER_SETUP;
                    ctx.screen = SCREEN_GAMEPLAY;
                    ctx.enterGuard = true;
                }
                else ctx.setupStage = 2;
            }
            else
            {
                ctx.player2.character = ctx.selectedChar;
                ctx.gameState = GameState();
                ctx.cursorX = ctx.cursorY = BOARD_SIZE / 2;
                ctx.turnTimer = TURN_TIME;
                ctx.prevScreen = SCREEN_PLAYER_SETUP;
                ctx.screen = SCREEN_GAMEPLAY;
                ctx.enterGuard = true;
            }
        }
    }
}

// Helper: confirm chọn nhân vật 
static void ConfirmCharacter(AppContext& ctx, int charIdx)
{
    ctx.selectedChar = charIdx;
    if (ctx.setupStage == 1)
    {
        ctx.player1.character = charIdx;
        ctx.selectedChar = 0;
        if (ctx.playWithBot)
        {
            ctx.player2.name = getText("PlayerSetup.bot", *ctx.curLanguage);
            ctx.player2.character = 0;
            ctx.gameState = GameState();
            ctx.cursorX = ctx.cursorY = BOARD_SIZE / 2;
            ctx.turnTimer = TURN_TIME;
            ctx.prevScreen = SCREEN_PLAYER_SETUP;
            ctx.screen = SCREEN_GAMEPLAY;
            ctx.enterGuard = true;
        }
        else ctx.setupStage = 2;
    }
    else
    {
        ctx.player2.character = charIdx;
        ctx.gameState = GameState();
        ctx.cursorX = ctx.cursorY = BOARD_SIZE / 2;
        ctx.turnTimer = TURN_TIME;
        ctx.prevScreen = SCREEN_PLAYER_SETUP;
        ctx.screen = SCREEN_GAMEPLAY;
        ctx.enterGuard = true;
    }
}

void DrawPlayerSetup(AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.originBg);

    if (ctx.setupStage == 0 || ctx.setupStage == 2)
    {
        DrawTexture(tex.dialogInputName, 192, 28, WHITE);
        DrawPixelText(getText("PlayerSetup.ok", *ctx.curLanguage), 922, 638, 5, WHITE);

        const char* label = (ctx.setupStage == 0)
            ? getText("PlayerSetup.player_1_name", *ctx.curLanguage)
            : getText("PlayerSetup.player_2_name", *ctx.curLanguage);
        DrawPixelText(label, 772, 338, 5, WHITE);
        DrawPixelText(ctx.currentInput.c_str(), 662, 498, 6, BLACK);

        float blink = (sinf((float)GetTime() * 5.0f) + 1.0f) / 2.0f;
        if (blink > 0.4f)
        {
            int charAdvance = (5 + 1) * 6;
            int textW = (int)ctx.currentInput.size() * charAdvance;
            DrawRectangle(662 + textW, 498, 6, 7 * 6, BLACK);
        }
    }

    if (ctx.setupStage == 1 || ctx.setupStage == 3)
    {
        float scale = 0.7f;
        Rectangle src = { 0, 0, (float)tex.dialogChooseChar.width, (float)tex.dialogChooseChar.height };
        Rectangle dst = { 960.0f, 200,
                          tex.dialogChooseChar.width * scale, tex.dialogChooseChar.height * scale };
        Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };
        DrawTexturePro(tex.dialogChooseChar, src, dst, origin, 0.0f, WHITE);

        DrawPixelTextStyled(getText("PlayerSetup.choose_character", *ctx.curLanguage), 730, 200, 5);

        Texture2D   charSprites[3] = { tex.spriteKnight_L, tex.spriteMage_L, tex.spriteArcher_L };
        std::vector<const char*> charNames = {
            getText("PlayerSetup.knight", *ctx.curLanguage),
            getText("PlayerSetup.mage", *ctx.curLanguage),
            getText("PlayerSetup.archer", *ctx.curLanguage)
        };
        const int   frameSize = 450, spacing = 40;
        int         frameStartX = 292, frameY = 428;

        Vector2 mouse = GetMousePosition();

        for (int i = 0; i < 3; i++)
        {
            int fx = frameStartX + i * (frameSize + spacing), fy = frameY;
            Rectangle frameRect = { (float)fx, (float)fy, (float)frameSize, (float)frameSize };

            // Hover chuột
            if (CheckCollisionPointRec(mouse, frameRect))
                ctx.selectedChar = i;

            // Click chuột
            if (CheckCollisionPointRec(mouse, frameRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                ConfirmCharacter(ctx, i);
                break; 
            }

            bool sel = (i == ctx.selectedChar);

            DrawTextureEx(tex.dialogCharFrame, Vector2{ (float)fx, (float)fy },
                0, (float)frameSize / tex.dialogCharFrame.width,
                sel ? Color{ 255,220,80,255 } : Color{ 180,180,180,200 });

            DrawTextureEx(charSprites[i], Vector2{ (float)(fx + 10), (float)(fy + 10) },
                0, (float)(frameSize - 20) / charSprites[i].width, WHITE);

            int nameW = (int)(GetUTF8Length(charNames[i]) * 6 * 4);
            DrawPixelText(charNames[i], fx + frameSize / 2 - nameW / 2, fy + frameSize + 8,
                4, sel ? YELLOW : GRAY);

            if (sel)
            {
                float glow = (sinf((float)GetTime() * 4.0f) + 1.0f) / 2.0f;
                DrawRectangleLinesEx({ (float)(fx - 3),(float)(fy - 3),
                                       (float)(frameSize + 6),(float)(frameSize + 6) },
                    3, { 255,220,80,(unsigned char)(180 + glow * 75) });
            }
        }
    }

    bool h, c;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, h, c);
}