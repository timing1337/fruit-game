#include "main_stage.h"

MainStage::MainStage() : BaseScene(SceneId::GAME) {
	//fast hack
	TextElement* comboElement = this->AddText("combo", vec2_t{ RENDERER_CENTER_X, 20 }, "0", "Helvetica-Bold", 20, { 255, 255, 255, 255 });
	comboElement->active = false;

	ImageElement* scoreIcon = this->AddImage("score_icon", vec2_t{ 10, 10 }, "score_icon.png");
	scoreIcon->SetAlignmentVertical(AlignmentVertical::TOP);

	float scoreX = 10 + scoreIcon->bound.x + 5;

	TextElement* scoreElement = this->AddText("score", vec2_t{ scoreX, 10 }, "0", "Helvetica-Bold", 40, { 255, 255, 255, 255 });
	GameManager* game_mgr = GameManager::GetInstance();

	TextElement* highestScoreElement = this->AddText("highest_score", vec2_t{ scoreX, 10 + scoreElement->bound.y + 2 }, "Record: " + to_string(game_mgr->gameData->highestScore), "Helvetica-Bold", 20, { 255, 255, 255, 255 });
}

void MainStage::Prepare() {
	BaseScene::Prepare();

	gameCanvas = SDL_CreateRGBSurfaceWithFormat(0, RENDERER_WIDTH, RENDERER_HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetSurfaceBlendMode(gameCanvas, SDL_BLENDMODE_BLEND);
}

void MainStage::Render() {
	BaseScene::Render();
	
	SDL_FillRect(gameCanvas, NULL, 0);

	GameManager* game_mgr = GameManager::GetInstance();
	EntityManager* entity_mgr = EntityManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();

	for (int i = 0; i < entity_mgr->entities.size(); i++) {
		Entity* entity = entity_mgr->entities[i];
		if (!entity->alive) {
			continue;
		}
		entity->onRender();
	}

	for (auto& record : game_mgr->mousePathRecordsLeftover) {
		DrawMousePathRecord(record);
	}

	if (game_mgr->mousePathRecord != nullptr) {
		DrawMousePathRecord(game_mgr->mousePathRecord);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->gRenderer, gameCanvas);
	//Downsampling::Downsample(texture);
	//Downsampling::Render();
	SDL_RenderCopy(renderer->gRenderer, texture, NULL, NULL);
	SDL_DestroyTexture(texture);

	if (game_mgr->currentCombo > 0 && game_mgr->comboExpirationTick > 0) {
		int maxComboDuration = max(COMBO_DURATION_BASE + game_mgr->currentCombo * COMBO_DURATION_MULTIPLIER, COMBO_DURATION_MAX);
		int width = ((float)game_mgr->comboExpirationTick / maxComboDuration) * 200;
		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_Rect fillRect = { RENDERER_CENTER_X - 100, 40, width, 15 };
		SDL_RenderFillRect(renderer->gRenderer, &fillRect);
	}
}

void MainStage::Release() {
	BaseScene::Release();

	SDL_FreeSurface(gameCanvas);

	gameCanvas = nullptr;
}

void MainStage::DrawMousePathRecord(MousePathRecord* record) {
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

			distance += sqrt(pow(point->point.x - nextPoint->point.x, 2) + pow(point->point.y - nextPoint->point.y, 2));
		}
	}
}
