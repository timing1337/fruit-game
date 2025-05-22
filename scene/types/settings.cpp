#include "settings.h"

Settings::Settings() : BaseScene(SceneId::SETTING) {
	//add a test slider
	TextElement* title = this->AddText("title", vec2_t{ RENDERER_CENTER_X, 50 }, "Settings", "genshin", 30, { 255, 255, 255, 255 });
	title->SetAlignment(Alignment::CENTER);
	title->SetAlignmentVertical(AlignmentVertical::TOP);
	TextElement* description = this->AddText("description", vec2_t{ RENDERER_CENTER_X, 90 }, "Change your game settings here", "genshin", 18, { 255, 255, 255, 255 });
	description->SetAlignment(Alignment::CENTER);
	description->SetAlignmentVertical(AlignmentVertical::TOP);

	float alignedStartX = RENDERER_CENTER_X - 200;
	float secondAlignedStartX = RENDERER_CENTER_X;

	float alignedStartY = RENDERER_CENTER_Y - 200;

	TextElement* music_slider_text = this->AddText("music_slider_text", { alignedStartX, alignedStartY }, "Music Volume", "genshin", 20, { 255, 255, 255, 255 });
	music_slider_text->SetAlignment(Alignment::LEFT);
	music_slider_text->SetAlignmentVertical(AlignmentVertical::MIDDLE);

	SliderElement* music_slider = this->AddSlider("music_volume", { secondAlignedStartX, alignedStartY }, 30, 200, 100, [this](SliderElement* slider) {
		SDL_Log("Music volume changed to %d", slider->currentValue);
		AudioManager::GetInstance()->ChangeMusicVolume(slider->currentValue);
	});

	alignedStartY += 100;

	TextElement* sound_slider_text = this->AddText("sound_slider_text", { alignedStartX, alignedStartY }, "Sound Volume", "genshin", 20, { 255, 255, 255, 255 });
	sound_slider_text->SetAlignment(Alignment::LEFT);
	sound_slider_text->SetAlignmentVertical(AlignmentVertical::MIDDLE);

	SliderElement* sound_slider = this->AddSlider("sound_volume", { secondAlignedStartX, alignedStartY }, 30, 200, 100, [this](SliderElement* slider) {
		SDL_Log("Sound volume changed to %d", slider->currentValue);
		AudioManager::GetInstance()->ChangeSoundVolume(slider->currentValue);
	});

	alignedStartY += 100;

	CheckboxElement* enableBloom = this->AddCheckbox("enable_bloom", { secondAlignedStartX + 25, alignedStartY }, 30, [this](CheckboxElement* checkbox) {
		SDL_Log("Enable bloom changed to %d", checkbox->value);
		});

	TextElement* enableBloomText = this->AddText("enable_bloom_text", { alignedStartX, alignedStartY }, "Enable Bloom", "genshin", 20, { 255, 255, 255, 255 });
	enableBloomText->SetAlignment(Alignment::LEFT);
	enableBloomText->SetAlignmentVertical(AlignmentVertical::MIDDLE);

	enableBloom->value = true;
}