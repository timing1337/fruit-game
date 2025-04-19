#include "main_scene.h"

GameTexture* MainScene::scoreText = nullptr;
SDL_Surface* MainScene::gameCanvas = nullptr;

void MainScene::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/score_text"] = renderer->CreateText("SCORES: ", "Helvetica-Bold", 24, { 255, 255, 255, 255 });

	renderer->textures["ui/title_number_one"] = renderer->CreateText("1", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_number_two"] = renderer->CreateText("2", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_number_three"] = renderer->CreateText("3", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_go"] = renderer->CreateText("GO!", "Helvetica-Bold", 60, { 255, 255, 255, 255 });

	scoreText = renderer->CreateText("0", "Helvetica-Bold", 24, { 255, 255, 255, 255 });
	gameCanvas = SDL_CreateRGBSurfaceWithFormat(0, renderer->width, renderer->height, 32, SDL_PIXELFORMAT_RGBA8888);
}

void MainScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	int x = 40;
	int y = 40;
	renderer->RenderTexture("ui/score_text", x, y, Alignment::RIGHT);
	x += renderer->GetTextureByName("ui/score_text")->width;
	renderer->RenderTexture(scoreText, x, y, Alignment::RIGHT);

	SDL_FillRect(gameCanvas, NULL, 0x000000);

	for (auto& entity : EntityManager::getInstance()->entities) {
		entity->onRender();
	}

	vector<MousePath> paths = GameManager::getInstance()->mousePathRecord->paths;
	if (paths.size() > 2) {
		for (int i = 0; i < GameManager::getInstance()->mousePathRecord->paths.size() - 1; i++) {
			MousePath* point = &GameManager::getInstance()->mousePathRecord->paths[i];
			MousePath* nextPoint = &GameManager::getInstance()->mousePathRecord->paths[i + 1];

			float distance = sqrt(pow(point->point.x - nextPoint->point.x, 2) + pow(point->point.y - nextPoint->point.y, 2));

			if (distance > 120) {
				continue;
			}

			int alpha = point->longevity * 255 / 1000;
			SDL_SurfaceDrawLine(gameCanvas, point->point, nextPoint->point, 255, 255, 255, alpha, 1);
		}
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