#include "UI.h"
#include "TextRenderer.h"
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
    const int glyphWidth = 5;
    const int spacing = 1;
    return (int)text.size() * (glyphWidth + spacing) * scale;
}

//  DrawBackButton
void DrawBackButton(Texture2D normalTex, Texture2D hoverTex,
    bool& hovered, bool& clicked)
{
    const int bx = 30, by = 30;
    int bw = normalTex.width;

    float scale = (bw > 180) ? 180.0f / bw : 1.0f;
    int dw = (int)(bw * scale);
    int dh = (int)(normalTex.height * scale);

    Vector2   mouse = GetMousePosition();
    Rectangle rect = { (float)bx, (float)by, (float)dw, (float)dh };

    hovered = CheckCollisionPointRec(mouse, rect);
    clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Texture2D tex = hovered ? hoverTex : normalTex;
    DrawTextureEx(tex, Vector2{ (float)bx, (float)by }, 0, scale, WHITE);
}

//  DrawSettingButton
void DrawSettingButton(Texture2D normalTex, Texture2D hoverTex,
    int screenW, bool& hovered, bool& clicked)
{
    int bw = normalTex.width, bh = normalTex.height;
    float scaleNormal = (bw > 180) ? 180.0f / bw : 1.0f;

    int dw = (int)(bw * scaleNormal);
    int dh = (int)(bh * scaleNormal);
    int bx = screenW - dw - 30;
    int by = 30;

    Vector2   mouse = GetMousePosition();
    Rectangle rect = { (float)bx, (float)by, (float)dw, (float)dh };

    hovered = CheckCollisionPointRec(mouse, rect);
    clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Texture2D tex;
    float     scale;

    if (!hovered)
    {
        tex = normalTex;
        scale = scaleNormal;
    }
    else
    {
        tex = hoverTex;
        float sx = (float)dw / hoverTex.width;
        float sy = (float)dh / hoverTex.height;
        scale = fminf(sx, sy);
    }

    float drawW = tex.width * scale;
    float drawH = tex.height * scale;
    float dx = bx + (dw - drawW) / 2.0f;
    float dy = by + (dh - drawH) / 2.0f;

    DrawTextureEx(tex, Vector2{ dx, dy }, 0.0f, scale, WHITE);
}

//  DrawOverlayScreen
bool DrawOverlayScreen(Texture2D bg,
    const char* title,
    Texture2D backNormal,
    Texture2D backHover)
{
    DrawFullscreenTexture(bg);

    int titleScale = 8;
    int titleW = (int)(strlen(title) * 8 * titleScale);
    DrawPixelTextStyled(title,
        GetScreenWidth() / 2 - titleW / 2,
        GetScreenHeight() / 2 - 40,
        titleScale);

    bool hovered, clicked;
    DrawBackButton(backNormal, backHover, hovered, clicked);

    return clicked || IsKeyPressed(KEY_ESCAPE);
}