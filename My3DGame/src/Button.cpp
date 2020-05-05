// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Button.h"
#include "Texture.h"
#include "Font.h"

Button::Button(const std::string& name, Game* game, Font* font, std::function<void()> onClick, const vector2& pos) :mOnClick(onClick), mTexture(nullptr), mFont(font), mPosition(pos), mState(NORMAL)
{
	SetName(name);
	mButtonNormal = game->GetTexture("Assets/ButtonNormal.png");
	mButtonHover = game->GetTexture("Assets/ButtonHover.png");
	mButtonClicked = game->GetTexture("Assets/ButtonClicked.png");
	mDimensions = vector2(static_cast<float>(mButtonNormal->GetWidth()), static_cast<float>(mButtonNormal->GetHeight()));
}
Button::~Button()
{
	if(mTexture)
	{
		mTexture->Unload();
		delete mTexture;
	}
	// Note: mButtonNormal, mButtonHover, and mButtonClicked are pointers to Textures that get deleted by Renderer
}
void Button::ProcessInput(const vector2& mousePos)
{
	
	if (ContainsPoint(mousePos))
	{
		if (mState != CLICKED)
		{
			mState = HOVER;
		}
	}
	else
	{
		mState = NORMAL;
	}
}
bool Button::ContainsPoint(const vector2& pt) const
{
	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) || pt.x >(mPosition.x + mDimensions.x / 2.0f) || pt.y < (mPosition.y - mDimensions.y / 2.0f) || pt.y >(mPosition.y + mDimensions.y / 2.0f);
	return !no;
}
Texture* Button::GetButtonTexure()
{
	switch (mState)
	{
	case HOVER:
		return mButtonHover;
	case CLICKED:
		return mButtonClicked;
	default:
		return mButtonNormal;
	}
}
void Button::Update(float deltaTime)
{
	//
}
void Button::Draw(class Shader* shader)
{
	DrawTexture(shader, GetButtonTexure());
	DrawTexture(shader, mTexture);
}
void Button::DrawTexture(class Shader* shader, class Texture* texture)
{
	matrix4 scaleMat = matrix4::CreateScale(static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()), 1.0f);
	// Translate to position on screen
	matrix4 transMat = matrix4::CreateTranslation(vector3(mPosition, 0.0f));
	// Set world transform
	matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
void Button::OnClick()
{
	// Call attached handler, if it exists
	if (mOnClick)
	{
		mState = CLICKED;
		mOnClick();
	}
}
void Button::SetName(const std::string& name)
{
	mName = name;
	if (mTexture)
	{
		mTexture->Unload();
		delete mTexture;
		mTexture = nullptr;
	}
	mTexture = mFont->RenderText(mName);
}