#ifndef _SAVE_H_
#define _SAVE_H_

#include "GameState.h"
#include "raylib.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

inline json gameStateToJson(const GameState& state) {
	json root;
	root["currentPlayer"] = static_cast<int>(state.currentPlayer);
	root["status"] = static_cast<int>(state.status);
	root["lastMove"] = {
		{"row", state.lastMove.row},
		{"column", state.lastMove.column}
	};

	json board = json::array();
	for (int i = 0; i < TOTAL_CELLS; ++i) {
		board.push_back(static_cast<int>(state.board.cell[i]));
	}
	root["board"] = board;

	return root;
}

inline bool saveGameState(const GameState& state, const char* filename) {
	if (filename == nullptr || filename[0] == '\0') {
		return false;
	}

	const std::string payload = gameStateToJson(state).dump(2);
	return SaveFileText(filename, const_cast<char*>(payload.c_str()));
}

#endif