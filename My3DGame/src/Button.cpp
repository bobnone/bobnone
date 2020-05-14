#include "Button.h"
#include "Texture.h"
#include "Font.h"

Button::Button(const std::string& name, Game* game, Font* font, std::function<void()> onClick, const Vector2& pos):f_onClick_(onClick), texture_(nullptr), font_(font), position_(pos), state_(BUTTON_NORMAL)
{
	setName(name);
	buttonNormal_ = game->renderer()->getTexture("Assets/ButtonNormal.png");
	buttonHover_ = game->renderer()->getTexture("Assets/ButtonHover.png");
	buttonClicked_ = game->renderer()->getTexture("Assets/ButtonClicked.png");
	dimensions_ = Vector2(static_cast<float>(buttonNormal_->getWidth()), static_cast<float>(buttonNormal_->getHeight()));
}
Button::~Button()
{
	if(texture_)
	{
		texture_->~Texture();
		delete texture_;
		texture_ = nullptr;
	}
	//Note: mButtonNormal, mButtonHover, and mButtonClicked are pointers to Textures that get deleted by Renderer
}
void Button::processInput(const Vector2& mousePos)
{
	if(containsPoint(mousePos))
	{
		if(state_ != BUTTON_CLICKED)
		{
			state_ = BUTTON_HOVER;
		}
	}
	else
	{
		state_ = BUTTON_NORMAL;
	}
}
bool Button::containsPoint(const Vector2& pt) const
{
	return !(pt.x < (position_.x - dimensions_.x / 2.0f) || pt.x >(position_.x + dimensions_.x / 2.0f) || pt.y < (position_.y - dimensions_.y / 2.0f) || pt.y >(position_.y + dimensions_.y / 2.0f));
}
Texture* Button::getButtonTexure()
{
	switch(state_)
	{
	case BUTTON_HOVER:
		return buttonHover_;
	case BUTTON_CLICKED:
		return buttonClicked_;
	default:
		return buttonNormal_;
	}
}
void Button::update(float deltaTime)
{
	//EMPTY:
}
void Button::draw(class Shader* shader)
{
	drawTexture(shader, getButtonTexure());
	drawTexture(shader, texture_);
}
void Button::drawTexture(class Shader* shader, class Texture* texture)
{
	Matrix4x4 scaleMat = Math::createScaleMatrix(static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()), 1.0f);
	//Translate to position on screen
	Matrix4x4 transMat = Math::createTranslationMatrix(Vector3(position_, 0.0f));
	//Set world transform
	Matrix4x4 world = scaleMat * transMat;
	shader->setMatrixUniform("uWorldTransform", world);
	//Set current texture
	texture->setActive();
	//Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
void Button::onClick()
{
	//Call attached handler, if it exists
	if(f_onClick_)
	{
		state_ = BUTTON_CLICKED;
		f_onClick_();
	}
}
void Button::setName(const std::string& name)
{
	name_ = name;
	if(texture_)
	{
		texture_->~Texture();
		delete texture_;
		texture_ = nullptr;
	}
	texture_ = font_->renderText(name_);
}