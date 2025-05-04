#include "main_stage.h"

MainStage::MainStage() : BaseScene(SceneId::GAME) {
	//fast hack
	TextElement* comboElement = this->AddText("combo", vec2_t{ RENDERER_CENTER_X, 20 }, "", "Helvetica-Bold", 20, { 255, 255, 255, 255 });
	comboElement->active = false;

	ImageElement* scoreIcon = this->AddImage("score_icon", vec2_t{ 10, 40 }, "score_icon.png");
	scoreIcon->SetAlignment(Alignment::LEFT);

	TextElement* scoreElement = this->AddText("score", vec2_t{ scoreIcon->bound.x + 15, 40 }, "0", "Helvetica-Bold", 40, { 255, 255, 255, 255 });
	scoreElement->SetAlignment(Alignment::LEFT);
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
	Downsampling::Downsample(texture);
	Downsampling::Render();
	SDL_RenderCopy(renderer->gRenderer, texture, NULL, NULL);
	SDL_DestroyTexture(texture);

	if (game_mgr->currentCombo > 0 && game_mgr->comboExpirationTick > 0) {
		int height = 50;
		int maxComboDuration = max(1000 + game_mgr->currentCombo * 50, 2000);
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
