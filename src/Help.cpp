#include "Overlay.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <math.h>
#include <vector>

void DrawHelpOverlay(const AppContext& ctx)
{
    float t = (float)GetTime();
    float glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;

    DrawOverlayTitle(getText("Help.title", *ctx.curLanguage), glow);

    Rectangle panel = GetOverlayPanelRect(0.66f, 0.72f);
    DrawClassicPanel(panel, glow);

    float cx = panel.x + panel.width / 2.0f;
    float curY = panel.y + 40.0f;
    float colW = panel.width / 2.0f - 30.0f;
    float lx = panel.x + 40.0f;
    float rx = panel.x + panel.width / 2.0f + 20.0f;

    // ---- ABOUT THE GAME ----
    DrawPixelText(getText("Help.about_the_game", *ctx.curLanguage), (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
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
    vector<string> gameInfo;
    
    gameInfo.push_back(getText("Help.gameinfo1", *ctx.curLanguage));
    gameInfo.push_back(getText("Help.gameinfo2", *ctx.curLanguage));
    gameInfo.push_back(getText("Help.gameinfo3", *ctx.curLanguage));
    gameInfo.push_back(getText("Help.gameinfo4", *ctx.curLanguage));
    gameInfo.push_back(getText("Help.gameinfo5", *ctx.curLanguage));

    for (auto& line : gameInfo)
    {
        DrawPixelText(line, (int)lx, (int)curY, 2, { 210, 200, 170, 220 });
        curY += 22;
    }

    curY += 14;

    // ---- HOW TO PLAY ----
    DrawPixelText(getText("Help.how_to_play", *ctx.curLanguage), (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    struct Row { const char* key; const char* desc; };
    Row moves[] = {
        { "W / UP",    getText("Help.move_cursor_up", *ctx.curLanguage)         },
        { "S / DOWN",  getText("Help.move_cursor_down", *ctx.curLanguage)       },
        { "A / LEFT",  getText("Help.move_cursor_left", *ctx.curLanguage)       },
        { "D / RIGHT", getText("Help.move_cursor_right", *ctx.curLanguage)      },
        { "ENTER",     getText("Help.place_your_piece", *ctx.curLanguage)       },
        { "SPACE",     getText("Help.place_your_piece", *ctx.curLanguage)       },
        { "MOUSE",     getText("Help.click_to_place_piece", *ctx.curLanguage)   },
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
    DrawPixelText(getText("Help.save_load", *ctx.curLanguage), (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    Row saves[] = {
        { "L", getText("Help.save_current_game", *ctx.curLanguage)    },
        { "T", getText("Help.load_saved_game", *ctx.curLanguage)    },
    };
    for (auto& row : saves)
    {
        DrawPixelText(row.key, (int)keyX, (int)curY, 2, { 255, 220, 100, 230 });
        DrawPixelText(row.desc, (int)descX, (int)curY, 2, { 200, 190, 160, 210 });
        curY += 22;
    }

    curY += 14;

    // ---- TIMER ----
    DrawPixelText(getText("Help.timer", *ctx.curLanguage), (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 80, 70);
    curY += 12;

    vector<string> timerDesc;
    timerDesc.push_back(getText("Help.each_player_has_30_seconds", *ctx.curLanguage));
    timerDesc.push_back(getText("Help.per_turn_time_runs_out", *ctx.curLanguage));
    timerDesc.push_back(getText("Help.the_opponent_wins", *ctx.curLanguage));

    for (auto& line : timerDesc)
    {
        DrawPixelText(line, (int)lx, (int)curY, 2, { 210, 200, 170, 220 });
        curY += 22;
    }
}
