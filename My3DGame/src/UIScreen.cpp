#include "Game.h"
#include "UIScreen.h"
#include "Renderer.h"

UIScreen::UIScreen(Game* game):mGame(game), mTitle(nullptr), mBackground(nullptr), mTitlePos(0.0f, 300.0f), mNextButtonPos(0.0f, 200.0f), mBGPos(0.0f, 250.0f), mState(UI_ACTIVE)
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
		// Highlight any buttons
		for (auto b : mButtons)
		{
			b->ProcessInput(mGame->GetMouse()->GetPosition());
			if (mGame->GetMouse()->Clicked() && b->IsHighlighted())
			{
				b->OnClick();
			}
		}
	}
}
void UIScreen::HandleKeyPress(int key)
{
	//
}
void UIScreen::Close()
{
	mState = UI_CLOSING;
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
void UIScreen::DrawTexture(Shader* shader, Texture* texture, const vector2& offset, float scale, bool flipY)
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