#include "PauseMenu.h"
#include <SDL/SDL.h>
#include "DialogBoxMenu.h"

PauseMenu::PauseMenu(Game* game):UIScreen(game)
{
	game_->setState(Game::GAME_PAUSED);
	game_->mouse()->setRelative(false);
	setTitle("PauseTitle");
	addButton("ResumeButton", [this](){
		close();
	});
	addButton("QuitButton", [this]() {
		new DialogBoxMenu(game_, "QuitText", [this](){
			game_->setState(Game::GAME_QUIT);
		});
	});
}
PauseMenu::~PauseMenu()
{
	game_->mouse()->setRelative(true);
	game_->setState(Game::GAME_PLAY);
}
void PauseMenu::handleKeyPress(int key)
{
	UIScreen::handleKeyPress(key);
	if(key == SDLK_ESCAPE)
	{
		close();
	}
}