#pragma once

#include "constant.h"
#include "textures.h"
#include "utils/math.h"

enum EntityDeathType {
    UNKNOWN,
    OUT_OF_BOUND,
    PLAYER_INTERACTION
};

enum EntityType {
	PARTICLE,
	ENEMY,
};

class Entity
{
public:
    GameTexture* entityTexture = nullptr;
    vec2_t position;
    vec2_t direction;
    vec2_t hitbox;

    bool alive = true;

    Entity(vec2_t position, vec2_t direction);
    Entity(vec2_t position, float speed, float angle);

    void SetTexture(GameTexture* texture);
    void SetHitbox(vec2_t hitbox);
    bool IsColliding(int x, int y);
    void despawn(EntityDeathType type = EntityDeathType::UNKNOWN);

    virtual void onTick(int deltaTicks);
    virtual void onRender() = 0;
    virtual void onHit() = 0;
    virtual void onDespawn(EntityDeathType type = EntityDeathType::UNKNOWN) = 0;
};