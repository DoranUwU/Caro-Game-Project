#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <string>

// Vẽ texture full-screen 
void DrawFullscreenTexture(Texture2D tex);

int GetPixelFontTextWidth(const std::string& text, int scale);

//  BUTTONS
// Nút Back 
void DrawBackButton(Texture2D normalTex, Texture2D hoverTex,
    bool& hovered, bool& clicked);

// Nút Setting 
void DrawSettingButton(Texture2D normalTex, Texture2D hoverTex,
    int screenW, bool& hovered, bool& clicked);

//  OVERLAY SCREENS 
bool DrawOverlayScreen(Texture2D bg,
    const char* title,
    Texture2D backNormal,
    Texture2D backHover); 

#endif // UI_H