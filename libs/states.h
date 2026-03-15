#ifndef STATES_H
#define STATES_H
#include "types.h"
#include "textures.h"

// --- MENU ---
void UpdateMenu(AppContext& ctx);
void DrawMenu(const AppContext& ctx, const TextureBank& tex);

// --- MODE SELECTION ---
void UpdateModeSelection(AppContext& ctx);
void DrawModeSelection(const AppContext& ctx, const TextureBank& tex);

// --- PLAYER SETUP ---
void UpdatePlayerSetup(AppContext& ctx);
void DrawPlayerSetup(const AppContext& ctx, const TextureBank& tex);

// --- GAMEPLAY ---
void UpdateGameplay(AppContext& ctx);
void DrawGameplay(const AppContext& ctx, const TextureBank& tex);

// --- OVERLAY (Settings / Help / About — dùng chung 1 cặp hàm) ---
void UpdateOverlay(AppContext& ctx, GameState returnTo);
void DrawOverlay(const AppContext& ctx, const TextureBank& tex, const char* title);

#endif // STATES_H