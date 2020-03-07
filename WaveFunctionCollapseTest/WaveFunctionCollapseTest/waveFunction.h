#pragma once
#include <vector>
#include "globals.h"

// Tile struct              	
struct tile
{
public:
	bool collapsed;
	std::string options;

	tile() { collapsed = false; options = TILE_FULL; }
};


// Prototypes
void init(std::vector<tile*>&);
void generateMap(std::vector<tile*>&);
void displayMap(std::vector<tile*>&);
void collapse(std::vector<tile*>&, vector2D);
void ripple(std::vector<tile*>&, vector2D);
bool conflicts(tile*, tile*);
bool rippledOut(std::vector<tile*>&);
void cleanup(std::vector<tile*>&);
