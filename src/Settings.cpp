#include "Overlay.h"
#include "TextRenderer.h"
#include "Constants.h"
#include "raygui.h"
#include "Sound.h"
#include <math.h>
#include <cstring>

static bool  s_bgmOn = true;
static bool  s_sfxOn = true;
static float s_bgmVolume = 50.0f;
static float s_sfxVolume = 50.0f;
static int   s_langIdx = 0;   
static const char* LANGUAGES[] = { "ENGLISH", "TIENG VIET" };
static const int   LANG_COUNT = 2;

void UpdateSettingsOverlay(AppContext& ctx)
{
    Vector2 mouse = GetMousePosition();
    Rectangle panel = GetOverlayPanelRect();
    float rowH = 70.0f;
    float startY = panel.y + 110.0f;
    float cx = panel.x + panel.width / 2.0f;

    // BGM toggle button
    float toggleW = 160, toggleH = 50;
    float rightPadding = 50.0f;
    float bx = panel.x + panel.width - toggleW - rightPadding;
    Rectangle bgmRect = { bx, startY + rowH * 0 + (rowH - toggleH) / 2, toggleW, toggleH };
    if (CheckCollisionPointRec(mouse, bgmRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        s_bgmOn = !s_bgmOn;
        if (!s_bgmOn) {
            s_bgmVolume = 0.0f;
        }
        else {
            s_bgmVolume = 50.0f;
        }
    }
    ctx.musicVolume = s_bgmVolume / 100.0f;

    // SFX toggle button
    Rectangle sfxRect = { bx, startY + rowH * 1 + (rowH - toggleH) / 2, toggleW, toggleH };
    if (CheckCollisionPointRec(mouse, sfxRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        s_sfxOn = !s_sfxOn;
        if (!s_sfxOn) {
            s_sfxVolume = 0.0f;
        }
        else {
            s_sfxVolume = 50.0f;
        }
    }
    ctx.sfxVolume =  s_sfxVolume / 100.0f;
    ChangeBGMVolume(ctx);
    ChangeSfxVolume(ctx);

    // Language left/right arrows
    float langY = startY + rowH * 3;
    Rectangle arrowL = { cx - 160, langY + (rowH - toggleH) / 2, 50, toggleH };
    Rectangle arrowR = { cx + 110, langY + (rowH - toggleH) / 2, 50, toggleH };
    if (CheckCollisionPointRec(mouse, arrowL) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        s_langIdx--; if (s_langIdx < 0) s_langIdx = LANG_COUNT - 1;
    }
    if (CheckCollisionPointRec(mouse, arrowR) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        s_langIdx++; if (s_langIdx >= LANG_COUNT) s_langIdx = 0;
    }
}

void DrawSettingsOverlay(const AppContext& ctx)
{
    float t = (float)GetTime();
    float glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;

    DrawOverlayTitle("SETTINGS", glow);

    Rectangle panel = GetOverlayPanelRect();
    DrawClassicPanel(panel, glow);

    float rowH = 70.0f;
    float startY = panel.y + 110.0f;
    
    float cx = panel.x + panel.width / 2.0f;
    float labelX = panel.x + 80.0f;

    // ---- Section header: AUDIO ----
    {
        int hw = (int)(strlen("AUDIO") * 8 * FONT_SCALE_SM);
        DrawPixelText("AUDIO", (int)(panel.x + 80), (int)(panel.y + 60),
            FONT_SCALE_SM, { 255, 200, 80, 200 });
        DrawPanelDivider(panel.x + 40, panel.y + 95, panel.width - 80, 80);
    }

    // Helper
    auto DrawToggleRow = [&](const char* label, bool isOn,float &soundVolume, float rowY)
        {
            // Label
            DrawPixelText(label, (int)labelX, (int)(rowY + 18), FONT_SCALE_SM, WHITE);

            // Toggle button
            float toggleW = 160, toggleH = 50;
            float rightPadding = 50.0f;
            float bx = panel.x + panel.width - toggleW - rightPadding;
            float by = rowY + (rowH - toggleH) / 2.0f ;

            Rectangle btnRect = { bx, by, toggleW, toggleH };

            Vector2 mouse = GetMousePosition();
            bool hov = CheckCollisionPointRec(mouse, btnRect);
            float btnGlow = hov ? (sinf(t * 6.0f) + 1.0f) / 2.0f : 0.0f;

            Color bgCol = isOn
                ? Color{ 40, 80, 40, 220 }
            : Color{ 60, 30, 30, 200 };
            Color bdrCol = isOn
                ? Color{ 80, 200, 80, (unsigned char)(160 + btnGlow * 80) }
            : Color{ 200, 60, 60, (unsigned char)(140 + btnGlow * 80) };
            Color txtCol = isOn
                ? Color{ 120, 255, 120, 255 }
            : Color{ 255, 100, 100, 220 };

            DrawRectangleRec(btnRect, bgCol);
            DrawRectangleLinesEx(btnRect, 2, bdrCol);

            const char* lbl = isOn ? "ON" : "OFF";
            int lw = (int)(strlen(lbl) * 8 * FONT_SCALE_SM);
            DrawPixelText(lbl, (int)(bx + toggleW / 2 - lw / 2),
                (int)(by + toggleH / 2 - FONT_GLYPH_H * FONT_SCALE_SM / 2),
                FONT_SCALE_SM, txtCol);
            // Sound slidebar
            float sliderW = 360.0f;
            float sliderH = 30.0f;
            float sliderX = cx - panel.x / 2.0f;
            Rectangle soundSliderRect = { sliderX, by, sliderW, sliderH };
            if (isOn) {
                GuiSliderBar(soundSliderRect, "", "", &soundVolume, 0.0f, 100.0f);
            }
            else {
                GuiDisable();
                GuiSliderBar(soundSliderRect, "", "", &soundVolume, 0.0f, 100.0f);
                GuiEnable();
            }

            char soundText[16];
            sprintf(soundText, "%d%%", (int)soundVolume);
            DrawPixelText(soundText,
                (int)(sliderX + sliderW + 12),
                (int)(rowY + 18),
                FONT_SCALE_SM,
                { 255, 220, 120, 255 });
        };

    DrawToggleRow("BGM  ", s_bgmOn, s_bgmVolume, startY + rowH * 0);
    DrawToggleRow("SFX  (Sound Effects)", s_sfxOn, s_sfxVolume, startY + rowH * 1);
    DrawPanelDivider(panel.x + 40, startY + rowH * 2 + 10, panel.width - 80, 70);

    // ---- Section header: LANGUAGE ----
    DrawPixelText("LANGUAGE", (int)(panel.x + 80), (int)(startY + rowH * 2 + 22),
        FONT_SCALE_SM, { 255, 200, 80, 200 });

    // Language selector
    {
        float langY = startY + rowH * 3;
        float toggleH = 50;

        float arrowW = 50;
        Rectangle arrowL = { cx - 160, langY + (rowH - toggleH) / 2, arrowW, toggleH };
        Rectangle arrowR = { cx + 110, langY + (rowH - toggleH) / 2, arrowW, toggleH };

        Vector2 mouse = GetMousePosition();
        bool hovL = CheckCollisionPointRec(mouse, arrowL);
        bool hovR = CheckCollisionPointRec(mouse, arrowR);
        Color arrowCol = { 200, 170, 80, 220 };
        Color arrowHov = { 255, 220, 100, 255 };

        DrawRectangleRec(arrowL, { 20, 18, 8, 180 });
        DrawRectangleLinesEx(arrowL, 2, hovL ? arrowHov : arrowCol);
        DrawPixelText("<", (int)(arrowL.x + arrowW / 2 - 3 * FONT_SCALE_SM / 2),
            (int)(arrowL.y + toggleH / 2 - FONT_GLYPH_H * FONT_SCALE_SM / 2),
            FONT_SCALE_SM, hovL ? arrowHov : arrowCol);

        DrawRectangleRec(arrowR, { 20, 18, 8, 180 });
        DrawRectangleLinesEx(arrowR, 2, hovR ? arrowHov : arrowCol);
        DrawPixelText(">", (int)(arrowR.x + arrowW / 2 - 3 * FONT_SCALE_SM / 2),
            (int)(arrowR.y + toggleH / 2 - FONT_GLYPH_H * FONT_SCALE_SM / 2),
            FONT_SCALE_SM, hovR ? arrowHov : arrowCol);

        // Tên ngôn ngữ
        const char* lang = LANGUAGES[s_langIdx];
        int lw = (int)(strlen(lang) * 8 * FONT_SCALE_SM);
        DrawPixelText(lang, (int)(cx - lw / 2),
            (int)(langY + rowH / 2 - FONT_GLYPH_H * FONT_SCALE_SM / 2),
            FONT_SCALE_SM, { 255, 255, 200, 255 });
    }

    {
        const char* note = "* Language affects UI text only";
        int nw = (int)(strlen(note) * 8 * 2);
        DrawPixelText(note, (int)(panel.x + panel.width / 2 - nw / 2),
            (int)(panel.y + panel.height - 50),
            2, { 150, 140, 110, 140 });
    }
}
