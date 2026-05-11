#ifndef _LOAD_H_
#define _LOAD_H_

#include "GameState.h"
#include "raylib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

inline bool intToPlayer(int value, Player& outPlayer) {
	switch (value) {
	case static_cast<int>(Player::NONE):
		outPlayer = Player::NONE;
		return true;
	case static_cast<int>(Player::PlayerX):
		outPlayer = Player::PlayerX;
		return true;
	case static_cast<int>(Player::PlayerO):
		outPlayer = Player::PlayerO;
		return true;
	default:
		return false;
	}
}

inline bool intToGameStatus(int value, GameStatus& outStatus) {
	switch (value) {
	case static_cast<int>(GameStatus::ONGOING):
		outStatus = GameStatus::ONGOING;
		return true;
	case static_cast<int>(GameStatus::DRAW):
		outStatus = GameStatus::DRAW;
		return true;
	case static_cast<int>(GameStatus::WIN_X):
		outStatus = GameStatus::WIN_X;
		return true;
	case static_cast<int>(GameStatus::WIN_O):
		outStatus = GameStatus::WIN_O;
		return true;
	default:
		return false;
	}
}

inline bool jsonToGameState(const json& root, GameState& outState) {
	if (!root.is_object()) {
		return false;
	}
	if (!root.contains("board") || !root["board"].is_array()) {
		return false;
	}
	if (static_cast<int>(root["board"].size()) != TOTAL_CELLS) {
		return false;
	}
	if (!root.contains("currentPlayer") || !root["currentPlayer"].is_number_integer()) {
		return false;
	}
	if (!root.contains("status") || !root["status"].is_number_integer()) {
		return false;
	}
	if (!root.contains("lastMove") || !root["lastMove"].is_object()) {
		return false;
	}

	const json& lastMove = root["lastMove"];
	if (!lastMove.contains("row") || !lastMove["row"].is_number_integer()) {
		return false;
	}
	if (!lastMove.contains("column") || !lastMove["column"].is_number_integer()) {
		return false;
	}

	GameState parsed;

	Player parsedCurrentPlayer;
	if (!intToPlayer(root["currentPlayer"].get<int>(), parsedCurrentPlayer)) {
		return false;
	}
	parsed.currentPlayer = parsedCurrentPlayer;

	GameStatus parsedStatus;
	if (!intToGameStatus(root["status"].get<int>(), parsedStatus)) {
		return false;
	}
	parsed.status = parsedStatus;

	parsed.lastMove = Position(lastMove["row"].get<int>(), lastMove["column"].get<int>());

	for (int i = 0; i < TOTAL_CELLS; ++i) {
		if (!root["board"][i].is_number_integer()) {
			return false;
		}
		Player cellPlayer;
		if (!intToPlayer(root["board"][i].get<int>(), cellPlayer)) {
			return false;
		}
		parsed.board.cell[i] = cellPlayer;
	}

	outState = parsed;
	return true;
}

inline bool loadGameState(const char* filename, GameState& outState) {
	if (filename == nullptr || filename[0] == '\0') {
		return false;
	}
	if (!FileExists(filename)) {
		return false;
	}

	char* content = LoadFileText(filename);
	if (content == nullptr) {
		return false;
	}

	try {
		const json root = json::parse(content);
		UnloadFileText(content);
		return jsonToGameState(root, outState);
	}
	catch (...) {
		UnloadFileText(content);
		return false;
	}
}

#endif