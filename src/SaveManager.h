#pragma once
#include "GameState.h"
#include "Save.h"
#include "Load.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <cstdio>
#include <ctime>

using json = nlohmann::json;

struct SavePreview {
    bool exists = false;
    int moveCount = 0;
    double timestamp = 0.0;
    GameStatus status = GameStatus::ONGOING;
};

// Trả về đường dẫn file save theo slot number
inline const char* GetSlotPath(int slot) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "saves/slot_%d.json", slot);
    return buf;
}

inline bool SaveManagerSave(int slot, const GameState& state) {
    std::filesystem::create_directories("saves");

    const char* path = GetSlotPath(slot);

    json root = gameStateToJson(state);

    // add metadata
    int moveCount = 0;
    for (int i = 0; i < TOTAL_CELLS; i++) {
        if (state.board.cell[i] != Player::NONE)
            moveCount++;
    }

    root["moveCount"] = moveCount;
    root["timestamp"] = time(nullptr);

    std::string data = root.dump(2);
    return SaveFileText(path, const_cast<char*>(data.c_str()));
}

inline bool SaveManagerLoad(int slot, GameState& state) {
    return loadGameState(GetSlotPath(slot), state);
}

inline bool SaveManagerLoadPreview(int slot, SavePreview& preview) {
    preview.exists = false;

    const char* path = GetSlotPath(slot);
    if (!FileExists(path)) return false;

    char* content = LoadFileText(path);
    if (!content) return false;

    try {
        json root = json::parse(content);

        preview.exists = true;
        preview.moveCount = root.value("moveCount", 0);
        preview.timestamp = root.value("timestamp", 0.0);

        int statusInt = root.value("status", 0);
        intToGameStatus(statusInt, preview.status);

        UnloadFileText(content);
        return true;
    }
    catch (...) {
        UnloadFileText(content);
        return false;
    }
}
