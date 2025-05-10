#include "end_stage.h"

EndStage::EndStage() : BaseScene(SceneId::END_GAME)
{
	TextElement* title = this->AddText("end_title", vec2_t{ RENDERER_CENTER_X, 200 }, "Game Over", "genshin", 30, { 255,255, 255, 200 });
	title->SetAlignment(Alignment::CENTER);
	title->SetAlignmentVertical(AlignmentVertical::TOP);

	TextElement* stats = this->AddText("end_stats", vec2_t{ RENDERER_CENTER_X, 260 }, "Stats", "genshin", 20, { 255, 255, 204, 200 });
	stats->SetAlignment(Alignment::CENTER);

	TextElement* highestScore = this->AddText("score", vec2_t{ RENDERER_CENTER_X - 250, 330 }, "Score", "genshin", 20, {255, 255, 255, 200});
	TextElement* maxComboAchieved = this->AddText("max_combo_achieved", vec2_t{ RENDERER_CENTER_X - 250, 380 }, "Max combo achieved", "genshin", 20, { 255, 255, 255, 200 });
	TextElement* amountOfFruitSliced = this->AddText("amount_of_fruit_sliced", vec2_t{ RENDERER_CENTER_X - 250, 430 }, "Amount of fruit sliced", "genshin", 20, { 255, 255, 255, 200 });

	TextElement* highestScoreValue = this->AddText("score_value", vec2_t{ RENDERER_CENTER_X + 250, 330 }, "0", "genshin", 20, { 255, 255, 204, 255 });
	TextElement* maxComboAchievedValue = this->AddText("max_combo_achieved_value", vec2_t{ RENDERER_CENTER_X + 250, 380 }, "0", "genshin", 20, { 255, 255, 204, 255 });
	TextElement* amountOfFruitSlicedValue = this->AddText("amount_of_fruit_sliced_value", vec2_t{ RENDERER_CENTER_X + 250, 430 }, "0", "genshin", 20, { 255, 255, 204, 255 });

	ButtonElement* mainMenuButton = this->AddButton("main_menu_button", vec2_t{ RENDERER_CENTER_X + 250, 620 }, "Return to menu", "genshin", 20, { 255, 255, 255, 255 }, [this](ButtonElement* button) {
		ReturnToMenu(button);
		}, 0, { 0, 0, 0, 255 }, 10);

	ButtonElement* playAgainButton = this->AddButton("play_again_button", vec2_t{ RENDERER_CENTER_X - 250, 620 }, "Play again", "genshin", 20, { 255, 255, 255, 255 }, [this](ButtonElement* button) {
		PlayAgain(button);
	
		}, 0, { 0, 0, 0, 255 }, 10);
}

void EndStage::Prepare() {
	TextElement* score = (TextElement*)this->GetElementById("score_value");	
	TextElement* maxComboAchieved = (TextElement*)this->GetElementById("max_combo_achieved_value");
	TextElement* amountOfFruitSliced = (TextElement*)this->GetElementById("amount_of_fruit_sliced_value");

	score->SetText(to_string(GameManager::GetInstance()->score));
	maxComboAchieved->SetText(to_string(GameManager::GetInstance()->highestComboRecorded));
	amountOfFruitSliced->SetText(to_string(GameManager::GetInstance()->slicedFruit));

	BaseScene::Prepare();
}

void EndStage::PlayAgain(ButtonElement* button) {
	Renderer* renderer = Renderer::GetInstance();
	GameManager* game_mgr = GameManager::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();

	game_mgr->FireStateChange(GameState::STARTING);

	renderer->PlayFadeTransition(
		[this, scene_mgr](TimerTask* self) {
			this->SetActive(false);
			scene_mgr->GetScene(SceneId::GAME)->SetActive(true);
		},
		[button](TimerTask* self) {
			GameManager::GetInstance()->FireStateChange(GameState::RUNNING);
			button->isClicked = false;
		});
}

void EndStage::ReturnToMenu(ButtonElement* button) {
	Renderer* renderer = Renderer::GetInstance();
	GameManager* game_mgr = GameManager::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();

	game_mgr->FireStateChange(GameState::WAITING);

	renderer->PlayFadeTransition(
		[this, scene_mgr](TimerTask* self) {
			this->SetActive(false);
			scene_mgr->GetScene(SceneId::MAIN_MENU)->SetActive(true);
		},
		[button](TimerTask* self) {
			button->isClicked = false;
		});
}

void EndStage::Render() {
	BaseScene::Render();
}