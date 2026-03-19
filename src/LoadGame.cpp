#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include "Load.h"
#include "Save.h"
#include <math.h>
#include <cstring>

static const char* SAVE_FILE = "saves/current_game.json";

static Rectangle GetPanelRect()
{
    float pw = GetScreenWidth() * PANEL_W_RATIO;
    float ph = GetScreenHeight() * PANEL_H_RATIO;
    return {
        (GetScreenWidth() - pw) / 2.0f,
        (GetScreenHeight() - ph) / 2.0f,
        pw, ph
    };
}

void UpdateLoadGame(AppContext& ctx)
{
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
        bool backClicked = CheckCollisionPointRec(mouse, rect)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (backClicked || IsKeyPressed(KEY_ESCAPE))
        {
            ctx.screen = SCREEN_MENU;
            return;
        }
    }

    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Rectangle panel = GetPanelRect();
        float     btnW = 260, btnH = 54;
        float     btnX = panel.x + (panel.width - btnW) / 2.0f;
        float     btnY = panel.y + panel.height - btnH - 60.0f;
        Rectangle btnRect = { btnX, btnY, btnW, btnH };

        Vector2 mouse = GetMousePosition();
        bool    enter = IsKeyPressed(KEY_ENTER);
        bool    click = CheckCollisionPointRec(mouse, btnRect)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (enter || click)
        {
            GameState loaded;
            if (loadGameState(SAVE_FILE, loaded))
            {
                ctx.gameState = loaded;
                ctx.cursorX = BOARD_SIZE / 2;
                ctx.cursorY = BOARD_SIZE / 2;
                ctx.prevScreen = SCREEN_LOAD_GAME;
                ctx.screen = SCREEN_GAMEPLAY;
            }
        }
    }
}

