#pragma once
#include "GameState.h"
//Calculate the current state's score. if X is winning then positive, otherwise negative.


//Get score for current state
int getPatternScore(int consecutivePieces, int openEnds);

//Evaluate function
int evalBoard(const GameState& state);