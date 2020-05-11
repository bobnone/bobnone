//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include <vector>

class Grid: public Actor
{
public:
	Grid(class Game* game);
	//Handle a mouse click at the x/y screen locations
	void processClick(int x, int y);
	//Use A* to find a path
	bool findPath(class Tile* start, class Tile* goal);
	//Try to build a tower
	void buildTower();
	//Get start/end tile
	class Tile* getStartTile();
	class Tile* getEndTile();
	void updateActor(float deltaTime) override;
private:
	//Select a specific tile
	void selectTile(size_t row, size_t col);
	//Update textures for tiles on path
	void updatePathTiles(class Tile* start);
	//Currently selected tile
	class Tile* selectedTile_;
	//2D vector of tiles in grid
	std::vector<std::vector<class Tile*>> tiles_;
	//Time until next enemy
	float nextEnemy_;
	//Rows/columns in grid
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	//Start y position of top left corner
	const float StartY = 192.0f;
	//Width/height of each tile
	const float TileSize = 64.0f;
	//Time between enemies
	const float EnemyTime = 1.5f;
};