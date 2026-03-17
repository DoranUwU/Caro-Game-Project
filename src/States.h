#ifndef STATES_H
#define STATES_H

#include "Types.h"      
#include "Textures.h"   
#include "Board.h"     


void UpdateMenu(AppContext& ctx);
void DrawMenu(const AppContext& ctx, const TextureBank& tex);

void UpdateModeSelection(AppContext& ctx);
void DrawModeSelection(const AppContext& ctx, const TextureBank& tex);

void UpdatePlayerSetup(AppContext& ctx);
void DrawPlayerSetup(const AppContext& ctx, const TextureBank& tex);

void UpdateGameplay(AppContext& ctx);
void DrawGameplay(const AppContext& ctx, const TextureBank& tex);

void UpdateOverlay(AppContext& ctx, UIScreen returnTo);
void DrawOverlay(const AppContext& ctx, const TextureBank& tex, const char* title);

#endif // STATES_H