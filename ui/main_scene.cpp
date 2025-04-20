#include "main_scene.h"

GameTexture* MainScene::scoreText = nullptr;
GameTexture* MainScene::comboText = nullptr;
SDL_Surface* MainScene::gameCanvas = nullptr;

void MainScene::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/score_text"] = renderer->CreateText("SCORES: ", "Helvetica-Bold", 24, { 255, 255, 255, 255 });
	renderer->textures["ui/combo_text"] = renderer->CreateText("X ", "Helvetica-Bold", 20, { 255, 255, 255, 255 });

	renderer->textures["ui/title_number_one"] = renderer->CreateText("1", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_number_two"] = renderer->CreateText("2", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_number_three"] = renderer->CreateText("3", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_go"] = renderer->CreateText("GO!", "Helvetica-Bold", 60, { 255, 255, 255, 255 });

	scoreText = renderer->CreateText("0", "Helvetica-Bold", 24, { 255, 255, 255, 255 });
	comboText = renderer->CreateText("0", "Helvetica-Bold", 20, { 255, 255, 255, 255 });
	gameCanvas = SDL_CreateRGBSurfaceWithFormat(0, renderer->width, renderer->height, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetSurfaceBlendMode(gameCanvas, SDL_BLENDMODE_BLEND);

}

void MainScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	int x = 40;
	int y = 40;
	renderer->RenderTexture("ui/score_text", x, y, Alignment::RIGHT);
	x += renderer->GetTextureByName("ui/score_text")->width;
	renderer->RenderTexture(scoreText, x, y, Alignment::RIGHT);

	SDL_FillRect(gameCanvas, NULL, 0);

	for (auto& entity : EntityManager::getInstance()->entities) {
		entity->onRender();
	}

	GameManager* game_mgr = GameManager::getInstance();

	vector<MousePath> paths = game_mgr->mousePathRecord->paths;
	if (paths.size() > 2) {
		for (int i = 0; i < game_mgr->mousePathRecord->paths.size() - 1; i++) {
			MousePath* point = &game_mgr->mousePathRecord->paths[i];
			MousePath* nextPoint = &game_mgr->mousePathRecord->paths[i + 1];

			float distance = sqrt(pow(point->point.x - nextPoint->point.x, 2) + pow(point->point.y - nextPoint->point.y, 2));

			if (distance > 200) {
				continue;
			}

			int alpha = ((float)point->longevity / 1000) * 255;
			SDL_SurfaceDrawLine(gameCanvas, point->point, nextPoint->point, 255, 0, 0, alpha, 1);
		}
	}

	if (game_mgr->currentCombo > 0 && game_mgr->comboExpirationTick > 0) {
		int height = 50; 
		int maxComboDuration = max(1000 + game_mgr->currentCombo * 50, 2000);
		int width = ((float)game_mgr->comboExpirationTick / maxComboDuration) * 200;

		GameTexture* comboTitleText = renderer->GetTextureByName("ui/combo_text");

		int comboX = renderer->width / 2;
		renderer->RenderTexture(comboTitleText, comboX, 40, Alignment::RIGHT);
		comboX += comboTitleText->width;
		renderer->RenderTexture(comboText, comboX, 40, Alignment::RIGHT);

		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_Rect fillRect = { renderer->width / 2 - 100, 40 + comboTitleText->height / 2, width, 15 };
		SDL_RenderFillRect(renderer->gRenderer, &fillRect);
	}


	//BLOOM
	//this fuck ass bloom looks so fuckign shit omfg
	//purely because we missing the blurring step so it looks inconsistent and really blocky
	//theres no "fast" way to do this since doing this w/o shader is basically burning your cpu
	int steps[4] = { 4, 8, 16, 32 };
	for (int i = 0; i < 6; i++) {
		SDL_Surface* downsampled = SDL_Downsampling(gameCanvas, (float)1 / steps[i]);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->gRenderer, downsampled);
		SDL_RenderCopy(renderer->gRenderer, texture, NULL, NULL);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(downsampled);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->gRenderer, gameCanvas);
	SDL_RenderCopy(renderer->gRenderer, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
}

void MainScene::UpdateScoreText() {
	Renderer* renderer = Renderer::getInstance();
	renderer->FreeGameTexture(scoreText);
	scoreText = renderer->CreateText(to_string(GameManager::getInstance()->score).c_str(), "Helvetica-Bold", 24, { 255, 255, 255, 255 });
}

void MainScene::UpdateComboText() {
	SDL_Log("Updating combo text");
	Renderer* renderer = Renderer::getInstance();
	renderer->FreeGameTexture(comboText);
	comboText = renderer->CreateText(to_string(GameManager::getInstance()->currentCombo).c_str(), "Helvetica-Bold", 20, { 255, 255, 255, 255 });
}