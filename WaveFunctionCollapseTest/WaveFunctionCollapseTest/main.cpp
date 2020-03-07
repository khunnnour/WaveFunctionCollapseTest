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

	// make map
	generateMap(graph);

	// cleanup
	cleanup(graph);

	std::cout << "\nDone.\n";

	return 0;
}
