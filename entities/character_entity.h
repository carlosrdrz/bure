#pragma once

#include "entities/entity.h"

using namespace bure::entities;

class character_entity : public entity {
 using entity::entity;

 void init() override;
};
