// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "Texture.h"
#include "Renderer.h"
#include "Font.h"

UIScreen::UIScreen(Game* game):mGame(game), mTitle(nullptr), mBackground(nullptr), mTitlePos(0.0f, 300.0f), mNextButtonPos(0.0f, 200.0f), mBGPos(0.0f, 250.0f), mState(EActive)
{
	// Add to UI Stack
	mGame->PushUI(this);
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
}
UIScreen::~UIScreen()
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}
	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
}
void UIScreen::Update(float deltaTime)
{
	for (auto b : mButtons)
	{
		b->Update(deltaTime);
	}
}
void UIScreen::Draw(Shader* shader)
{
	// Draw background (if exists)
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// Draw title (if exists)
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}
	// Draw buttons
	for (auto b : mButtons)
	{
		b->Draw(shader);
	}
	// Override in subclasses to draw any textures
}
void UIScreen::ProcessInput(const uint8_t* keys)
{
	// Do we have buttons?
	if (!mButtons.empty())
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
		// Highlight any buttons
		for (auto b : mButtons)
		{
			b->ProcessInput(mousePos);
		}
	}
}
void UIScreen::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty())
		{
			for (auto b : mButtons)
			{
				if(b->IsHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}
void UIScreen::Close()
{
	mState = EClosing;
}
void UIScreen::SetTitle(const std::string& text, const vector3& color, int pointSize)
{
	// Clear out previous title texture if it exists
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	mTitle = mFont->RenderText(text, color, pointSize);
}
void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Button* b = new Button(name, mGame, mFont, onClick, mNextButtonPos);
	mButtons.emplace_back(b);
	// Update position of next button
	// Move down by height of button plus padding
	mNextButtonPos.y -= b->GetDimensions().y + 20.0f;
}
void UIScreen::DrawTexture(class Shader* shader, class Texture* texture, const vector2& offset, float scale, bool flipY)
{
	// Scale the quad by the width/height of texture
	// and flip the y if we need to
	float yScale = static_cast<float>(texture->GetHeight()) * scale;
	if (flipY)
	{
		yScale *= -1.0f;
	}
	matrix4 scaleMat = matrix4::CreateScale(static_cast<float>(texture->GetWidth()) * scale, yScale, 1.0f);
	// Translate to position on screen
	matrix4 transMat = matrix4::CreateTranslation(vector3(offset.x, offset.y, 0.0f));
	// Set world transform
	matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// Make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}