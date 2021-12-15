#include "pch.h"
#include "UIScript.hpp"

bool UIScript::Data::pendingNoti;
std::string UIScript::Data::notiText;

bool ShowCount()
{
	return SURVIVAL::SurvivalData::IsActive && SURVIVAL::SurvivalData::Started && !INTERMISSION::Data::Active && !SURVIVAL::SurvivalData::timed;
}

bool ShowInterTimeLeft()
{
	return SURVIVAL::SurvivalData::IsActive && SURVIVAL::SurvivalData::Started && INTERMISSION::Data::Active;
}

bool ShowTimeLeft()
{
	return SURVIVAL::SurvivalData::IsActive && SURVIVAL::SurvivalData::Started && !INTERMISSION::Data::Active && SURVIVAL::SurvivalData::timed;
}

void UIScriptMain()
{
	SCREEN::LoadSprites();

	while (true)
	{
		if (!SURVIVAL::SurvivalData::IsActive && Data::canStartMission)
		{
			SCREEN::ShowControls();
		}

		if (UIScript::Data::pendingNoti)
		{
			if (CAM::IS_SCREEN_FADED_IN() && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))
			{
				SCREEN::ShowNotification(UIScript::Data::notiText.c_str());
				UIScript::Data::pendingNoti = false;
				UIScript::Data::notiText = "";
			}
		}

		if (ShowCount())
		{
			SCREEN::ShowEnemyCountBadge(SURVIVAL::SurvivalData::MaxWaveSize - ENEMIES::EnemiesData::kills, SURVIVAL::SurvivalData::MaxWaveSize, SURVIVAL::SurvivalData::CurrentWave);
		}
		else if (ShowInterTimeLeft())
		{
			SCREEN::ShowIntermissionBadge(TIMERS::Intermission::timeLeft, SURVIVAL::SurvivalData::CurrentWave + 1, SURVIVAL::SurvivalData::timed);
		}
		else if (ShowTimeLeft())
		{
			SCREEN::ShowTimeLeftBadge(TIMERS::TimedSurvival::timeLeft);
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
	SCREEN::UnloadSprites();
	Data::pendingNoti = false;
	Data::notiText = (char*)"";
}
