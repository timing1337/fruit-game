#include "blade_color.h"

//Not sure if handling this is good, eh whatever lol
// this is extremely limited -> i need to figure out another way of doing this

std::vector<BladeColor*> BladeColorsConfig::colors = {
	new BladeColor("default_blade", "Default", "Default blade", "blades/default.png", "blade/default.wav", { 255, 255, 255, 255 }, {255, 255, 255, 255}, [](int score, int combo) {
		return true;
	}),
	new BladeColor("bloomify_blade", "Bloomify", "Reach 100 scores", "blades/bloomify.png", "blade/bloomify.wav", { 255, 107, 196, 255 }, {255, 161, 192, 255}, [](int score, int combo) {
		return score >= 150;
	}),
	new BladeColor("corruption_blade", "Corruption", "Reach 250 scores", "blades/corruption.png", "blade/default.wav", {104, 28, 255, 255}, {17, 13, 74, 255}, [](int score, int combo) {
		return score >= 250;
	}),
	new BladeColor("naturalist_blade", "Natural", "Reach 140 combo", "blades/natural.png", "blade/default.wav", {221, 235, 157, 255}, {184, 250, 117, 255}, [](int score, int combo) {
		return combo >= 140;
	}),
	new BladeColor("yin_and_yang_blade", "Yin & yang", "Reach 500 scores", "blades/yin_and_yang.png", "blade/default.wav", {0, 0, 0, 255}, {255, 255, 255, 255}, [](int score, int combo) {
		return score >= 500;
	}),
	new BladeColor("energized_blade", "ENERGIZEDDD", "Reach 1000 scores and 250 combo", "blades/energized.png", "blade/default.wav", {88, 250, 82, 255}, {255, 220, 43, 255}, [](int score, int combo) {
		return score >= 1000 && combo >= 250;
	}),
	new BladeColor("summer_time", "Summertime", "Summer-themed blade, event-only", "blades/summertime.png", "blade/default.wav", { 61, 213, 255, 255 }, {243, 132, 23, 255}, [](int score, int combo) {
		return false;
	}),
};

BladeColor* BladeColorsConfig::GetBladeColorByName(const char* name) {
	for (auto& color : colors) {
		if (strcmp(color->id, name) == 0) {
			return color;
		}
	}
	return nullptr; 
}