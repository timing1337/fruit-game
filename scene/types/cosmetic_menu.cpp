#include "cosmetic_menu.h"

CosmeticMenu::CosmeticMenu() : BaseScene(SceneId::COSMETIC)
{
	TextElement* title = this->AddText("cosmetic_title", vec2_t{ RENDERER_CENTER_X, 50 }, "Cosmetic Menu", "genshin", 30, { 255, 255, 255, 255 });
	title->SetAlignment(Alignment::CENTER);
	title->SetAlignmentVertical(AlignmentVertical::TOP);
	TextElement* description = this->AddText("cosmetic_description", vec2_t{ RENDERER_CENTER_X, 90 }, "Customize your blade with stylist color!", "genshin", 18, { 255, 255, 255, 255 });
	description->SetAlignment(Alignment::CENTER);
	description->SetAlignmentVertical(AlignmentVertical::TOP);

	ImageElement* blood_silver_blade = this->AddImage("blood_silver_blade", vec2_t{ 300, 300 }, "blades/blood_silver.png", [](ImageElement* self) {
		SDL_Log("Blade clicked");
		}, 20, { 0, 0, 0, 255 });
	blood_silver_blade->SetAlignment(Alignment::CENTER);
	blood_silver_blade->SetAlignmentVertical(AlignmentVertical::MIDDLE);

	TextElement* blood_silver_blade_text = this->AddText("blood_silver_blade_text", vec2_t{ blood_silver_blade->position.x, blood_silver_blade->position.y + 70 }, "Blood & Silver", "genshin", 15, { 255, 255, 255, 255 });
	blood_silver_blade_text->SetAlignment(Alignment::CENTER);

	ImageElement* yin_and_yang_blade = this->AddImage("yin_and_yang_blade", vec2_t{ 500, 300 }, "blades/yin_and_yang.png", [](ImageElement* self) {
		SDL_Log("Blade clicked");
		}, 20, { 0, 0, 0, 255 });
	yin_and_yang_blade->SetAlignment(Alignment::CENTER);
	yin_and_yang_blade->SetAlignmentVertical(AlignmentVertical::MIDDLE);

	TextElement* yin_and_yang_blade_text = this->AddText("yin_and_yang_blade_text", vec2_t{ yin_and_yang_blade->position.x, yin_and_yang_blade->position.y + 70 }, "Yin & Yang", "genshin", 15, { 255, 255, 255, 255 });
	yin_and_yang_blade_text->SetAlignment(Alignment::CENTER);

	ImageElement* bloomify_blade = this->AddImage("bloomify_blade", vec2_t{ 700, 300 }, "blades/bloomify.png", [](ImageElement* self) {
		SDL_Log("Blade clicked");
		}, 20, { 0, 0, 0, 255 });
	bloomify_blade->SetAlignment(Alignment::CENTER);
	bloomify_blade->SetAlignmentVertical(AlignmentVertical::MIDDLE);

	TextElement* bloomify_blade_text = this->AddText("bloomify_blade_text", vec2_t{ bloomify_blade->position.x, bloomify_blade->position.y + 70 }, "Bloomify", "genshin", 15, { 255, 255, 255, 255 });
	bloomify_blade_text->SetAlignment(Alignment::CENTER);
}