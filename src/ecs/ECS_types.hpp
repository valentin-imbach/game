
#pragma once
#include "utils.hpp"

ENUM(ComponentId,
//* Basic
POSITION,
INVENTORY,
LOOT,
NAME,
HEALTH,
DEATH,
TAG,

//* Rendering
SPRITE,
CREATURE_ANIMATION,
LIGHT,
PARTICLE,

//* Player
PLAYER,
CONTROLLER,

//* Static
GRID,
INTERACTION,
RESOURCE,

//* Dynamic
ACTION,
MOVEMENT,
DIRECTION,
FORCE,
COLLIDER,
HITBOX,
CHUNK,

//* Item
ITEM,
ITEM_KIND,
MELEE_ITEM,
DURABILITY,

//* AI
AI,
SENSOR,
AI_WANDER,
AI_MOVE,
AI_FLEE,
AI_CHASE,
AI_MELEE,

//* Misc
PORTAL,
MATURITY,
LAUNCHER,
PROJECTILE,
DAMAGE,
EFFECT,
EXPLOSIVE,
ITEM_MOD,
HAND)

ENUM(SystemId,
//* Basic
POSITION,
HEALTH,
LOOT,
DEATH,
INVENTORY_DEATH,

//* Rendering
ENTITY_DRAW,
CREATURE_ANIMATION,
HAND_RENDER,
PARTICLE_EMIT,
CREATURE_PARTICLE,
COLLIDER_DRAW,
HITBOX_DRAW,
LIGHT,

//*Player
PLAYER,
CONTROLLER,
ITEM_PICKUP,

//* Dynamic
CREATURE_MOVEMENT,
COLLISION,
HITBOX,
CHUNK,

FORAGE,
CREATURE_ACTION,
SHOVING,

//* Static
GRID,
INTERACTION,

//* AI
SENSOR,
AI,
AI_WANDER,
AI_MOVE,
AI_FLEE,
AI_CHASE,
AI_MELEE,

//* Misc
PROJECTILE,
MATURITY,
DAMAGE,
EFFECT,
EXPLOSIVE,
ITEM_MOD)

using Entity = uint;

using EntitySet = std::unordered_set<Entity>;
using Signature = std::bitset<ComponentId::count>;
using GridMap = std::unordered_map<pair, Entity>;
using EntityMap = std::unordered_map<Entity, std::vector<Entity>>;