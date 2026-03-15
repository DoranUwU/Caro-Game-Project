#include "../libs/states.h"
#include "../libs/ui.h"
#include "../libs/board.h"
#include "../libs/text_renderer.h"
#include <math.h>
#include <cstring>

//  UpdatePlayerSetup
void UpdatePlayerSetup(AppContext& ctx)
{
    bool keyLeft = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
    bool keyRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);

    // --- Back / ESC ---
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (ctx.setupStage == 0)                    ctx.state = MODE_SELECTION;
        else if (ctx.setupStage == 1) { ctx.setupStage = 0; ctx.currentInput.clear(); }
        else if (ctx.setupStage == 2) { ctx.setupStage = 1; ctx.currentInput.clear(); }
        else if (ctx.setupStage == 3) { ctx.setupStage = 2; ctx.currentInput.clear(); }
        return;
    }

    // Back button
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { 30, 30, 180, 60 };
        if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (ctx.setupStage == 0)                    ctx.state = MODE_SELECTION;
            else if (ctx.setupStage == 1) { ctx.setupStage = 0; ctx.currentInput.clear(); }
            else if (ctx.setupStage == 2) { ctx.setupStage = 1; ctx.currentInput.clear(); }
            else if (ctx.setupStage == 3) { ctx.setupStage = 2; ctx.currentInput.clear(); }
            return;
        }
    }

    // --- Nhập tên (stage 0, 2) ---
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
            if (ctx.setupStage == 0)
            {
                ctx.player1.name = ctx.currentInput.empty() ? "PLAYER 1" : ctx.currentInput;
                ctx.currentInput.clear();
                ctx.setupStage = 1;
            }
            else
            {
                ctx.player2.name = ctx.currentInput.empty() ? "PLAYER 2" : ctx.currentInput;
                ctx.currentInput.clear();
                ctx.setupStage = 3;
            }
        }
    }

    // --- Chọn nhân vật (stage 1, 3) ---
    else if (ctx.setupStage == 1 || ctx.setupStage == 3)
    {
        if (keyRight) ctx.selectedChar++;
        if (keyLeft)  ctx.selectedChar--;
        if (ctx.selectedChar < 0) ctx.selectedChar = 2;
        if (ctx.selectedChar > 2) ctx.selectedChar = 0;

        if (IsKeyPressed(KEY_ENTER))
        {
            if (ctx.setupStage == 1)
            {
                ctx.player1.character = ctx.selectedChar;
                ctx.selectedChar = 0;

                if (ctx.playWithBot)
                {
                    ctx.player2.name = "BOT";
                    ctx.player2.character = 0;
                    InitBoard(ctx);
                    ctx.prevState = PLAYER_SETUP;
                    ctx.state = GAMEPLAY;
                }
                else
                {
                    ctx.setupStage = 2;
                }
            }
            else    // stage 3
            {
                ctx.player2.character = ctx.selectedChar;
                InitBoard(ctx);
                ctx.prevState = PLAYER_SETUP;
                ctx.state = GAMEPLAY;
            }
        }
    }
}

