#pragma once
#include "AIEval.h"
#include "GameLogic.h"
#include "AIUtils.h"

MoveList getRelevantMoves(const GameState& state);
MoveList getOrderedMoves(const GameState& state);
int negamax(const GameState& state, int depth, int alpha, int beta, int colourMul);
int negamax_improved(const GameState& state, int depth, int alpha, int beta, int colourMul);