//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(class Game* game):Actor(game), parent_(nullptr), f_(0.0f), g_(0.0f), h_(0.0f), blocked_(false), sprite_(nullptr), tileState_(EDefault), selected_(false)
{
	sprite_ = new SpriteComponent(this);
	updateTexture();
}
void Tile::setTileState(TileState state)
{
	tileState_ = state;
	updateTexture();
}
void Tile::toggleSelect()
{
	selected_ = !selected_;
	updateTexture();
}
void Tile::updateTexture()
{
	std::string text;
	switch(tileState_)
	{
		case EStart:
			text = "Assets/TileTan.png";
			break;
		case EBase:
			text = "Assets/TileGreen.png";
			break;
		case EPath:
			if(selected_)
				text = "Assets/TileGreySelected.png";
			else
				text = "Assets/TileGrey.png";
			break;
		case EDefault:
		default:
			if(selected_)
				text = "Assets/TileBrownSelected.png";
			else
				text = "Assets/TileBrown.png";
			break;
	}
	sprite_->setTexture(game()->renderer()->getTexture(text));
}