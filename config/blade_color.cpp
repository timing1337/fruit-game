#include "blade_color.h"

//Not sure if handling this is good, eh whatever lol
// this is extremely limited -> i need to figure out another way of doing this

std::vector<BladeColor*> BladeColorsConfig::colors = {
	new BladeColor("default_blade", "Default", "Default blade", "blades/default.png", { 255, 255, 255, 255 }, {255, 255, 255, 255}, [](int score, int combo) {
		return true; //default, always true
	}),
	new BladeColor("ocean_mist", "Ocean mist", "Ocean-themed blade", "blades/default.png", { 113, 192, 187, 255 }, {78, 102, 136, 255}, [](int score, int combo) {
		return true; //default, always true
	}),
	new BladeColor("bloomify_blade", "Bloomify", "Reach 50 scores", "blades/bloomify.png", { 244, 155, 171, 255 }, {255, 225, 224, 255}, [](int score, int combo) {
		return true;
	}),

	/*
	new BladeColor("bloomify_blade", "Bloomify", "Reach 50 scores", "blades/bloomify.png", { 255, 44, 123, 255 }, {255, 143, 227, 255}, [](int score, int combo) {
		return score >= 50;
	}),
	new BladeColor("kyoko_blade", "Kyoko", "Reach 50 combos", "blades/blood_silver.png", { 255, 0, 0, 255 }, {150, 200, 230, 255}, [](int score, int combo) {
		return combo >= 50;
	}),
	new BladeColor("yin_and_yang_blade", "Yin & yang", "Reach 200 scores", "blades/yin_and_yang.png", {0, 0, 0, 255}, {255, 255, 255, 255}, [](int score, int combo) {
		return score >= 200;
	}),
	*/
};

BladeColor* BladeColorsConfig::GetBladeColorByName(const char* name) {
	for (auto& color : colors) {
		if (strcmp(color->id, name) == 0) {
			return color;
		}
	}
	return nullptr; 
}