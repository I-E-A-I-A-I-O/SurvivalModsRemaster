#pragma once

#include "natives.h"
#include <string>

namespace SCREEN
{
	void ShowEnemyCountHelpText(int count, int max, int wave, int altCount);
	void ShowHelpTextThisFrame(const char* text, bool beep);
	void ShowSubtitle(const char* text, int time);
	void ShowNotification(const char* text);
	void DrawMarker(int type, Vector3 position, Vector3 scale);
}
