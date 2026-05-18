#pragma once

#include "raylib.h"

// ============================================================
//  SoundBank
//  Tập trung toàn bộ âm thanh vào một struct.
//  Load / Unload thông qua 2 hàm helper bên dưới.
// ============================================================
struct AppContext;
struct SoundBank {
    Music bgm;
    Music ingame;

    Sound win;
    Sound clickSfx;
    Sound placeSfx;
    Sound choiceSfx;
};

void LoadAllSounds(SoundBank &sound);
void UnloadAllSounds(SoundBank &sound);

void PlayMenuBGM(AppContext& ctx);
void PlayInGameBGM(AppContext& ctx);
void UpdateCurrentMusic(AppContext& ctx);

void PlaySfx(Sound& sound);

void ChangeBGMVolume(AppContext& ctx);
void ChangeSfxVolume(AppContext& ctx);

