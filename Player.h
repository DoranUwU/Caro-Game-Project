#pragma once

enum class Player {
    NONE = 0,
    PlayerX = 1,
    PlayerO = 2
};

inline Player getOpponent(Player player) {
    if (player == Player::PlayerX) return Player::PlayerO;
    if (player == Player::PlayerO) return Player::PlayerX;
    return Player::NONE;
}
