#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

// vec2
struct vector2D
{
public:
	int x,y;
	
	vector2D() { x=0; y=0; }
	vector2D(int xDir, int yDir) { x=xDir; y=yDir; }
	
	vector2D& operator=(vector2D& other)
	{
		x=other.x;
		y=other.y;
		
		return *this;
	}
	friend vector2D operator+(vector2D l, const vector2D& r)
	{
		vector2D v(l.x + r.x,l.y + r.y);
		return v;
	}
	bool operator==(vector2D r)
	{
		if(x==r.x && y==r.y)
			return true;
		else
			return false;
	}
};

/* - - GLOBALS - - */
// Dimensions
const vector2D DIMENSIONS = vector2D(20,20);
// Directions
const vector2D DIR_TOP		= vector2D( 0,  1);
const vector2D DIR_BOT		= vector2D( 0, -1);
const vector2D DIR_LEFT		= vector2D(-1,  0);
const vector2D DIR_RIGHT	= vector2D( 1,  0);
// Area types
const int typeCt = 4;
const string TILE_FOREST= "0001";
const string TILE_LAND 	= "0010";
const string TILE_COAST = "0100";
const string TILE_SEA 	= "1000";
const string TILE_FULL	= "1111";
const string TILE_EMPTY	= "0000";
// Tile weights
const float WEIGHT_FOREST	= 1.1;
const float WEIGHT_LAND		= 1.5;
const float WEIGHT_COAST	= 1.0;
const float WEIGHT_SEA		= 1.5;

float Weight_Total = WEIGHT_FOREST + WEIGHT_LAND + WEIGHT_COAST + WEIGHT_SEA;

// tile              	
struct tile          		
{
public:
	bool collapsed;
	string options;
	
	tile() {collapsed = false;options = TILE_FULL;}
};


// prototypes
void init(vector<tile*>&);
void generateMap(vector<tile*>&);
void displayMap(vector<tile*>&);
void collapse(vector<tile*>&, vector2D);
void ripple(vector<tile*>&, vector2D);
bool conflicts(tile*, tile*);
bool rippledOut(vector<tile*>&);

int vec2dToIndex(vector2D v)
{
	return v.x + (DIMENSIONS.x * v.y);
}
void indexToVec2d(int i, vector2D& v)
{
	int x=0, y=0;
	int temp = i;
	
	if (temp>=DIMENSIONS.x)
	while (true)
	{
		temp -= DIMENSIONS.x;
		if(temp>0)
			y++;
		else
			break;
	}
	x = i-(y*DIMENSIONS.x);
	
	v.x = x;
	v.y = y;
}

void cleanup(vector<tile*>& gr)
{
	for(int i =0;i<gr.size();i++)
		delete gr[i];
	
	gr.clear();
}

int main()
{
	// init rng
	srand(time(0));

	// init map
	vector<tile*> graph;
	init(graph);

	// make map
	generateMap(graph);

	// cleanup
	cleanup(graph);

	return 0;
}

void init(vector<tile*>& gr)
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

int findLowestEntropy(vector<tile*>& gr)
{
	int index = -1, min = typeCt+1;

	for(int i=0; i<gr.size(); i++)
	{
	  // skip if collapsed
	  if(gr[i]->collapsed)
    	continue;
		
		int entropyCt = 0;
		// go over everything in the string
		for(int j=0;j<gr[i]->options.size();j++)
		{
			if(gr[i]->options[j]=='1')
				entropyCt++;
		}
		//cout << "index " << index << " has " << entropyCt << " entropy\n";
		if(entropyCt<min)
		{
			index=i;
			min=entropyCt;
		}
	}

	//cout << "lowest: " << index << endl;
	return index;
}

