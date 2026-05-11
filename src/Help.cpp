#include "Overlay.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <math.h>
#include <vector>
#include <cstring>

static void DrawHelpTitle(const char* title, float glow)
{
    Color titleCol = { 255, 220, 100, (unsigned char)(200 + glow * 55) };
    int tW = (int)(strlen(title) * 8 * FONT_SCALE_XL);
    int tX = GetScreenWidth() / 2 - tW / 2;
    DrawPixelText(title, 854, PANEL_TITLE_Y + 4 - 32, FONT_SCALE_XL, { 0, 0, 0, 160 });
    DrawPixelText(title, 850, PANEL_TITLE_Y - 32, FONT_SCALE_XL, titleCol);

    // Đường kẻ trang trí
    int lineY = PANEL_TITLE_Y + FONT_SCALE_XL * 9 + 10;
    int lineLen = 400;
    int lineX = GetScreenWidth() / 2 - lineLen / 2;
    unsigned char lineA = (unsigned char)(120 + glow * 80);
    DrawRectangle(lineX, lineY - 32, lineLen, 2, { 255, 220, 100, lineA });
    DrawRectangle(lineX + 20, lineY - 32 + 6, lineLen - 40, 1, { 255, 200, 80, (unsigned char)(lineA / 2) });
}

void DrawHelpOverlay(const AppContext& ctx)
{
    float t = (float)GetTime();
    float glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;

    DrawHelpTitle(getText("Help.title", *ctx.curLanguage), glow);

    Rectangle panel = GetOverlayPanelRect(0.66f, 0.72f);
    DrawClassicPanel(panel, glow);

    float cx = panel.x + panel.width / 2.0f;
    float curY = panel.y + 40.0f;
    float colW = panel.width / 2.0f - 30.0f;
    float lx = panel.x + 40.0f;
    float rx = panel.x + panel.width / 2.0f + 20.0f;

    // ---- ABOUT THE GAME ----
    DrawPixelText("ABOUT THE GAME", (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    const char* desc[] = {
        "Caro (Gomoku) is a strategy",
        "board game for 2 players.",
        "Place 5 pieces in a row to win",
        "on a 15x15 grid. Diagonals,",
        "horizontal and vertical all count!"
    };
    for (auto& line : desc)
    {
        DrawPixelText(line, (int)lx, (int)curY, 2, { 210, 200, 170, 220 });
        curY += 22;
    }

    curY += 14;

    // ---- HOW TO PLAY ----
    DrawPixelText("HOW TO PLAY", (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    struct Row { const char* key; const char* desc; };
    Row moves[] = {
        { "W / UP",    "Move cursor up"       },
        { "S / DOWN",  "Move cursor down"     },
        { "A / LEFT",  "Move cursor left"     },
        { "D / RIGHT", "Move cursor right"    },
        { "ENTER",     "Place your piece"     },
        { "SPACE",     "Place your piece"     },
        { "MOUSE",     "Click to place piece" },
    };

    float keyX = lx;
    float descX = lx + 200;
    for (auto& row : moves)
    {
        DrawPixelText(row.key, (int)keyX, (int)curY, 2, { 255, 220, 100, 230 });
        DrawPixelText(row.desc, (int)descX, (int)curY, 2, { 200, 190, 160, 210 });
        curY += 22;
    }

    curY += 14;

    // ---- SAVE / LOAD ----
    DrawPixelText("SAVE  &  LOAD", (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    Row saves[] = {
        { "L", "Save current game"  },
        { "T", "Load saved game"    },
    };
    for (auto& row : saves)
    {
        DrawPixelText(row.key, (int)keyX, (int)curY, 2, { 255, 220, 100, 230 });
        DrawPixelText(row.desc, (int)descX, (int)curY, 2, { 200, 190, 160, 210 });
        curY += 22;
    }

    curY += 14;

    // ---- TIMER ----
    DrawPixelText("TIMER", (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    const char* timerDesc[] = {
        "Each player has 30 seconds",
        "per turn. Time runs out?",
        "The opponent wins!",
    };
    for (auto& line : timerDesc)
    {
        DrawPixelText(line, (int)lx, (int)curY, 2, { 210, 200, 170, 220 });
        curY += 22;
    }
}
