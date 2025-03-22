#pragma once

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

    bool alive = true;

    virtual void onTick(int deltaTicks);
    virtual void onRender() = 0;
    virtual void onHit() = 0;
    virtual void onDespawn(EntityDeathType type = EntityDeathType::UNKNOWN) = 0;
    virtual bool IsCollidingWithPoint(int x, int y) = 0;

    void despawn(EntityDeathType type = EntityDeathType::UNKNOWN);
};