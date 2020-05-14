//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <vector>
#include "UIScreen.h"
#include "Game.h"
#include "TargetComponent.h"


class HUD : public UIScreen
{
public:
	//Note: Lower draw order corresponds with further back
	HUD(class Game* game);
	~HUD();
	void update(float deltaTime) override;
	void draw(class Shader* shader) override;
	void addTargetComponent(class TargetComponent* tc);
	void removeTargetComponent(class TargetComponent* tc);
protected:
	void updateCrosshair();
	void updateRadar();
	class Texture* healthBar_;
	class Texture* radar_;
	class Texture* crosshair_;
	class Texture* crosshairEnemy_;
	class Texture* blipTex_;
	class Texture* radarArrow_;
	//All the target components in the game
	std::vector<class TargetComponent*> targetComps_;
	//2D offsets of blips relative to radar
	std::vector<Vector2> blips_;
	//Adjust range of radar and radius
	float radarRange_;
	float radarRadius_;
	//Whether the crosshair targets an enemy
	bool targetEnemy_;
};