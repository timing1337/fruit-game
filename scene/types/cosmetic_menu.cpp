#include "cosmetic_menu.h"

CosmeticMenu::CosmeticMenu() : BaseScene(SceneId::COSMETIC)
{
	TextElement* title = this->AddText("cosmetic_title", vec2_t{ RENDERER_CENTER_X, 50 }, "Cosmetic Menu", "genshin", 30, { 255, 255, 255, 255 });
	title->SetAlignment(Alignment::CENTER);
	title->SetAlignmentVertical(AlignmentVertical::TOP);
	TextElement* description = this->AddText("cosmetic_description", vec2_t{ RENDERER_CENTER_X, 90 }, "Customize your blade with stylist color!", "genshin", 18, { 255, 255, 255, 255 });
	description->SetAlignment(Alignment::CENTER);
	description->SetAlignmentVertical(AlignmentVertical::TOP);

	vec2_t basePosition = { 200, 250 };

	for (auto& bladeColor : BladeColorsConfig::colors) {
		ImageElement* blade = this->AddImage(bladeColor->id, basePosition, bladeColor->textureId, [this](ImageElement* element) { this->OnSelectBlade(element); }, 5);
		blade->SetAlignment(Alignment::CENTER);
		blade->SetAlignmentVertical(AlignmentVertical::MIDDLE);

		TextElement* bladeName = this->AddText(bladeColor->id, vec2_t{ basePosition.x, basePosition.y + 70 }, bladeColor->name, "genshin", 20, { 255, 255, 255, 255 });
		bladeName->SetAlignment(Alignment::CENTER);
		bladeName->SetAlignmentVertical(AlignmentVertical::MIDDLE);

		TextElement* bladeDescription = this->AddText(bladeColor->id, vec2_t{ basePosition.x, bladeName->position.y + bladeName->bound.y / 2 + 10 }, bladeColor->description, "genshin", 14, { 255, 255, 255, 255 });
		bladeDescription->SetAlignment(Alignment::CENTER);
		bladeDescription->SetAlignmentVertical(AlignmentVertical::MIDDLE);
		
		//if we run out of bound, just reset the location and go down
		basePosition.x += blade->bound.x + 250;
		if (basePosition.x >= RENDERER_WIDTH - 200) {
			basePosition.x = 200;
			basePosition.y += 250;
		}
	}
}

void CosmeticMenu::OnSelectBlade(ImageElement* element) {
	GameData* data = GameManager::GetInstance()->gameData;

	BladeColor* bladeColor = BladeColorsConfig::GetBladeColorByName(element->id);

	if (bladeColor == nullptr) {
		SDL_Log("Can't find blade with name %s", element->id);
		return;
	}

	if (!bladeColor->isUnlocked) {
		AudioManager::GetInstance()->PlaySound("error.wav");
		SDL_Log("Blade %s is not unlocked", bladeColor->name);
		return;
	}
	AudioManager::GetInstance()->PlaySound("button.wav");
	data->bladeColor = bladeColor;
}

void CosmeticMenu::Render() {
	BaseScene::Render();

	GameData* data = GameManager::GetInstance()->gameData;
	Renderer* renderer = Renderer::GetInstance();

	GameTexture* lockedIcon = Renderer::GetInstance()->GetTextureByName("locked_item.png");
	GameTexture* selectedIcon = Renderer::GetInstance()->GetTextureByName("selected_item.png");

	for (auto& bladeColor : BladeColorsConfig::colors) {
		ImageElement* blade = (ImageElement*)this->GetElementById(bladeColor->id);
		if (blade == nullptr) {
			continue;
		}

		if (bladeColor->isUnlocked) {
			continue;
		}

		renderer->RenderTexture(lockedIcon, blade->position.x, blade->position.y, Alignment::CENTER, AlignmentVertical::MIDDLE);
	}

	ImageElement* selectedBlade = (ImageElement*)this->GetElementById(data->bladeColor->id);
	renderer->RenderTexture(selectedIcon, selectedBlade->position.x, selectedBlade->position.y, Alignment::CENTER, AlignmentVertical::MIDDLE);
}