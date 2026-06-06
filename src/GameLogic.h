#pragma once
#include "Board.h"
#include "Player.h"
#include "Position.h"
#include "Constants.h"
#include "GameState.h"

int countConsecutive(const Board& board, const Position& startPos, const Position& direction, Player player);
bool checkWin(const Board& board, const Position& pos, Player player);
int findWinLine(const Board& board, const Position& pos, Player player, Position outLine[], int maxLen);
bool isBoardFull(const Board& board);
GameState playMove(const GameState& currState, const Position& movePos);
