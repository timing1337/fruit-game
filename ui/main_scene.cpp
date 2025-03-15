#include "main_scene.h"

void MainScene::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/score_text"] = renderer->CreateText("SCORES: ", "Helvetica-Bold", 24, { 255, 255, 255, 255 });

	renderer->textures["ui/title_number_one"] = renderer->CreateText("1", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_number_two"] = renderer->CreateText("2", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_number_three"] = renderer->CreateText("3", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
	renderer->textures["ui/title_go"] = renderer->CreateText("GO!", "Helvetica-Bold", 60, { 255, 255, 255, 255 });
}

void MainScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	renderer->RenderText("ui/score_text", 40, 40, Alignment::RIGHT);
}