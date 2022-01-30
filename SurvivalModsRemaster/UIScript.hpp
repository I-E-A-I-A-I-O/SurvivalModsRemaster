#pragma once

#include "Screen.hpp"
#include "Enemies.hpp"
#include "natives.h"

namespace UIScript
{
    enum class ScaleformType: int {
        WAVE_SURVIVED,
        SURVIVAL_CANCELED,
        SURVIVAL_PASSED,
        PLAYER_DIED,
        SURVIVAL_START,
        PICKUPS_REGEN,
        DIFFICULTY_INC,
        SURVIVAL_PASSED_TIMED
    };

	struct Data
	{
		static bool showScaleform;
		static ScaleformType scaleformType;
		static bool pendingNoti;
	};

    void Clean();
	void ScriptMain();
	void OnAbort();
}
