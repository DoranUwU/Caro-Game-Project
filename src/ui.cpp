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
    // 1. Vẽ hình nền (Background)
    DrawFullscreenTexture(bg);

    // ==========================================
    // 2. VẼ Ô ĐEN MỜ TRUNG TÂM (THÊM MỚI Ở ĐÂY)
    // ==========================================
    float panelW = GetScreenWidth() * 0.7f;
    float panelH = GetScreenHeight() * 0.7f;
    float panelX = (GetScreenWidth() / 2.0f) - (panelW / 2.0f);
    float panelY = (GetScreenHeight() / 2.0f) - (panelH / 2.0f);
    
    // Sử dụng Fade(BLACK, 0.75f) để tạo màu đen với 75% opacity
    DrawRectangle((int)panelX, (int)panelY, (int)panelW, (int)panelH, Fade(BLACK, 0.75f));
    // ==========================================

    // 3. Vẽ Tiêu đề (Góc trên bên phải)
    Color antiqueGold = { 218, 165, 32, 255 };
    int titleScale = 10;
    int titleW = (int)(strlen(title) * 8 * titleScale);
    int padding = 60;
    int x = GetScreenWidth() - titleW - padding;
    int y = padding;
    
    
    DrawText(title, x + 4, y + 4, titleScale * 8, Fade(BLACK, 0.5f)); 
    DrawText(title, x, y, titleScale * 8, antiqueGold);

    // 4. Vẽ nút Back
    bool hovered, clicked;
    DrawBackButton(backNormal, backHover, hovered, clicked);

    // 5. Trả về kết quả
    return clicked || IsKeyPressed(KEY_ESCAPE);
}