#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "raylib.h"

// ============================================================
//  TimeUtils
// ============================================================
struct TimeUtils
{
    float timer = 0.0f;
    float blinkTimer = 0.0f;
    float flashTimer = 0.0f;
    float fadeTimer = 0.0f;
    float fadeDuration = 0.0f;
    Color fadeColor = { 0, 0, 0, 0 };
    bool  isFading = false;

    void Reset()
    {
        timer = 0.0f;
        blinkTimer = 0.0f;
        flashTimer = 0.0f;
        fadeTimer = 0.0f;
        fadeDuration = 0.0f;
        fadeColor = { 0, 0, 0, 0 };
        isFading = false;
    }

    void Update(float dt)
    {
        timer += dt;
        blinkTimer += dt;
        flashTimer += dt;
        if (isFading)
        {
            fadeTimer += dt;
            if (fadeTimer >= fadeDuration)
            {
                fadeTimer = fadeDuration;
                isFading = false;
            }
        }
    }

    bool IsBlinkOn(float period = 0.5f)
    {
        return fmodf(blinkTimer, period) < period * 0.5f;
    }

    bool IsFlashOn(float period = 0.15f)
    {
        return fmodf(flashTimer, period) < period * 0.5f;
    }

    float GetFadeAlpha()
    {
        if (!isFading) return 0.0f;
        return fadeTimer / fadeDuration;
    }

    void StartFadeOut(float duration, Color color)
    {
        fadeDuration = duration;
        fadeColor = color;
        fadeTimer = 0.0f;
        isFading = true;
    }

    void StartFadeIn(float duration, Color color)
    {
        fadeDuration = duration;
        fadeColor = color;
        fadeTimer = duration;
        isFading = true;
    }
};

#endif // TIME_UTILS_H
