#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <string>

// ============================================================
//  UI HELPERS
// ============================================================

// Vẽ texture full-screen (dùng chung mọi state)
void DrawFullscreenTexture(Texture2D tex);

// Tính độ rộng pixel của chuỗi theo pixel-font
int GetPixelFontTextWidth(const std::string& text, int scale);

// ============================================================
//  BUTTONS
// ============================================================

// Nút Back (góc trái trên)
// hovered / clicked là output
void DrawBackButton(Texture2D normalTex, Texture2D hoverTex,
    bool& hovered, bool& clicked);

// Nút Setting (góc phải trên)
void DrawSettingButton(Texture2D normalTex, Texture2D hoverTex,
    int screenW, bool& hovered, bool& clicked);

// ============================================================
//  OVERLAY SCREENS (Settings / Help / About — cùng layout)
// ============================================================
// Vẽ background + tiêu đề + nút Back.
// Trả về true nếu người dùng bấm Back / ESC.
bool DrawOverlayScreen(Texture2D bg,
    const char* title,
    Texture2D backNormal,
    Texture2D backHover); 

#endif // UI_H