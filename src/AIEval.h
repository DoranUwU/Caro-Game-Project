#pragma once
#include "GameState.h"
//Calculate the current state's score. if X is winning then positive, otherwise negative.


//Get score for current state
int getPatternScore(int consecutivePieces, int openEnds);

//Score how threatening a position is for a given player.
//Simulates placing player's piece at pos and scans all 4 directions.
int scoreThreat(const Board& board, Position pos, Player player);

//Evaluate function
int evalBoard(const GameState& state);