#include "death_scene.h"

void DeathScene::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/death_title"] = renderer->CreateText("YOU DIED...", "Helvetica-Bold", 40, { 255, 255, 255, 255 });
}

void DeathScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	const SDL_Point center = SDL_Point{ renderer->width / 2, renderer->height / 2 };
	renderer->RenderText("ui/death_title", center.x, center.y - 150);
}