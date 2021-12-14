#include "pch.h"
#include "UIScript.hpp"

bool UIScript::Data::pendingNoti;
std::string UIScript::Data::notiText;

bool ShouldShowCount()
{
	return SURVIVAL::SurvivalData::IsActive && SURVIVAL::SurvivalData::Started && !INTERMISSION::Data::Active && !SURVIVAL::SurvivalData::timed;
}

void UIScriptMain()
{
	while (true)
	{
		if (UIScript::Data::pendingNoti)
		{
			if (CAM::IS_SCREEN_FADED_OUT() && !CAM::IS_SCREEN_FADING_IN() && !CAM::IS_SCREEN_FADING_OUT())
			{
				SCREEN::ShowNotification(UIScript::Data::notiText.c_str());
				UIScript::Data::pendingNoti = false;
				UIScript::Data::notiText = "";
			}
		}

		if (ShouldShowCount())
		{
			SCREEN::ShowEnemyCountHelpText(SURVIVAL::SurvivalData::MaxWaveSize - ENEMIES::EnemiesData::kills, SURVIVAL::SurvivalData::MaxWaveSize, SURVIVAL::SurvivalData::CurrentWave, ENEMIES::EnemiesData::footEnemies.size());
		}

		WAIT(0);
	}
}

void UIScript::ScriptMain()
{
	UIScriptMain();
}

void UIScript::OnAbort()
{
	Data::pendingNoti = false;
	Data::notiText = (char*)"";
}
