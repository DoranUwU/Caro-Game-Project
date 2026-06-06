#ifndef TEXTURES_H
#define TEXTURES_H

#include "raylib.h"

// ============================================================
//  TextureBank
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
    Texture2D spriteKnight_L;
    Texture2D spriteMage_L;
    Texture2D spriteArcher_L;
    Texture2D spriteKnight_R;
    Texture2D spriteMage_R;
    Texture2D spriteArcher_R;
    Texture2D spriteGoblin;
    Texture2D spriteDragon;
};

// ============================================================
//  Declarations
// ============================================================
void LoadAllTextures(TextureBank& tex);
void UnloadAllTextures(TextureBank& tex);

#endif // TEXTURES_H