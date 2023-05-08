#include "Enemy.h"
#include <unordered_map>

unordered_map<string, move_strategy> Enemy::move_strategies={
    {"stupid",[](Enemy* enemy){
        // Move towards a fixed direction, and change when met obstacle
    }},
    {"towards_player",[](Enemy* enemy){
        // Always move towards the player

    }}
};
