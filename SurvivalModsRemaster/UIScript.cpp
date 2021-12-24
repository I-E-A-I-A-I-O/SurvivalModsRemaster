#include "pch.h"
#include "UIScript.hpp"

bool UIScript::Data::pendingNoti;
std::string UIScript::Data::notiText;
bool waveScaleformRequested;
int waveScaleform = 0;
bool waveScaleformActive;
bool waveScaleformFade;

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

		if (ShowInterTimeLeft())
		{
			if (!waveScaleformRequested)
			{
				waveScaleformRequested = true;
				waveScaleformActive = true;
				waveScaleform = SCREEN::RequestScaleform();
				char title[150];
				char subtitle[50];

				if (SURVIVAL::SurvivalData::CurrentWave == 0)
				{
					strcpy_s(title, "~y~");
					strcat_s(title, SURVIVAL::SurvivalData::MissionID.c_str());
					strcpy_s(subtitle, "Mode: ");

					if (SURVIVAL::SurvivalData::timed)
						strcat_s(subtitle, "Timed survival.");
					else if (SURVIVAL::SurvivalData::hardcore)
						strcat_s(subtitle, "Hardcore survival.");
					else if (SURVIVAL::SurvivalData::InfiniteWaves)
						strcat_s(subtitle, "Endless waves survival.");
					else
						strcat_s(subtitle, "Ten waves survival.");
				}
				else
				{
					strcpy_s(title, "~y~wave ");
					strcat_s(title, std::to_string(SURVIVAL::SurvivalData::CurrentWave).c_str());
					strcat_s(title, " survived");
					strcpy_s(subtitle, "Prepare for the next wave.");
				}

				SCREEN::SetScaleformText(waveScaleform, title, subtitle);
			}

			if (waveScaleformActive)
			{
				GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(waveScaleform, 255, 255, 0, 255, 0);

				if (TIMERS::Intermission::timeLeft <= 12 && !waveScaleformFade)
				{
					waveScaleformFade = true;
					SCREEN::FadeOutScaleform(waveScaleform, 2000);
				}
				else if (TIMERS::Intermission::timeLeft <= 10)
					waveScaleformActive = false;
			}

			SCREEN::ShowIntermissionBadge(TIMERS::Intermission::timeLeft, SURVIVAL::SurvivalData::CurrentWave + 1, SURVIVAL::SurvivalData::timed);
		}
		else
		{
			waveScaleformRequested = false;
			waveScaleformFade = false;
			SCREEN::FreeScaleform(waveScaleform);
		}

		if (ShowTimeLeft())
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

	if (waveScaleform != 0)
		SCREEN::FreeScaleform(waveScaleform);
}
