#pragma once
#include <string>
#include "vector2D.h"

// Dimensions
const vector2D DIMENSIONS = vector2D(28, 17);
// Directions
const vector2D DIR_TOP		= vector2D( 0,  1);
const vector2D DIR_BOT		= vector2D( 0, -1);
const vector2D DIR_LEFT		= vector2D(-1,  0);
const vector2D DIR_RIGHT	= vector2D( 1,  0);
// Area types
const int typeCt = 4;
const std::string TILE_FOREST	= "0001";
const std::string TILE_LAND		= "0010";
const std::string TILE_COAST	= "0100";
const std::string TILE_SEA		= "1000";
const std::string TILE_FULL		= "1111";
const std::string TILE_EMPTY	= "0000";
// Tile weights
const float WEIGHT_FOREST	= 1.00f;
const float WEIGHT_LAND		= 1.00f;
const float WEIGHT_COAST	= 1.50f;
const float WEIGHT_SEA		= 1.00f;