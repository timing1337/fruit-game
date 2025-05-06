#pragma once

#include "scene/base_scene.h"

class EndStage : public BaseScene
{
public:
	EndStage();

	void OnStart();
	void OnSettings();
	void OnQuit();
};