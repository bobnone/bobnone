//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "HUD.h"
#include "FollowActor.h"

HUD::HUD(Game* game):UIScreen(game), radarRange_(2000.0f), radarRadius_(92.0f), targetEnemy_(false)
{
	Renderer* renderer = game_->renderer();
	healthBar_ = renderer->getTexture("Assets/HealthBar.png");
	radar_ = renderer->getTexture("Assets/Radar.png");
	crosshair_ = renderer->getTexture("Assets/Crosshair.png");
	crosshairEnemy_ = renderer->getTexture("Assets/CrosshairRed.png");
	blipTex_ = renderer->getTexture("Assets/Blip.png");
	radarArrow_ = renderer->getTexture("Assets/RadarArrow.png");
}
HUD::~HUD()
{
	//EMPTY:
}
void HUD::update(float deltaTime)
{
	UIScreen::update(deltaTime);
	updateCrosshair();
	updateRadar();
}
void HUD::draw(Shader* shader)
{
	//Crosshair
	Texture* cross = targetEnemy_ ? crosshairEnemy_:crosshair_;
	drawTexture(shader, cross, Vector2(), 2.0f);
	//Radar
	const Vector2 cRadarPos(-390.0f, 275.0f);
	drawTexture(shader, radar_, cRadarPos, 1.0f);
	//Blips
	for(Vector2& blip: blips_)
	{
		drawTexture(shader, blipTex_, cRadarPos + blip, 1.0f);
	}
	//Radar arrow
	drawTexture(shader, radarArrow_, cRadarPos);
	//// Health bar
	drawTexture(shader, healthBar_, Vector2(-350.0f, -350.0f));
	//Draw the mirror (bottom left)
	Texture* mirror = game_->renderer()->mirrorTexture();
	//DrawTexture(shader, mirror, Vector2(-350.0f, -250.0f), 1.0f, true);
	Texture* tex = game_->renderer()->gBuffer()->getTexture(GBuffer::GBUFFER_DIFFUSE);
	//DrawTexture(shader, tex, Vector2::Zero, 1.0f, true);
}
void HUD::addTargetComponent(TargetComponent * tc)
{
	targetComps_.emplace_back(tc);
}
void HUD::removeTargetComponent(TargetComponent * tc)
{
	auto iter = std::find(targetComps_.begin(), targetComps_.end(), tc);
	targetComps_.erase(iter);
}
void HUD::updateCrosshair()
{
	//Reset to regular cursor
	targetEnemy_ = false;
	//Make a line segment
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	game_->renderer()->getScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	//Segment cast
	Physics::CollisionInfo info;
	if(game_->physics()->segmentCast(l, info))
	{
		//Is this a target?
		for(auto tc : targetComps_)
		{
			if(tc->owner() == info.actor_)
			{
				targetEnemy_ = true;
				break;
			}
		}
	}
}
void HUD::updateRadar()
{
	//Clear blip positions from last frame
	blips_.clear();
	//Convert player position to radar coordinates (x forward, z up)
	Vector3 playerPos = game_->player()->position();
	Vector2 playerPos2D(playerPos.y, playerPos.x);
	//Ditto for player forward
	Vector3 playerForward = game_->player()->getForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);
	//Use atan2 to get rotation of radar
	float angle = Math::atan2(playerForward2D.y, playerForward2D.x);
	//Make a 2D rotation matrix
	Matrix3x3 rotMat = Math::createRotationMatrix(angle);
	//Get positions of blips
	for(auto tc : targetComps_)
	{
		Vector3 targetPos = tc->owner()->position();
		Vector2 actorPos2D(targetPos.y, targetPos.x);
		//Calculate vector between player and target
		Vector2 playerToTarget = actorPos2D - playerPos2D;
		//See if within range
		if(playerToTarget.length2() <= (radarRange_ * radarRange_))
		{
			//Convert playerToTarget into an offset from the center of the on-screen radar
			Vector2 blipPos = playerToTarget;
			blipPos *= radarRadius_ / radarRange_;
			//Rotate blipPos
			blipPos = Math::transform(blipPos, rotMat);
			blips_.emplace_back(blipPos);
		}
	}
}