//  DrawPlayerSetup
void DrawPlayerSetup(const AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.originBg);

    int dialogX = GetScreenWidth() / 2 - tex.dialogInputName.width / 2;
    int dialogY = GetScreenHeight() / 2 - tex.dialogInputName.height / 2;

    // ---- Stage nhập tên ----
    if (ctx.setupStage == 0 || ctx.setupStage == 2)
    {
        DrawTexture(tex.dialogInputName, dialogX, dialogY, WHITE);
        DrawPixelText("OK", dialogX + 730, dialogY + 610, 5, WHITE);

        const char* label = (ctx.setupStage == 0) ? "PLAYER 1 NAME" : "PLAYER 2 NAME";
        DrawPixelText(label, dialogX + 580, dialogY + 310, 5, WHITE);
        DrawPixelText(ctx.currentInput.c_str(), dialogX + 470, dialogY + 470, 6, BLACK);

        // Con trỏ nhấp nháy dọc
        float blink = (sinf((float)GetTime() * 5.0f) + 1.0f) / 2.0f;
        if (blink > 0.4f)
        {
            const int scale = 6;
            const int glyphW = 5;
            const int spacingCols = 1;
            int charAdvance = (glyphW + spacingCols) * scale;
            int textW = (int)ctx.currentInput.size() * charAdvance;
            int curX = dialogX + 470 + textW;
            int curY = dialogY + 470;
            DrawRectangle(curX, curY, scale, 7 * scale, BLACK);
        }
    }

    // ---- Stage chọn nhân vật ----
    if (ctx.setupStage == 1 || ctx.setupStage == 3)
    {
        // Dialog background (scale 0.7)
        {
            float     scale = 0.7f;
            Rectangle src = { 0, 0,
                                 (float)tex.dialogChooseChar.width,
                                 (float)tex.dialogChooseChar.height };
            Rectangle dst = { GetScreenWidth() / 2.0f, 200,
                                 tex.dialogChooseChar.width * scale,
                                 tex.dialogChooseChar.height * scale };
            Vector2   origin = { dst.width / 2.0f, dst.height / 2.0f };
            DrawTexturePro(tex.dialogChooseChar, src, dst, origin, 0.0f, WHITE);
        }

        // Tiêu đề
        const char* titleStr = "CHOOSE CHARACTER";
        int titleScale = 5;
        int cdx = GetScreenWidth() / 2 - tex.dialogChooseChar.width / 2;
        int cdy = GetScreenHeight() / 2 - tex.dialogChooseChar.height / 2;
        DrawPixelTextStyled(titleStr, cdx + 540, 200, titleScale);

        // 3 frame nhân vật
        Texture2D charSprites[3] = { tex.spriteKnight, tex.spriteMage, tex.spriteArcher };
        const char* charNames[3] = { "KNIGHT", "MAGE", "ARCHER" };

        const int frameSize = 450;
        const int spacing = 40;
        int       frameStartX = cdx + 100;
        int       frameY = cdy + 400;

        for (int i = 0; i < 3; i++)
        {
            int  fx = frameStartX + i * (frameSize + spacing);
            int  fy = frameY;
            bool isSelected = (i == ctx.selectedChar);

            // Frame nền
            float frameScale = (float)frameSize / tex.dialogCharFrame.width;
            Color frameColor = isSelected
                ? Color{ 255, 220, 80, 255 }
            : Color{ 180, 180, 180, 200 };
            DrawTextureEx(tex.dialogCharFrame,
                Vector2{ (float)fx, (float)fy },
                0, frameScale, frameColor);

            // Sprite nhân vật
            float sprScale = (float)(frameSize - 20) / charSprites[i].width;
            DrawTextureEx(charSprites[i],
                Vector2{ (float)(fx + 10), (float)(fy + 10) },
                0, sprScale, WHITE);

            // Tên
            Color nameColor = isSelected ? YELLOW : GRAY;
            int   nameScale = 4;
            int   nameW = (int)(strlen(charNames[i]) * 8 * nameScale);
            DrawPixelText(charNames[i],
                fx + frameSize / 2 - nameW / 4,
                fy + frameSize + 8,
                nameScale, nameColor);

            // Viền glow khi chọn
            if (isSelected)
            {
                float glow = (sinf((float)GetTime() * 4.0f) + 1.0f) / 2.0f;
                DrawRectangleLinesEx(
                    Rectangle{ (float)(fx - 3), (float)(fy - 3),
                                (float)(frameSize + 6), (float)(frameSize + 6) },
                    3,
                    Color{ 255, 220, 80, (unsigned char)(180 + glow * 75) }
                );
            }
        }
    }

    // Nút Back (luôn hiện)
    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}