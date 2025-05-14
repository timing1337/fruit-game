#include "blade_color.h"

//Not sure if handling this is good, eh whatever lol
// this is extremely limited -> i need to figure out another way of doing this

std::vector<BladeColor*> BladeColorsConfig::colors = {
	new BladeColor("default_blade", "Default", "Default blade", "blades/bloomify.png", { 102, 204, 255, 255 }, {204, 228, 255, 255}, [](int score, int combo) {
		return true;
	}),
	new BladeColor("bloomify_blade", "Bloomify", "Reach 50 scores", "blades/bloomify.png", { 255, 44, 123, 255 }, {255, 143, 227, 255}, [](int score, int combo) {
		return true;
	}),
	new BladeColor("kyoko_blade", "Kyoko", "Reach 50 combos", "blades/blood_silver.png", { 255, 0, 0, 255 }, {150, 200, 230, 255}, [](int score, int combo) {
		return true;
	}),
	new BladeColor("yin_and_yang_blade", "Yin & yang", "Reach 200 scores", "blades/yin_and_yang.png", {0, 0, 0, 255}, {255, 255, 255, 255}, [](int score, int combo) {
		return true;
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