#pragma once

#include "Screen.hpp"
#include "Enemies.hpp"
#include "natives.h"

namespace UIScript
{
	struct Data
	{
		static bool showScaleform;
		static int scaleformType;
		static bool pendingNoti;
		static std::string notiText;
	};

	void ScriptMain();
	void OnAbort();
}
