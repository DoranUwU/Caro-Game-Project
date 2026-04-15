#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <math.h>
#include <cstring>

//  DrawFullscreenTexture
void DrawFullscreenTexture(Texture2D tex)
{
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dst = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    DrawTexturePro(tex, src, dst, { 0, 0 }, 0, WHITE);
}

//  GetPixelFontTextWidth
int GetPixelFontTextWidth(const std::string& text, int scale)
{
    if (text.empty()) return 0;
    return (int)text.size() * (FONT_GLYPH_W + FONT_SPACING) * scale;
}

//  DrawBackButton
void DrawBackButton(Texture2D normalTex, Texture2D hoverTex,
    bool& hovered, bool& clicked)
{
    int bw = normalTex.width;

    float scale = (bw > 180) ? 180.0f / bw : 1.0f;
    int dw = (int)(bw * scale);
    int dh = (int)(normalTex.height * scale);

    Vector2   mouse = GetMousePosition();
    Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, (float)dw, (float)dh };

    hovered = CheckCollisionPointRec(mouse, rect);
    clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    float glow = hovered
        ? (sinf((float)GetTime() * 6.0f) + 1.0f) / 2.0f
        : 0.0f;
    unsigned char bright = hovered
        ? (unsigned char)(210 + glow * 45)
        : 150;
    Color tint = { bright, bright, bright, 255 };

    DrawTextureEx(normalTex, Vector2{ BTN_BACK_X, BTN_BACK_Y }, 0, scale, tint);
}

//  DrawSettingButton
void DrawSettingButton(Texture2D normalTex, Texture2D hoverTex,
    int screenW, bool& hovered, bool& clicked)
{
    int bw = normalTex.width, bh = normalTex.height;
    float scale = (bw > BTN_MAX_W) ? 180.0f / bw : 1.0f;

    int dw = (int)(bw * scale);
    int dh = (int)(bh * scale);
    int bx = screenW - dw - 30;
    int by = 30;

    Vector2   mouse = GetMousePosition();
    Rectangle rect = { (float)bx, (float)by, (float)dw, (float)dh };

    hovered = CheckCollisionPointRec(mouse, rect);
    clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    float glow = hovered
        ? (sinf((float)GetTime() * 6.0f) + 1.0f) / 2.0f
        : 0.0f;
    unsigned char bright = hovered
        ? (unsigned char)(210 + glow * 45)
        : 150;
    Color tint = { bright, bright, bright, 255 };

    DrawTextureEx(normalTex, Vector2{ (float)bx, (float)by }, 0.0f, scale, tint);
}

//  DrawOverlayScreen
bool DrawOverlayScreen(Texture2D bg,
    const char* title,
    Texture2D backNormal,
    Texture2D backHover)
{
    DrawFullscreenTexture(bg);

    int titleW = (int)(strlen(title) * 8 * FONT_SCALE_XL);
    DrawPixelTextStyled(title,
        GetScreenWidth() / 2 - titleW / 2,
        GetScreenHeight() / 2 - 40,
        FONT_SCALE_XL);

    bool hovered, clicked;
    DrawBackButton(backNormal, backHover, hovered, clicked);

    return clicked || IsKeyPressed(KEY_ESCAPE);
}