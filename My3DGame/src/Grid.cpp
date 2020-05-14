//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(class Game* game):Actor(game), selectedTile_(nullptr)
{
	//7 rows, 16 columns
	tiles_.resize(NumRows);
	for(size_t i = 0; i < tiles_.size(); i++)
	{
		tiles_[i].resize(NumCols);
	}
	//Create tiles
	for(size_t i = 0; i < NumRows; i++)
	{
		for(size_t j = 0; j < NumCols; j++)
		{
			tiles_[i][j] = new Tile(game);
			tiles_[i][j]->setPosition(Vector3(TileSize/2.0f + j * TileSize, StartY + i * TileSize, 0));
		}
	}
	//Set start/end tiles
	getStartTile()->setTileState(Tile::EStart);
	getEndTile()->setTileState(Tile::EBase);
	//Set up adjacency lists
	for(size_t i = 0; i < NumRows; i++)
	{
		for(size_t j = 0; j < NumCols; j++)
		{
			if(i > 0)
			{
				tiles_[i][j]->adjacent_.push_back(tiles_[i-1][j]);
			}
			if(i < NumRows - 1)
			{
				tiles_[i][j]->adjacent_.push_back(tiles_[i+1][j]);
			}
			if(j > 0)
			{
				tiles_[i][j]->adjacent_.push_back(tiles_[i][j-1]);
			}
			if(j < NumCols - 1)
			{
				tiles_[i][j]->adjacent_.push_back(tiles_[i][j+1]);
			}
		}
	}
	//Find path (in reverse)
	findPath(getEndTile(), getStartTile());
	updatePathTiles(getStartTile());
	nextEnemy_ = EnemyTime;
}
void Grid::selectTile(size_t row, size_t col)
{
	//Make sure it's a valid selection
	Tile::TileState tstate = tiles_[row][col]->tileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		//Deselect previous one
		if(selectedTile_)
		{
			selectedTile_->toggleSelect();
		}
		selectedTile_ = tiles_[row][col];
		selectedTile_->toggleSelect();
	}
}
void Grid::processClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			selectTile(y, x);
		}
	}
}
//Implements A* pathfinding
bool Grid::findPath(Tile* start, Tile* goal)
{
	for(size_t i = 0; i < NumRows; i++)
	{
		for(size_t j = 0; j < NumCols; j++)
		{
			tiles_[i][j]->g_ = 0.0f;
			tiles_[i][j]->inOpenSet_ = false;
			tiles_[i][j]->inClosedSet_ = false;
		}
	}
	std::vector<Tile*> openSet;
	//Set current node to start, and add to closed set
	Tile* current = start;
	current->inClosedSet_ = true;
	do
	{
		//Add adjacent nodes to open set
		for(Tile* neighbor : current->adjacent_)
		{
			if(neighbor->blocked_)
			{
				continue;
			}
			//Only check nodes that aren't in the closed set
			if(!neighbor->inClosedSet_)
			{
				if(!neighbor->inOpenSet_)
				{
					//Not in the open set, so set parent
					neighbor->parent_ = current;
					neighbor->h_ = (neighbor->position() - goal->position()).length();
					//g(x) is the parent's g plus cost of traversing edge
					neighbor->g_ = current->g_ + TileSize;
					neighbor->f_ = neighbor->g_ + neighbor->h_;
					openSet.emplace_back(neighbor);
					neighbor->inOpenSet_ = true;
				}
				else
				{
					//Compute g(x) cost if current becomes the parent
					float newG = current->g_ + TileSize;
					if(newG < neighbor->g_)
					{
						//Adopt this node
						neighbor->parent_ = current;
						neighbor->g_ = newG;
						//f(x) changes because g(x) changes
						neighbor->f_ = neighbor->g_ + neighbor->h_;
					}
				}
			}
		}
		//If open set is empty, all possible paths are exhausted
		if(openSet.empty())
		{
			break;
		}
		//Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(), [](Tile* a, Tile* b) {
			return a->f_ < b->f_;
		});
		//Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		current->inOpenSet_ = false;
		current->inClosedSet_ = true;
	}
	while(current != goal);
	//Did we find a path?
	return (current == goal) ? true:false;
}
void Grid::updatePathTiles(class Tile* start)
{
	//Reset all tiles to normal (except for start/end)
	for(size_t i = 0; i < NumRows; i++)
	{
		for(size_t j = 0; j < NumCols; j++)
		{
			if(!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				tiles_[i][j]->setTileState(Tile::EDefault);
			}
		}
	}
	Tile* t = start->parent_;
	while(t != getEndTile())
	{
		t->setTileState(Tile::EPath);
		t = t->parent_;
	}
}
void Grid::buildTower()
{
	if(selectedTile_ && !selectedTile_->blocked_)
	{
		selectedTile_->blocked_ = true;
		if(findPath(getEndTile(), getStartTile()))
		{
			Tower* t = new Tower(game());
			t->setPosition(selectedTile_->position());
		}
		else
		{
			//This tower would block the path, so don't allow build
			selectedTile_->blocked_ = false;
			findPath(getEndTile(), getStartTile());
		}
		updatePathTiles(getStartTile());
	}
}
Tile* Grid::getStartTile()
{
	return tiles_[3][0];
}
Tile* Grid::getEndTile()
{
	return tiles_[3][15];
}
void Grid::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	//Is it time to spawn a new enemy?
	nextEnemy_ -= deltaTime;
	if(nextEnemy_ <= 0.0f)
	{
		new Enemy(game());
		nextEnemy_ += EnemyTime;
	}
}