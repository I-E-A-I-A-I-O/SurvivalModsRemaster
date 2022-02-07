#pragma once

#include "natives.h"
#include "Script.hpp"
#include <string>

namespace SCREEN
{
	void ShowEnemyCountBadge(int count, int max, int wave);
	void ShowIntermissionBadge(int time, int wave, bool timed);
	void ShowTimeLeftBadge(int time);
	void ShowSubtitle(const char* text, int time);
	void ShowNotification(const char* text);
	void LoadSprites();
    void ShowHelpText(const char* text, bool beep);
	void UnloadSprites();
	void DrawBadge(const char* title, const char* content, bool red, int slot);
    void DrawBadge(const char* title, float percentage, int slot);
	void ShowControls();
	int RequestScaleform();
	void SetScaleformText(int scaleform, const char* title, const char* subtitle);
	void FadeOutScaleform(int handle, int duration);
	void FreeScaleform(int handle);
	void SetScaleformTextPassed(int scaleform, const char* title, const char* subtitle);
    std::vector<int> LoadWallStat(int cash, int wave, int milis, bool passed, bool doCash, bool isWave, bool time);
}
