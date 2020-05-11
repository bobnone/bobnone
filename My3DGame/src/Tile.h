//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include <vector>

class Tile:public Actor
{
public:
	friend class Grid;
	enum TileState
	{
		EDefault,
		EPath,
		EStart,
		EBase
	};
	Tile(class Game* game);
	void setTileState(TileState state);
	TileState tileState() const
	{
		return tileState_;
	}
	void toggleSelect();
	const Tile* parent() const
	{
		return parent_;
	}
private:
	//For pathfinding
	std::vector<Tile*> adjacent_;
	Tile* parent_;
	float f_;
	float g_;
	float h_;
	bool inOpenSet_;
	bool inClosedSet_;
	bool blocked_;
	void updateTexture();
	class SpriteComponent* sprite_;
	TileState tileState_;
	bool selected_;
};