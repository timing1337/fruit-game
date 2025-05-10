#include "main_stage.h"

MainStage::MainStage() : BaseScene(SceneId::GAME) {
	//fast hack
	TextElement* comboElement = this->AddText("combo", vec2_t{ RENDERER_CENTER_X, 16 }, "0", "genshin", 25, { 255, 255, 255, 255 });
	comboElement->active = false;

	ImageElement* scoreIcon = this->AddImage("score_icon", vec2_t{ 10, 10 }, "score_icon.png");
	scoreIcon->SetAlignmentVertical(AlignmentVertical::TOP);

	float scoreX = 10 + scoreIcon->bound.x + 5;

	TextElement* scoreElement = this->AddText("score", vec2_t{ scoreX, 12 }, "0", "genshin", 30, { 255, 255, 255, 255 });
	GameManager* game_mgr = GameManager::GetInstance();

	TextElement* highestScoreElement = this->AddText("highest_score", vec2_t{ scoreX, 12 + scoreElement->bound.y + 2 }, "Record: " + to_string(game_mgr->gameData->highestScore), "genshin", 20, { 255, 255, 255, 255 });
}

void MainStage::Prepare() {
	BaseScene::Prepare();

	Renderer* renderer = Renderer::GetInstance();

	glowCanvas = SDL_CreateTexture(renderer->gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RENDERER_WIDTH, RENDERER_HEIGHT);
	SDL_SetTextureBlendMode(glowCanvas, SDL_BLENDMODE_BLEND);

	gameCanvas = SDL_CreateTexture(renderer->gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RENDERER_WIDTH, RENDERER_HEIGHT);
	SDL_SetTextureBlendMode(gameCanvas, SDL_BLENDMODE_BLEND);
}

void MainStage::Render() {
	GameManager* game_mgr = GameManager::GetInstance();
	EntityManager* entity_mgr = EntityManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();
	

	SDL_SetRenderTarget(renderer->gRenderer, glowCanvas);
	SDL_SetRenderDrawColor(renderer->gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer->gRenderer);

	//Clear canvas
	SDL_SetRenderTarget(renderer->gRenderer, gameCanvas);
	SDL_SetRenderDrawColor(renderer->gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer->gRenderer);

	//ui elements | gameCanvas
	BaseScene::Render();

	if (game_mgr->currentCombo > 0 && game_mgr->comboExpirationTick > 0) {
		int maxComboBarWidth = 200;
		TextElement* comboElement = (TextElement*)this->GetElementById("combo");
		int maxComboDuration = max(COMBO_DURATION_BASE + game_mgr->currentCombo * COMBO_DURATION_MULTIPLIER, COMBO_DURATION_MAX);
		int width = ((float)game_mgr->comboExpirationTick / maxComboDuration) * maxComboBarWidth;
		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_Rect fillRect = { comboElement->position.x - maxComboBarWidth/2, comboElement->position.y + comboElement->bound.y + 3, width, 15 };
		SDL_RenderFillRect(renderer->gRenderer, &fillRect);
	}

	//entities
	for (int i = 0; i < entity_mgr->entities.size(); i++) {
		Entity* entity = entity_mgr->entities[i];
		if (!entity->alive) {
			continue;
		}
		entity->onRender();
	}

	SDL_SetRenderTarget(renderer->gRenderer, glowCanvas);
	for (auto& record : game_mgr->mousePathRecordsLeftover) {
		DrawMousePathRecord(record);
	}

	if (game_mgr->mousePathRecord != nullptr) {
		DrawMousePathRecord(game_mgr->mousePathRecord);
	}

	SDL_SetRenderTarget(renderer->gRenderer, NULL);
	SDL_RenderCopy(renderer->gRenderer, gameCanvas, NULL, NULL);
	SDL_RenderCopy(renderer->gRenderer, glowCanvas, NULL, NULL);

	Downsampling::Downsample(glowCanvas);
	Downsampling::Render();

	renderer->SetBackgroundColor(255, 0, 0, redColorOverlayOpacity);
}

void MainStage::Release() {
	BaseScene::Release();
	SDL_DestroyTexture(gameCanvas);
	SDL_DestroyTexture(glowCanvas);

	glowCanvas = nullptr;
	gameCanvas = nullptr;

	this->redColorOverlayOpacity = 0;
}

void MainStage::DrawMousePathRecord(MousePathRecord* record) {
	Renderer* renderer = Renderer::GetInstance();
	float distance = 0;

	int thickness = 1;
	vector<MousePath> paths = record->paths;
	if (paths.size() > 2) {
		for (int i = 0; i < record->paths.size() - 1; i++) {
			MousePath* point = &record->paths[i];
			MousePath* nextPoint = &record->paths[i + 1];

			SDL_Color color1 = { 102, 0, 204, 255 };
			SDL_Color color2 = { 255, 153, 255, 255 };

			float ratio = distance / record->distance;
			SDL_Color color = GradientColorMix(color1, color2, ratio);

			int alpha = ((float)point->longevity / 500) * 255;

			SDL_SetRenderDrawColor(renderer->gRenderer, color.r, color.g, color.b, alpha);

			for (int j = -thickness; j <= thickness; j++) {
				SDL_RenderDrawLineF(renderer->gRenderer, point->point.x, point->point.y + j, nextPoint->point.x, nextPoint->point.y + j);
			}

			for (int j = -thickness; j <= thickness; j++) {
				SDL_RenderDrawLineF(renderer->gRenderer, point->point.x + j, point->point.y, nextPoint->point.x + j, nextPoint->point.y);
			}

			distance += sqrt(pow(point->point.x - nextPoint->point.x, 2) + pow(point->point.y - nextPoint->point.y, 2));
		}
	}
	SDL_SetRenderDrawColor(renderer->gRenderer, 0, 0, 0, 0);
}
