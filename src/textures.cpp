#include "Textures.h"

void LoadAllTextures(TextureBank& tex)
{
    tex.background = LoadTexture("assets/background_castle.jpg");
    tex.gameplayBg = LoadTexture("assets/gameplay_bg.jpg");
    tex.originBg = LoadTexture("assets/bg_origin.jpg");

    tex.dialogInputName = LoadTexture("assets/dialog_input_name.png");
    tex.dialogChooseChar = LoadTexture("assets/dialog_choose_char.png");
    tex.dialogModeGame = LoadTexture("assets/dialog_mode.png");
    tex.dialogCharFrame = LoadTexture("assets/character_frame.png");

    tex.buttonBackNormal = LoadTexture("assets/button_back_normal.png");
    tex.buttonBackHover = LoadTexture("assets/button_back_hover.png");
    tex.buttonSettingNormal = LoadTexture("assets/button_settings_normal.png");
    tex.buttonSettingHover = LoadTexture("assets/button_settings_hover.png");

    tex.tileLight = LoadTexture("assets/tile_light.png");
    tex.tileDark = LoadTexture("assets/tile_dark.png");
    tex.spriteX = LoadTexture("assets/sprite_X.png");
    tex.spriteO = LoadTexture("assets/sprite_O.png");

    tex.spriteKnight = LoadTexture("assets/knight.png");
    tex.spriteMage = LoadTexture("assets/mage.png");
    tex.spriteArcher = LoadTexture("assets/archer.png");
    tex.spriteGoblin = LoadTexture("assets/goblin.png");
    tex.spriteDragon = LoadTexture("assets/dragon.png");
}

void UnloadAllTextures(TextureBank& tex)
{
    UnloadTexture(tex.background);
    UnloadTexture(tex.gameplayBg);
    UnloadTexture(tex.originBg);

    UnloadTexture(tex.dialogInputName);
    UnloadTexture(tex.dialogChooseChar);
    UnloadTexture(tex.dialogModeGame);
    UnloadTexture(tex.dialogCharFrame);

    UnloadTexture(tex.buttonBackNormal);
    UnloadTexture(tex.buttonBackHover);
    UnloadTexture(tex.buttonSettingNormal);
    UnloadTexture(tex.buttonSettingHover);

    UnloadTexture(tex.tileLight);
    UnloadTexture(tex.tileDark);
    UnloadTexture(tex.spriteX);
    UnloadTexture(tex.spriteO);

    UnloadTexture(tex.spriteKnight);
    UnloadTexture(tex.spriteMage);
    UnloadTexture(tex.spriteArcher);
    UnloadTexture(tex.spriteGoblin);
    UnloadTexture(tex.spriteDragon);
}