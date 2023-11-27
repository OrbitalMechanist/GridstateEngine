#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

// Entity type definition
using Entity = uint32_t;

// Constant for an invalid entity
constexpr Entity InvalidEntity = static_cast<Entity>(-1);

#endif // ENTITY_H