void DrawLoadGame(const AppContext& ctx, const TextureBank& tex)
{
    float t = (float)GetTime();

    DrawFullscreenTexture(tex.originBg);

    const char* title = "LOAD GAME";
    float       glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;
    Color       titleCol = { 255, 220, 100,
                             (unsigned char)(200 + glow * 55) };
    int         tW = (int)(strlen(title) * 8 *  FONT_SCALE_XL);
    int         tX = GetScreenWidth() / 2 - tW / 2;

    DrawPixelText(title, tX + 4, PANEL_TITLE_Y + 4,  FONT_SCALE_XL, { 0, 0, 0, 160 });
    DrawPixelText(title, tX, PANEL_TITLE_Y,  FONT_SCALE_XL, titleCol);

    int lineY = PANEL_TITLE_Y +  FONT_SCALE_XL * 9 + 10;
    int lineLen = 400;
    int lineX = GetScreenWidth() / 2 - lineLen / 2;
    unsigned char lineA = (unsigned char)(120 + glow * 80);
    DrawRectangle(lineX, lineY, lineLen, 2, { 255, 220, 100, lineA });
    DrawRectangle(lineX + 20, lineY + 6, lineLen - 40, 1, { 255, 200, 80, (unsigned char)(lineA / 2) });

    Rectangle panel = GetPanelRect();

    DrawRectangleRec({ panel.x + 8, panel.y + 8, panel.width, panel.height },
        { 0, 0, 0, 60 });

    DrawRectangleRec(panel, { 0, 0, 0, 180 });

    unsigned char borderA = (unsigned char)(160 + glow * 60);
    DrawRectangleLinesEx(panel, 2, { 200, 170, 80, borderA });

    DrawRectangleLinesEx(
        { panel.x + 6, panel.y + 6, panel.width - 12, panel.height - 12 },
        1, { 200, 170, 80, (unsigned char)(borderA / 2) }
    );

    auto DrawCornerDot = [](float x, float y, Color c) {
        DrawRectangle((int)x - 3, (int)y - 3, 6, 6, c);
        };
    Color cornerCol = { 255, 220, 100, borderA };
    DrawCornerDot(panel.x, panel.y, cornerCol);
    DrawCornerDot(panel.x + panel.width, panel.y, cornerCol);
    DrawCornerDot(panel.x, panel.y + panel.height, cornerCol);
    DrawCornerDot(panel.x + panel.width, panel.y + panel.height, cornerCol);

    bool   saveExists = FileExists(SAVE_FILE);
    float  cx = panel.x + panel.width / 2.0f;
    float  contenPANEL_TITLE_Y = panel.y + 60.0f;

    {
        int sx = (int)cx - 16;
        int sy = (int)contenPANEL_TITLE_Y;
        DrawRectangle(sx + 15, sy, 2, 40, { 200, 200, 210, 220 });
        DrawRectangle(sx + 6, sy + 38, 20, 4, { 180, 140, 60, 220 });
        DrawRectangle(sx + 14, sy - 6, 4, 8, { 220, 220, 230, 220 });
    }

    contenPANEL_TITLE_Y += 70.0f;

    if (saveExists)
    {
        const char* line1 = "SAVE FILE FOUND";
        int l1W = (int)(strlen(line1) * 8 * 4);
        DrawPixelText(line1, (int)(cx - l1W / 2), (int)contenPANEL_TITLE_Y, 4, { 120, 220, 120, 255 });

        contenPANEL_TITLE_Y += 55.0f;

        DrawRectangle((int)(panel.x + 40), (int)contenPANEL_TITLE_Y,
            (int)(panel.width - 80), 1,
            { 200, 170, 80, 80 });
        contenPANEL_TITLE_Y += 20.0f;

        GameState preview;
        if (loadGameState(SAVE_FILE, preview))
        {
            int countX = 0, countO = 0;
            for (int i = 0; i < TOTAL_CELLS; i++)
            {
                if (preview.board.cell[i] == Player::PlayerX) countX++;
                if (preview.board.cell[i] == Player::PlayerO) countO++;
            }
            int totalMoves = countX + countO;

            const char* statusStr = "ONGOING";
            if (preview.status == GameStatus::WIN_X) statusStr = "BLACK WINS";
            else if (preview.status == GameStatus::WIN_O) statusStr = "WHITE WINS";
            else if (preview.status == GameStatus::DRAW)  statusStr = "DRAW";

            char infoLine[64];

            snprintf(infoLine, sizeof(infoLine), "MOVES PLAYED :  %d", totalMoves);
            int iW = (int)(strlen(infoLine) * 8 * 3);
            DrawPixelText(infoLine, (int)(cx - iW / 2), (int)contenPANEL_TITLE_Y,
                3, { 200, 190, 150, 220 });
            contenPANEL_TITLE_Y += 42.0f;

            snprintf(infoLine, sizeof(infoLine), "STATUS       :  %s", statusStr);
            iW = (int)(strlen(infoLine) * 8 * 3);
            DrawPixelText(infoLine, (int)(cx - iW / 2), (int)contenPANEL_TITLE_Y,
                3, { 200, 190, 150, 220 });
            contenPANEL_TITLE_Y += 42.0f;

            const char* turnStr = (preview.currentPlayer == Player::PlayerX)
                ? "BLACK (X)" : "WHITE (O)";
            snprintf(infoLine, sizeof(infoLine), "NEXT TURN    :  %s", turnStr);
            iW = (int)(strlen(infoLine) * 8 * 3);
            DrawPixelText(infoLine, (int)(cx - iW / 2), (int)contenPANEL_TITLE_Y,
                3, { 200, 190, 150, 220 });
        }
    }
    else
    {
        const char* noSave = "NO SAVE FILE FOUND";
        int nsW = (int)(strlen(noSave) * 8 * 4);
        DrawPixelText(noSave, (int)(cx - nsW / 2), (int)contenPANEL_TITLE_Y,
            4, { 200, 80, 80, 220 });

        contenPANEL_TITLE_Y += 60.0f;
        const char* hint = "PLAY A GAME AND PRESS F5 TO SAVE";
        int hW = (int)(strlen(hint) * 8 * 3);
        DrawPixelText(hint, (int)(cx - hW / 2), (int)contenPANEL_TITLE_Y,
            3, { 150, 140, 110, 180 });
    }

    {
        float btnW = 260, btnH = 54;
        float btnX = panel.x + (panel.width - btnW) / 2.0f;
        float btnY = panel.y + panel.height - btnH - 60.0f;
        Rectangle btnRect = { btnX, btnY, btnW, btnH };

        Vector2 mouse = GetMousePosition();
        bool    btnHover = CheckCollisionPointRec(mouse, btnRect) && saveExists;

        Color btnBg = saveExists
            ? (btnHover ? Color{ 80, 70, 30, 220 } : Color{ 40, 35, 15, 200 })
            : Color{ 30, 30, 30, 120 };
        DrawRectangleRec(btnRect, btnBg);

        float btnGlow = btnHover ? (sinf(t * 6.0f) + 1.0f) / 2.0f : 0.0f;
        unsigned char ba = saveExists
            ? (unsigned char)(160 + btnGlow * 80)
            : 60;
        DrawRectangleLinesEx(btnRect, 2, { 200, 170, 80, ba });

        const char* btnLabel = saveExists ? "LOAD" : "LOAD";
        Color       btnTCol = saveExists
            ? Color{ 255, 220, 100, (unsigned char)(200 + btnGlow * 55) }
        : Color{ 100, 100, 100, 140 };
        int lblW = (int)(strlen(btnLabel) * 8 * 5);
        DrawPixelText(btnLabel,
            (int)(btnX + btnW / 2 - lblW / 2),
            (int)(btnY + btnH / 2 - 5 * 4),
            5, btnTCol);

        if (saveExists)
        {
            const char* hint = "ENTER  /  CLICK";
            int hw = (int)(strlen(hint) * 8 * 2);
            DrawPixelText(hint,
                (int)(btnX + btnW / 2 - hw / 2),
                (int)(btnY + btnH + 10),
                2, { 150, 140, 100, 140 });
        }
    }

    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}