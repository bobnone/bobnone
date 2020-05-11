#include "FollowActor.h"
#include<GLM/glm.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "Animation.h"
#include "AudioSystem.h"
#include "JsonHelper.h"

FollowActor::FollowActor(Game* game):Actor(game), animated_(false)
{
	meshComp_ = new SkeletalMeshComponent(this);
	Renderer* renderer = game_->renderer();
	Mesh* mesh = renderer->getMesh("Assets/CatWarrior.gpmesh");
	meshComp_->setMesh(mesh);
	meshComp_->setShader(renderer->getShader("Skinned"));
	meshComp_->setSkeleton(game_->getSkeleton("Assets/CatWarrior.gpskel"));
	meshComp_->playAnimation(game->getAnimation("Assets/CatActionIdle.gpanim"));
	setPosition(vector3(0.0f, -100.0f, 0.0f));
	moveComp_ = new MoveComponent(this);
	cameraComp_ = new FollowCamera(this);
	cameraComp_->snapToIdeal();
	audioComp_ = new AudioComponent(this);
	lastFootstep_ = 0.0f;
	footstep_ = audioComp_->playEvent("event:/Footstep");
	footstep_.setPaused(true);
	// Add a component for the mirror camera
	MirrorCamera* mirror = new MirrorCamera(this);
	mirror->snapToIdeal();
	game_->setPlayer(this);
}
void FollowActor::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	//Play the footstep if we're moving and haven't recently
	lastFootstep_ -= deltaTime;
	if(!Math::NearZero(moveComp_->forwardSpeed()) && lastFootstep_ <= 0.0f)
	{
		footstep_.setPaused(false);
		footstep_.restart();
		lastFootstep_ = 0.5f;
	}
}
void FollowActor::actorInput(const uint8_t* keys)
{
	float angularXSpeed = 0.0f;
	float angularYSpeed = 0.0f;
	float angularZSpeed = 0.0f;
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	float jumpSpeed = 0.0f;
	//wasd movement
	if(keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	else if(keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if(keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400;
	}
	else if(keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400;
	}
	if(keys[SDL_SCANCODE_SPACE])
	{
		jumpSpeed += 400;
	}
	else if(keys[SDL_SCANCODE_Z])//idk why it doesn't like ctrl; modifier key?
	{
		jumpSpeed -= 400;
	}
	if(keys[SDL_SCANCODE_INSERT])
	{
		angularXSpeed -= Math::Pi;
	}
	else if(keys[SDL_SCANCODE_DELETE])
	{
		angularXSpeed += Math::Pi;
	}
	if(keys[SDL_SCANCODE_HOME])
	{
		angularYSpeed -= Math::Pi;
	}
	else if(keys[SDL_SCANCODE_END])
	{
		angularYSpeed += Math::Pi;
	}
	if(keys[SDL_SCANCODE_PAGEUP])
	{
		angularZSpeed -= Math::Pi;
	}
	else if(keys[SDL_SCANCODE_PAGEDOWN])
	{
		angularZSpeed += Math::Pi;
	}
	//Did we just start moving forward?
	if(!animated_ && !Math::NearZero(forwardSpeed))
	{
		animated_ = true;
		meshComp_->playAnimation(game()->getAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	//Or did we just stop moving forward?
	else if(animated_ && Math::NearZero(forwardSpeed))
	{
		animated_ = false;
		meshComp_->playAnimation(game()->getAnimation("Assets/CatActionIdle.gpanim"));
	}
	moveComp_->setSpeed(angularXSpeed, angularYSpeed, angularZSpeed, forwardSpeed, strafeSpeed, jumpSpeed);
}
void FollowActor::setVisible(bool visible)
{
	meshComp_->setVisible(visible);
}
void FollowActor::setFootstepSurface(float value)
{
	//Pause here because the way I setup the parameter in FMOD
	//changing it will play a footstep
	footstep_.setPaused(true);
	footstep_.setParameter("Surface", value);
}