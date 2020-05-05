// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "HUD.h"
#include "FollowActor.h"

HUD::HUD(Game* game) :UIScreen(game), mRadarRange(2000.0f), mRadarRadius(92.0f), mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBar = r->GetTexture("Assets/HealthBar.png");
	mRadar = r->GetTexture("Assets/Radar.png");
	mCrosshair = r->GetTexture("Assets/Crosshair.png");
	mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	mBlipTex = r->GetTexture("Assets/Blip.png");
	mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}
HUD::~HUD()
{
	//
}
void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);
	UpdateCrosshair();
	UpdateRadar();
}
void HUD::Draw(Shader* shader)
{
	// Crosshair
	Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
	DrawTexture(shader, cross, vector2::Zero, 2.0f);
	// Radar
	const vector2 cRadarPos(-390.0f, 275.0f);
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);
	// Blips
	for (vector2& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}
	// Radar arrow
	DrawTexture(shader, mRadarArrow, cRadarPos);
	//// Health bar
	DrawTexture(shader, mHealthBar, vector2(-350.0f, -350.0f));
	// Draw the mirror (bottom left)
	Texture* mirror = mGame->GetRenderer()->GetMirrorTexture();
	//DrawTexture(shader, mirror, Vector2(-350.0f, -250.0f), 1.0f, true);
	Texture* tex = mGame->GetRenderer()->GetGBuffer()->GetTexture(GBuffer::GBUFFER_DIFFUSE);
	//DrawTexture(shader, tex, Vector2::Zero, 1.0f, true);
}
void HUD::AddTargetComponent(TargetComponent * tc)
{
	mTargetComps.emplace_back(tc);
}
void HUD::RemoveTargetComponent(TargetComponent * tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(), tc);
	mTargetComps.erase(iter);
}
void HUD::UpdateCrosshair()
{
	// Reset to regular cursor
	mTargetEnemy = false;
	// Make a line segment
	const float cAimDist = 5000.0f;
	vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	// Segment cast
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// Is this a target?
		for (auto tc : mTargetComps)
		{
			if (tc->GetOwner() == info.mActor)
			{
				mTargetEnemy = true;
				break;
			}
		}
	}
}
void HUD::UpdateRadar()
{
	// Clear blip positions from last frame
	mBlips.clear();
	// Convert player position to radar coordinates (x forward, z up)
	vector3 playerPos = mGame->GetPlayer()->GetPosition();
	vector2 playerPos2D(playerPos.y, playerPos.x);
	// Ditto for player forward
	vector3 playerForward = mGame->GetPlayer()->GetForward();
	vector2 playerForward2D(playerForward.x, playerForward.y);
	// Use atan2 to get rotation of radar
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// Make a 2D rotation matrix
	matrix3 rotMat = matrix3::CreateRotation(angle);
	// Get positions of blips
	for (auto tc : mTargetComps)
	{
		vector3 targetPos = tc->GetOwner()->GetPosition();
		vector2 actorPos2D(targetPos.y, targetPos.x);
		// Calculate vector between player and target
		vector2 playerToTarget = actorPos2D - playerPos2D;
		// See if within range
		if (playerToTarget.Length2() <= (mRadarRange * mRadarRange))
		{
			// Convert playerToTarget into an offset from
			// the center of the on-screen radar
			vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;
			// Rotate blipPos
			blipPos = vector2::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}
}