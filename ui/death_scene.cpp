#include "death_scene.h"

void DeathScene::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/death_title"] = renderer->CreateText("YOU DIED", "Helvetica-Bold", 30, { 255, 255, 255, 255 });
	renderer->textures["ui/death_score_text"] = renderer->CreateText("You have sliced a total of ", "Helvetica-Bold", 20, { 255, 255, 255, 255 });
}

void DeathScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	const SDL_Point center = SDL_Point{ renderer->width / 2, renderer->height / 2 };
	renderer->RenderText("ui/death_title", center.x, center.y - 150);
	renderer->RenderText("ui/death_score_text", center.x, center.y - 120);

	renderer->RenderText(MainScene::scoreText, center.x + (renderer->textures["ui/death_score_text"]->text->width / 2), center.y - 120, Alignment::RIGHT);
}