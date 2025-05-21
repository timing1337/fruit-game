#include "main_menu.h"

MainMenu::MainMenu() : BaseScene(SceneId::MAIN_MENU) {
	float mainTitlePosX = RENDERER_CENTER_X - 130;
	TextElement* letterF = this->AddText("main_title_f", vec2_t{ mainTitlePosX, 100 }, "F", "SuperMario256", 40, { 252, 83, 53, 255 }, 2);
	mainTitlePosX += letterF->bound.x + 2;

	TextElement* letterR = this->AddText("main_title_r", vec2_t{ mainTitlePosX, 100 }, "R", "SuperMario256", 40, { 255, 193, 36, 255 }, 2);
	mainTitlePosX += letterR->bound.x + 2;

	TextElement* letterU = this->AddText("main_title_u", vec2_t{ mainTitlePosX, 100 }, "U", "SuperMario256", 40, { 151, 247, 114, 255 }, 2);
	mainTitlePosX += letterU->bound.x + 2;

	TextElement* letterI = this->AddText("main_title_i_1", vec2_t{ mainTitlePosX, 100 }, "I", "SuperMario256", 40, { 240, 107, 255, 255 }, 2);
	mainTitlePosX += letterI->bound.x + 2;

	TextElement* letterT = this->AddText("main_title_t", vec2_t{ mainTitlePosX, 100 }, "T", "SuperMario256", 40, { 255, 238, 107, 255 }, 2);
	mainTitlePosX += letterT->bound.x + 2;

	TextElement* letterN = this->AddText("main_title_n", vec2_t{ mainTitlePosX, 100 }, "N", "SuperMario256", 40, { 232, 229, 209, 255 }, 2);
	mainTitlePosX += letterN->bound.x + 2;
	TextElement* letterI_2 = this->AddText("main_title_i_2", vec2_t{ mainTitlePosX, 100 }, "I", "SuperMario256", 40, { 232, 229, 209, 255 }, 2);
	mainTitlePosX += letterI_2->bound.x + 2;
	TextElement* letterN_2 = this->AddText("main_title_n_2", vec2_t{ mainTitlePosX, 100 }, "N", "SuperMario256", 40, { 232, 229, 209, 255 }, 2);
	mainTitlePosX += letterN_2->bound.x + 2;
	TextElement* letterJ = this->AddText("main_title_j", vec2_t{ mainTitlePosX, 100 }, "J", "SuperMario256", 40, { 232, 229, 209, 255 }, 2);
	mainTitlePosX += letterJ->bound.x + 2;
	TextElement* letterA = this->AddText("main_title_a", vec2_t{ mainTitlePosX, 100 }, "A", "SuperMario256", 40, { 232, 229, 209, 255 }, 2);
	mainTitlePosX += letterA->bound.x + 2;

	this->AddButton("start_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y - 60 }, "Start", "genshin", 25, { 245, 236, 233, 255 }, [this](ButtonElement* button) {
		OnStart(button);
	}, 0, {0, 0, 0, 255}, 10);

	this->AddButton("cosmetics_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y }, "Cosmetics", "genshin", 24, { 245, 236, 233, 255 }, [this](ButtonElement* button) {
		OnCosmetic(button);
	}, 0, { 0, 0, 0, 255 }, 10);

	this->AddButton("settings_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y + 60 }, "Settings", "genshin", 24, { 245, 236, 233, 255 }, [this](ButtonElement* button) {
		OnSettings(button);
		}, 0, { 0, 0, 0, 255 }, 10);

	this->AddButton("quit_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y + 120 }, "Quit", "genshin", 24, { 245, 236, 233, 255 }, [this](ButtonElement* button) {
		OnQuit(button);
	}, 0, { 0, 0, 0, 255 }, 10);
}

void MainMenu::OnCosmetic(ButtonElement* button) {
	SceneManager* scene_mgr = SceneManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();
	SceneManager::GetInstance()->TransitionToScene(SceneId::COSMETIC);
}

void MainMenu::OnSettings(ButtonElement* button) {
	SceneManager* scene_mgr = SceneManager::GetInstance();
	scene_mgr->TransitionToScene(SceneId::SETTING);
}

void MainMenu::OnQuit(ButtonElement* button) {
	GameManager* game_mgr = GameManager::GetInstance();
	game_mgr->running = false;
}

void MainMenu::OnStart(ButtonElement* button) {
	Renderer* renderer = Renderer::GetInstance();
	GameManager* game_mgr = GameManager::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();

	game_mgr->FireStateChange(GameState::STARTING);

	scene_mgr->TransitionToScene(SceneId::GAME, [game_mgr](SceneManager* scene_mgr) {
		game_mgr->FireStateChange(GameState::RUNNING);
	});
}