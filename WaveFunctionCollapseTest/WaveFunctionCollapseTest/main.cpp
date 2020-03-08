#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "waveFunction.h"

int main()
{
	// init rng
	srand((unsigned)time(0));

	// init map
	std::vector<tile*> graph;
	init(graph);

	// seed map
	vector2D seedLoc = vector2D(0, 0);
	seedMap(graph, seedLoc, 0);

	seedLoc = vector2D(DIMENSIONS.x - 1, DIMENSIONS.y - 1);
	seedMap(graph, seedLoc, 3);
	seedLoc = vector2D(DIMENSIONS.x - 3, DIMENSIONS.y - 4);
	seedMap(graph, seedLoc, 2);

	// make map
	generateMap(graph);

	// cleanup
	cleanup(graph);

	std::cout << "\nDone.\n";

	return 0;
}
