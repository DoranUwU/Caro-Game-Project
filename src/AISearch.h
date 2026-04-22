#pragma once
#include "AIEval.h"
#include "GameLogic.h"

int negamax(const GameState& state, int depth, int alpha, int beta, int colourMul);