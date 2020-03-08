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


// Initialize the graph
void init(std::vector<tile*>&);
// Generate random map
void generateMap(std::vector<tile*>&);
// Seed map at location with certain value
void seedMap(std::vector<tile*>&, vector2D, int);
// Display the map
void displayMap(std::vector<tile*>&);
// Collapse a tile
void collapse(std::vector<tile*>&, vector2D);
// Update surrounding tiles
void ripple(std::vector<tile*>&, vector2D);
// Check if two tiles conflict
bool conflicts(tile*, tile*);
// Check if map contains errors
bool rippledOut(std::vector<tile*>&);
// Cleanup
void cleanup(std::vector<tile*>&);
