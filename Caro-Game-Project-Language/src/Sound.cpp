#include "Sound.h"
#include "Types.h"


void LoadAllSounds(SoundBank& sound) {
    sound.bgm = LoadMusicStream("sound/bgm.mp3");
    sound.ingame = LoadMusicStream("sound/bgm_ingame.mp3");

    sound.win = LoadSound("sound/win.mp3");
    sound.placeSfx = LoadSound("sound/placesfx.mp3");
    sound.clickSfx = LoadSound("sound/click_sfx.mp3");
    sound.choiceSfx = LoadSound("sound/selectsfx.mp3");

}

void UnloadAllSounds(SoundBank &sound) {
    UnloadMusicStream(sound.bgm);
    UnloadMusicStream(sound.ingame);

    UnloadSound(sound.win);
    UnloadSound(sound.placeSfx);
    UnloadSound(sound.clickSfx);
    UnloadSound(sound.choiceSfx);

}

void PlaySfx(Sound& sound) {
    float randomPitch = 0.9f + (float)(GetRandomValue(0, 20)) / 100.0f;
    SetSoundPitch(sound, randomPitch);
    PlaySound(sound);
}
void PlayMenuBGM(AppContext& ctx) {
    if (ctx.curBGM == BGM_MENU) {
        return;
    }

    StopMusicStream(ctx.sound->ingame);
    PlayMusicStream(ctx.sound->bgm);
    ctx.curBGM = BGM_MENU;
}

void PlayInGameBGM(AppContext& ctx) {
    if (ctx.curBGM == BGM_INGAME) {
        return;
    }
    
    StopMusicStream(ctx.sound->bgm);
    PlayMusicStream(ctx.sound->ingame);
    ctx.curBGM = BGM_INGAME;
}

void UpdateCurrentMusic(AppContext& ctx) {
    switch (ctx.curBGM) {
        case BGM_MENU:
            UpdateMusicStream(ctx.sound->bgm);
            break;
        case BGM_INGAME:
            UpdateMusicStream(ctx.sound->ingame);
            break;
        default:
            break;
    }

}

void ChangeBGMVolume(AppContext& ctx) {
    SetMusicVolume(ctx.sound->bgm,ctx.musicVolume);
    SetMusicVolume(ctx.sound->ingame,ctx.musicVolume);
}

void ChangeSfxVolume(AppContext& ctx) {
    SetSoundVolume(ctx.sound->win,ctx.sfxVolume);
    SetSoundVolume(ctx.sound->clickSfx,ctx.sfxVolume);
    SetSoundVolume(ctx.sound->placeSfx,ctx.sfxVolume);
}
