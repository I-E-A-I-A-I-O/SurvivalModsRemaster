#pragma once

#include "natives.h"
#include "Script.hpp"
#include <string>

namespace SCREEN
{
	void ShowEnemyCountBadge(int count, int max, int wave);
	void ShowIntermissionBadge(int time, int wave, bool timed);
	void ShowTimeLeftBadge(int time);
	void ShowHelpTextThisFrame(const char* text, bool beep);
	void ShowSubtitle(const char* text, int time);
	void ShowNotification(const char* text);
	void DrawMarker(int type, Vector3 position, Vector3 scale);
	void LoadSprites();
	void UnloadSprites();
	void DrawBadge(const char* title, const char* content, bool red, int slot);
	void ShowControls();
}