void generateMap(vector<tile*>& gr)
{
	int index = findLowestEntropy(gr);
	while(index!=-1)
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

void displayMap(vector<tile*>& gr)
{
	for (int i = 0; i < DIMENSIONS.y; i++)
	{
		for (int j = 0; j < DIMENSIONS.x; j++)
		{
			if (gr[i*DIMENSIONS.x+j]->options == TILE_LAND)
				cout << "\033[0;32mL \033[0m";
			else if (gr[i*DIMENSIONS.x+j]->options == TILE_SEA)
				cout << "\033[0;34mS \033[0m";
			else if (gr[i*DIMENSIONS.x+j]->options == TILE_COAST)
				cout << "\033[0;33mC \033[0m";
			else if (gr[i*DIMENSIONS.x+j]->options == TILE_FOREST)
				cout << "\033[0;35mF \033[0m";
			else
				cout << "-1";
		}
		cout << endl;
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

void collapse(vector<tile*>& gr, vector2D loc)
{
	bool collapsed = false;
	
	int index = vec2dToIndex(loc);

	while(!collapsed)
	{
		int r;// = rand()%typeCt;

		float t = (float)(rand()%100)/100.0;
		
		if (t <= WEIGHT_FOREST / Weight_Total)
		{
			r = 0;
		}
		else if (t <= (WEIGHT_FOREST + WEIGHT_LAND) / Weight_Total)
		{
			r = 1;
		}
		else if (t <= (WEIGHT_FOREST + WEIGHT_LAND + WEIGHT_COAST) / Weight_Total)
		{
			r = 2;
		}
		else
		{
			r=3;
		}

		if (gr[index]->options[r]=='1')
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

void ripple(vector<tile*>& gr, vector2D center)
{
	int index = vec2dToIndex(center);
	// remove options incompatible with TILE_LAND
	if (gr[index]->options==TILE_LAND)
	{
		// Ripple DIR_LEFT
		if (center.x-1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x-1,center.y))]->options[0] = '0';
		}	
		// ripple DIR_RIGHT
		if (center.x+1 < DIMENSIONS.x)
		{
			gr[vec2dToIndex(vector2D(center.x+1,center.y))]->options[0] = '0';
		}
		if (center.y-1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x,center.y-1))]->options[0] = '0';
		}
		if (center.y+1 < DIMENSIONS.y)
		{
			gr[vec2dToIndex(vector2D(center.x,center.y+1))]->options[0] = '0';
		}
	}
	
	// remove options incompatible with TILE_SEA
	if (gr[index]->options==TILE_SEA)
	{
		// Ripple DIR_LEFT
		if (center.x-1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x-1,center.y))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x-1,center.y))]->options[3] = '0';
		}	
		// ripple DIR_RIGHT
		if (center.x+1 < DIMENSIONS.x)
		{
			gr[vec2dToIndex(vector2D(center.x+1,center.y))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x+1,center.y))]->options[3] = '0';
		}
		if (center.y-1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x,center.y-1))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x,center.y-1))]->options[3] = '0';
		}
		if (center.y+1 < DIMENSIONS.y)
		{
			gr[vec2dToIndex(vector2D(center.x,center.y+1))]->options[2] = '0';
			gr[vec2dToIndex(vector2D(center.x,center.y+1))]->options[3] = '0';
		}
	}
	
	// remove options incompatible with TILE_FOREST
	if (gr[index]->options==TILE_FOREST)
	{
		// Ripple DIR_LEFT
		if (center.x-1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x-1,center.y))]->options[0] = '0';
		}	
		// ripple DIR_RIGHT
		if (center.x+1 < DIMENSIONS.x)
		{
			gr[vec2dToIndex(vector2D(center.x+1,center.y))]->options[0] = '0';
		}
		if (center.y-1 >= 0)
		{
			gr[vec2dToIndex(vector2D(center.x,center.y-1))]->options[0] = '0';
		}
		if (center.y+1 < DIMENSIONS.y)
		{
			gr[vec2dToIndex(vector2D(center.x,center.y+1))]->options[0] = '0';
		}
	}
}

// make sure all options arent gone in a tile
bool rippledOut(vector<tile*>& gr)
{
	for (int i=0; i<gr.size();i++)
	if (gr[i]->options==TILE_EMPTY)
	{
		cout << "\nERROR: WE DON MESSED UP\n	map["<<i
			<<"] = " << TILE_EMPTY << endl;
		return true;
	}
	return false;
}