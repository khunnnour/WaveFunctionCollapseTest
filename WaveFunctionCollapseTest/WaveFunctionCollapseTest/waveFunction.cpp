#include <iostream>
#include "waveFunction.h"

// Conversion equations
int vec2dToIndex(vector2D v)
{
	return v.x + (DIMENSIONS.x * v.y);
}
void indexToVec2d(int i, vector2D& v)
{
	int x = 0, y = 0;
	int temp = i;

	if (temp >= DIMENSIONS.x)
		while (true)
		{
			temp -= DIMENSIONS.x;
			if (temp >= 0)
				y++;
			else
				break;
		}
	x = i - (y * DIMENSIONS.x);

	v.x = x;
	v.y = y;
}

void init(std::vector<tile*>& gr)
{
	for (int i = 0; i < DIMENSIONS.y; i++)
	{
		for (int j = 0; j < DIMENSIONS.x; j++)
		{
			tile* temp = new tile();
			gr.push_back(temp);
		}
	}
}

int findLowestEntropy(std::vector<tile*>& gr)
{
	int index = -1, min = typeCt + 1;

	for (unsigned int i = 0; i < gr.size(); i++)
	{
		// skip if collapsed
		if (gr[i]->collapsed)
			continue;

		int entropyCt = 0;
		// go over everything in the string
		for (unsigned int j = 0; j < gr[i]->options.size(); j++)
		{
			if (gr[i]->options[j] == '1')
				entropyCt++;
		}
		//cout << "index " << index << " has " << entropyCt << " entropy\n";
		if (entropyCt < min)
		{
			index = i;
			min = entropyCt;
		}
	}

	//cout << "lowest: " << index << endl;
	return index;
}

void generateMap(std::vector<tile*>& gr)
{
	int index = findLowestEntropy(gr);
	while (index != -1)
	{
		vector2D loc;
		indexToVec2d(index, loc);
		collapse(gr, loc);
		ripple(gr, loc);

		if (rippledOut(gr))
		{
			// display state of map when broken
			displayMap(gr);

			// delete map
			cleanup(gr);

			// restart
			init(gr);

			//break;
		}

		index = findLowestEntropy(gr);
	}
	displayMap(gr);
}

void displayMap(std::vector<tile*>& gr)
{
	for (int i = 0; i < DIMENSIONS.y; i++)
	{
		for (int j = 0; j < DIMENSIONS.x; j++)
		{
			if (gr[i * DIMENSIONS.x + j]->options == TILE_LAND)
				std::cout << "\033[0;32mL \033[0m";
			else if (gr[i * DIMENSIONS.x + j]->options == TILE_SEA)
				std::cout << "\033[0;34mS \033[0m";
			else if (gr[i * DIMENSIONS.x + j]->options == TILE_COAST)
				std::cout << "\033[0;33mC \033[0m";
			else if (gr[i * DIMENSIONS.x + j]->options == TILE_FOREST)
				std::cout << "\033[0;35mF \033[0m";
			else
				std::cout << "-1";
		}
		std::cout << std::endl;
	}
}

bool conflicts(tile* a, tile* b)
{
	if ((a->options == TILE_LAND && b->options == TILE_SEA) ||
		(a->options == TILE_FOREST && b->options == TILE_SEA))
		return true;
	else
		return false;
}

void collapse(std::vector<tile*>& gr, vector2D loc)
{
	float Weight_Total = WEIGHT_FOREST + WEIGHT_LAND + WEIGHT_COAST + WEIGHT_SEA;

	bool collapsed = false;

	int index = vec2dToIndex(loc);

	while (!collapsed)
	{
		int r;// = rand()%typeCt;

		float t = (float)(rand() % 100) / 100.0f;

		if (t <= WEIGHT_SEA / Weight_Total)
		{
			r = 0;
		}
		else if (t <= (WEIGHT_SEA + WEIGHT_COAST) / Weight_Total)
		{
			r = 1;
		}
		else if (t <= (WEIGHT_SEA + WEIGHT_COAST + WEIGHT_LAND) / Weight_Total)
		{
			r = 2;
		}
		else
		{
			r = 3;
		}

		if (gr[index]->options[r] == '1')
		{
			gr[index]->options = TILE_EMPTY;
			gr[index]->options[r] = '1';
			gr[index]->collapsed = true;

			collapsed = true;

			//cout << "Collapsed index " << index << " - " << gr[index]->options << endl;
		}
	}

	ripple(gr, loc);
}

void ripple(std::vector<tile*>& gr, vector2D center)
{
	int index = vec2dToIndex(center);
	// remove options incompatible with TILE_LAND
	if (gr[index]->options == TILE_LAND)
	{
		// Ripple DIR_LEFT
		if (center.x - 1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x - 1, center.y))]->options[0] = '0';
		}
		// ripple DIR_RIGHT
		if (center.x + 1 < DIMENSIONS.x)
		{
			gr[vec2dToIndex(vector2D(center.x + 1, center.y))]->options[0] = '0';
		}
		if (center.y - 1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x, center.y - 1))]->options[0] = '0';
		}
		if (center.y + 1 < DIMENSIONS.y)
		{
			gr[vec2dToIndex(vector2D(center.x, center.y + 1))]->options[0] = '0';
		}
	}

	// remove options incompatible with TILE_SEA
	if (gr[index]->options == TILE_SEA)
	{
		// Ripple DIR_LEFT
		if (center.x - 1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x - 1, center.y))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x - 1, center.y))]->options[3] = '0';
		}
		// ripple DIR_RIGHT
		if (center.x + 1 < DIMENSIONS.x)
		{
			gr[vec2dToIndex(vector2D(center.x + 1, center.y))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x + 1, center.y))]->options[3] = '0';
		}
		if (center.y - 1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x, center.y - 1))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x, center.y - 1))]->options[3] = '0';
		}
		if (center.y + 1 < DIMENSIONS.y)
		{
			gr[vec2dToIndex(vector2D(center.x, center.y + 1))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x, center.y + 1))]->options[3] = '0';
		}
	}

	// remove options incompatible with TILE_FOREST
	if (gr[index]->options == TILE_FOREST)
	{
		// Ripple DIR_LEFT
		if (center.x - 1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x - 1, center.y))]->options[0] = '0';
		}
		// ripple DIR_RIGHT
		if (center.x + 1 < DIMENSIONS.x)
		{
			gr[vec2dToIndex(vector2D(center.x + 1, center.y))]->options[0] = '0';
		}
		if (center.y - 1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x, center.y - 1))]->options[0] = '0';
		}
		if (center.y + 1 < DIMENSIONS.y)
		{
			gr[vec2dToIndex(vector2D(center.x, center.y + 1))]->options[0] = '0';
		}
	}
}

// make sure all options arent gone in a tile
bool rippledOut(std::vector<tile*>& gr)
{
	for (unsigned int i = 0; i < gr.size(); i++)
		if (gr[i]->options == TILE_EMPTY)
		{
			std::cout << "\nERROR: WE DON MESSED UP\n	map[" << i
				<< "] = " << TILE_EMPTY << std::endl;
			return true;
		}
	return false;
}

void cleanup(std::vector<tile*>& gr)
{
	for (unsigned int i = 0; i < gr.size(); i++)
		delete gr[i];

	gr.clear();
}