#include "pause_screen.h"

PauseScreen::PauseScreen() : BaseScene(SceneId::PAUSE) {
	TextElement* pausedTitle = this->AddText("pause_title", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y - 80 }, "PAUSED", "genshin", 40, { 255, 255, 255, 255 });
	pausedTitle->SetAlignment(Alignment::CENTER);

	this->AddButton("settings_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y + 20 }, "Settings", "genshin", 20, { 245, 236, 233, 255 }, []() {

		}, 0, { 0, 0, 0, 255 }, 10);

	this->AddButton("quit_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y + 60 }, "Back to menu", "genshin", 20, { 245,236,233,255 }, [this]() {
		OnQuit();
		}, 0, { 0,0,0,255 }, 10);
}

void PauseScreen::Render() {
	SDL_SetRenderDrawColor(Renderer::GetInstance()->gRenderer, 0, 0, 0, 70);
	SDL_Rect fillRect = { 0, 0, RENDERER_WIDTH, RENDERER_HEIGHT };
	SDL_RenderFillRect(Renderer::GetInstance()->gRenderer, &fillRect);

	BaseScene::Render();
}

void PauseScreen::OnQuit() {
	GameManager* game_mgr = GameManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();
	game_mgr->ResetGameData();
	//Dont need to fire postgame/saving data process here

	renderer->PlayFadeTransition(
		[this, scene_mgr](TimerTask* self) {
			this->SetActive(false);
			scene_mgr->GetScene(SceneId::GAME)->SetActive(false);
			scene_mgr->GetScene(SceneId::MAIN_MENU)->SetActive(true);
		},
		[game_mgr](TimerTask* self) {
			game_mgr->FireStateChange(GameState::ENDGAME); //cleanup
			game_mgr->FireStateChange(GameState::WAITING); //reset back to default state
		});
}