#pragma once

#include "game.h"
#include "textures.h"
#include "utils/algorithm.h"
#include "utils/math.h"

enum EntityDeathType {
    UNKNOWN,
    OUT_OF_BOUND,
    PLAYER_INTERACTION
};

class Entity
{
public:
    GameTexture* entityTexture;
    vec2_t position;
    vec2_t direction;
    vec2_t rotation;

    Entity(vec2_t position, vec2_t direction, vec2_t rotation = vec2_t(0, 0)) {
        this->position = position;
        this->direction = direction;
        this->rotation = rotation;
    }

    Entity(vec2_t position, float speed, float angle, vec2_t rotation = vec2_t(0, 0)) {
        this->position = position;
        this->rotation = rotation;

        float radian = deg2rad(angle);
        this->direction = vec2_t(speed * cos(radian), speed * sin(radian));
    }

    bool alive = true;

    virtual void onTick();
    virtual void onRender() = 0;
    virtual void onHit() = 0;
    virtual void onDespawn(EntityDeathType type = EntityDeathType::UNKNOWN) = 0;

    void despawn(EntityDeathType type = EntityDeathType::UNKNOWN);
    bool IsCollidingWithPoint(int x, int y);
};