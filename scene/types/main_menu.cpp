#include "main_menu.h"

MainMenu::MainMenu() : BaseScene(SceneId::MAIN_MENU) {
	this->AddText("main_title", vec2_t{ RENDERER_CENTER_X, 100 }, "FRUIT NINJA", "Helvetica-Bold", 40, { 255, 255, 255, 255 });

	this->AddButton("start_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y - 60 }, "START", "NoyhR-Light", 25, { 255, 255, 255, 255 }, [this]() {
		OnStart();
	}, 0, {0, 0, 0, 255}, 10);
	this->AddButton("settings_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y }, "SETTINGS", "NoyhR-Light", 24, { 255, 255, 255, 255 }, []() {
		
	}, 0, { 0, 0, 0, 255 }, 10);
	this->AddButton("quit_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y + 60 }, "QUIT", "NoyhR-Light", 24, { 255, 255, 255, 255 }, []() {}, 0, { 0, 0, 0, 255 }, 10);
}

void MainMenu::OnStart() {
	Renderer* renderer = Renderer::GetInstance();
	GameManager* game_mgr = GameManager::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();

	if (game_mgr->state != GameState::WAITING) return;

	game_mgr->FireStateChange(GameState::STARTING);

	renderer->PlayFadeTransition(
		[scene_mgr](TimerTask* self) {
			scene_mgr->GetScene(SceneId::MAIN_MENU)->SetActive(false);
			scene_mgr->GetScene(SceneId::GAME)->SetActive(true);
		},
		[](TimerTask* self) {
			GameManager::GetInstance()->FireStateChange(GameState::RUNNING);
		});
}