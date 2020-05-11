#include "Game.h"
#include "UIScreen.h"
#include "Renderer.h"

UIScreen::UIScreen(Game* game):game_(game), title_(nullptr), background_(nullptr), titlePos_(0.0f, 300.0f), nextButtonPos_(0.0f, 200.0f), bGPos_(0.0f, 250.0f), state_(UI_ACTIVE)
{
	//Add to UI Stack
	game_->pushUI(this);
	font_ = game_->getFont("Assets/Carlito-Regular.ttf");
}
UIScreen::~UIScreen()
{
	if(title_)
	{
		title_->~Texture();
		delete title_;
	}
	for(auto b : buttons_)
	{
		delete b;
	}
	buttons_.clear();
}
void UIScreen::update(float deltaTime)
{
	for(auto b: buttons_)
	{
		b->update(deltaTime);
	}
}
void UIScreen::draw(Shader* shader)
{
	//Draw background (if exists)
	if(background_)
	{
		drawTexture(shader, background_, bGPos_);
	}
	//Draw title (if exists)
	if(title_)
	{
		drawTexture(shader, title_, titlePos_);
	}
	//Draw buttons
	for(auto b : buttons_)
	{
		b->draw(shader);
	}
	//Override in subclasses to draw any textures
}
void UIScreen::processInput(const uint8_t* keys)
{
	//Do we have buttons?
	if(!buttons_.empty())
	{
		//Highlight any buttons
		for(auto b: buttons_)
		{
			b->processInput(game_->mouse()->position());
			if(game_->mouse()->clicked() && b->isHighlighted())
			{
				b->onClick();
			}
		}
	}
}
void UIScreen::handleKeyPress(int key)
{
	//EMPTY:
}
void UIScreen::close()
{
	state_ = UI_CLOSING;
}
void UIScreen::setTitle(const std::string& text, const vector3& color, int pointSize)
{
	//Clear out previous title texture if it exists
	if(title_)
	{
		title_->~Texture();
		delete title_;
		title_ = nullptr;
	}
	title_ = font_->renderText(text, color, pointSize);
}
void UIScreen::addButton(const std::string& name, std::function<void()> onClick)
{
	Button* b = new Button(name, game_, font_, onClick, nextButtonPos_);
	buttons_.emplace_back(b);
	//Update position of next button
	//Move down by height of button plus padding
	nextButtonPos_.y -= b->dimensions().y + 20.0f;
}
void UIScreen::drawTexture(Shader* shader, Texture* texture, const vector2& offset, float scale, bool flipY)
{
	//Scale the quad by the width/height of texture
	//and flip the y if we need to
	float yScale = static_cast<float>(texture->getHeight()) * scale;
	if(flipY)
	{
		yScale *= -1.0f;
	}
	matrix4 scaleMat = matrix4::CreateScale(static_cast<float>(texture->getWidth()) * scale, yScale, 1.0f);
	//Translate to position on screen
	matrix4 transMat = matrix4::CreateTranslation(vector3(offset.x, offset.y, 0.0f));
	//Set world transform
	matrix4 world = scaleMat * transMat;
	shader->setMatrixUniform("uWorldTransform", world);
	//Set current texture
	texture->setActive();
	//Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}