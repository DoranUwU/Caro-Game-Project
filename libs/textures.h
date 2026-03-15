#pragma once
#include "raylib.h"

// ============================================================
//  TextureBank
//  Tập trung toàn bộ texture vào một struct.
//  Load / Unload thông qua 2 hàm helper bên dưới.
// ============================================================
struct TextureBank
{
    // --- Backgrounds ---
    Texture2D background;
    Texture2D gameplayBg;
    Texture2D originBg;

    // --- Dialogs ---
    Texture2D dialogInputName;
    Texture2D dialogChooseChar;
    Texture2D dialogModeGame;
    Texture2D dialogCharFrame;

    // --- Buttons ---
    Texture2D buttonBackNormal;
    Texture2D buttonBackHover;
    Texture2D buttonSettingNormal;
    Texture2D buttonSettingHover;

    // --- Board tiles ---
    Texture2D tileLight;
    Texture2D tileDark;
    Texture2D spriteX;
    Texture2D spriteO;

    // --- Characters ---
    Texture2D spriteKnight;
    Texture2D spriteMage;
    Texture2D spriteArcher;
    Texture2D spriteGoblin;
    Texture2D spriteDragon;
};

// ============================================================
//  Declarations
// ============================================================
void LoadAllTextures(TextureBank& tex);
void UnloadAllTextures(TextureBank& tex);