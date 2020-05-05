#include "PauseMenu.h"
#include <SDL/SDL.h>
#include "DialogBoxMenu.h"

PauseMenu::PauseMenu(Game* game):UIScreen(game)
{
	mGame->SetState(Game::GAME_PAUSED);
	mGame->GetMouse()->SetRelative(false);
	SetTitle("PauseTitle");
	AddButton("ResumeButton", [this](){
		Close();
	});
	AddButton("QuitButton", [this]() {
		new DialogBoxMenu(mGame, "QuitText", [this](){
			mGame->SetState(Game::GAME_QUIT);
		});
	});
}
PauseMenu::~PauseMenu()
{
	mGame->GetMouse()->SetRelative(true);
	mGame->SetState(Game::GAME_PLAY);
}
void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}