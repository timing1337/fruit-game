#include "main_scene.h"

GameTexture* MainScene::scoreText = nullptr;
GameTexture* MainScene::comboText = nullptr;
SDL_Surface* MainScene::gameCanvas = nullptr;
SDL_Surface* MainScene::downsampledGameCanvas = nullptr;

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
	gameCanvas = SDL_CreateRGBSurfaceWithFormat(0, RENDERER_WIDTH, RENDERER_HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetSurfaceBlendMode(gameCanvas, SDL_BLENDMODE_BLEND);
	downsampledGameCanvas = SDL_CreateRGBSurfaceWithFormat(0, RENDERER_WIDTH, RENDERER_HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetSurfaceBlendMode(downsampledGameCanvas, SDL_BLENDMODE_BLEND);

}

void MainScene::DrawMousePathRecord(MousePathRecord* record) {
	float distance = 0;
	vector<MousePath> paths = record->paths;
	if (paths.size() > 2) {
		for (int i = 0; i < record->paths.size() - 1; i++) {
			MousePath* point = &record->paths[i];
			MousePath* nextPoint = &record->paths[i + 1];

			SDL_Color color1 = { 255, 0, 0, 255 };
			SDL_Color color2 = { 255, 255, 0, 255 };

			float ratio = distance / record->distance;
			SDL_Color color = GradientColorMix(color1, color2, ratio);

			int alpha = ((float)point->longevity / 500) * 255;
			SDL_SurfaceDrawLine(gameCanvas, point->point, nextPoint->point, color.r, color.g, color.b, alpha, 1);
			SDL_SurfaceDrawLine(downsampledGameCanvas, point->point, nextPoint->point, color.r, color.g, color.b, alpha, 3);
			distance += sqrt(pow(point->point.x - nextPoint->point.x, 2) + pow(point->point.y - nextPoint->point.y, 2));
		}
	}
}

void MainScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	SDL_FillRect(gameCanvas, NULL, 0);
	SDL_FillRect(downsampledGameCanvas, NULL, 0);

	for (auto& entity : EntityManager::getInstance()->entities) {
		entity->onRender();
	}

	GameManager* game_mgr = GameManager::getInstance();

	for (auto& record : game_mgr->mousePathRecordsLeftover) {
		DrawMousePathRecord(record);
	}

	if (game_mgr->mousePathRecord != nullptr) {
		DrawMousePathRecord(game_mgr->mousePathRecord);
	}

	//BLOOM
	//this fuck ass bloom looks so fuckign shit omfg
	//purely because we missing the blurring step so it looks inconsistent and really blocky
	//theres no "fast" way to do this since doing this w/o shader is basically burning your cpu
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->gRenderer, gameCanvas);
	//SDL_Texture* downsampledGameCanvasTexture = SDL_CreateTextureFromSurface(renderer->gRenderer, downsampledGameCanvas);
	//Downsampling::Downsample(downsampledGameCanvasTexture);
	//Downsampling::Render();

	SDL_RenderCopy(renderer->gRenderer, texture, NULL, NULL);
	
	SDL_DestroyTexture(texture);
	//SDL_DestroyTexture(downsampledGameCanvasTexture);

	int x = 40;
	int y = 40;
	renderer->RenderTexture("ui/score_text", x, y, Alignment::RIGHT);
	x += renderer->GetTextureByName("ui/score_text")->width;
	renderer->RenderTexture(scoreText, x, y, Alignment::RIGHT);

	if (game_mgr->currentCombo > 0 && game_mgr->comboExpirationTick > 0) {
		int height = 50;
		int maxComboDuration = max(1000 + game_mgr->currentCombo * 50, 2000);
		int width = ((float)game_mgr->comboExpirationTick / maxComboDuration) * 200;

		GameTexture* comboTitleText = renderer->GetTextureByName("ui/combo_text");

		int comboX = RENDERER_CENTER_X;
		renderer->RenderTexture(comboTitleText, comboX, 40, Alignment::RIGHT);
		comboX += comboTitleText->width;
		renderer->RenderTexture(comboText, comboX, 40, Alignment::RIGHT);

		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_Rect fillRect = { RENDERER_CENTER_X - 100, 40 + comboTitleText->height / 2, width, 15 };
		SDL_RenderFillRect(renderer->gRenderer, &fillRect);
	}
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