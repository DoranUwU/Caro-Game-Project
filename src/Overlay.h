#ifndef OVERLAY_H
#define OVERLAY_H

#include "raylib.h"
#include "Types.h"

// Shared UI Helpers for overlays
Rectangle GetOverlayPanelRect(float wRatio = 0.62f, float hRatio = 0.68f);
void DrawClassicPanel(Rectangle panel, float glow);
void DrawOverlayTitle(const char* title, float glow);
void DrawPanelDivider(float x, float y, float w, float alpha);

// Specific overlays
void UpdateSettingsOverlay(AppContext& ctx);
void DrawSettingsOverlay(const AppContext& ctx);

void DrawHelpOverlay(const AppContext& ctx);

void DrawAboutUsOverlay(const AppContext& ctx);

#endif // OVERLAY_H
