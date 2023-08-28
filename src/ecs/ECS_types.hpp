
#pragma once
#include "utils.hpp"

ENUM(ComponentId,
// Basic
POSITION,
INVENTORY,
LOOT,
NAME,
HEALTH,
DEATH,

// Rendering
SPRITE,
LIGHT,
PARTICLE,

// Player
PLAYER,
CONTROLLER,

// Static
GRID,
INTERACTION,
RESOURCE,

// Dynamic
CREATURE_STATE,
MOVEMENT,
DIRECTION,
FORCE,
COLLIDER,
HITBOX,
CHUNK,

// Item
ITEM,
ITEM_KIND,
DAMAGE,

// AI
AI,
SENSOR,
AI_WANDER,
AI_MOVE,
AI_FLEE,
AI_CHASE,
AI_MELEE,

// Misc
PORTAL,
MATURITY,
LAUNCHER,
PROJECTILE)

ENUM(SystemId,
// Basic
POSITION,
HEALTH,
LOOT,
DAMAGE,
DEATH,
INVENTORY_DEATH,

// Rendering
ENTITY_DRAW,
CREATURE_ANIMATION,
HAND_RENDER,
PARTICLE_EMIT,
CREATURE_PARTICLE,
COLLIDER_DRAW,
HITBOX_DRAW,
LIGHT,

//Player
PLAYER,
CONTROLLER,
ITEM_PICKUP,

// Dynamic
CREATURE_MOVEMENT,
COLLISION,
CHUNK,

FORAGE,

// Static
GRID,
INTERACTION,

// AI
SENSOR,
AI,
AI_WANDER,
AI_MOVE,
AI_FLEE,
AI_CHASE,
AI_MELEE,

// Misc
PROJECTILE,
MATURITY)

using Entity = uint;
using EntitySet = std::unordered_set<Entity>;
using Signature = std::bitset<ComponentId::count>;
using GridMap = std::unordered_map<pair, Entity>;
constexpr Entity MAX_ENTITIES = 100000;

using Collisions = std::unordered_map<Entity,std::vector<Entity>>;
using gridMap = std::unordered_map<pair, Entity>;