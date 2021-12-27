#include "pch.h"
#include "UIScript.hpp"

bool UIScript::Data::pendingNoti;
std::string UIScript::Data::notiText;
bool UIScript::Data::showScaleform;
int UIScript::Data::scaleformType;
int scaleform = 0;
int scaleformST;
bool scaleformRequested;
bool scaleformFading;

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
		if (UIScript::Data::pendingNoti)
		{
			if (CAM::IS_SCREEN_FADED_IN() && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))
			{
				UIScript::Data::showScaleform = true;
				SCREEN::ShowNotification(UIScript::Data::notiText.c_str());
				UIScript::Data::pendingNoti = false;
				UIScript::Data::notiText = "";
			}
		}

		if (!SURVIVAL::SurvivalData::IsActive && Data::canStartMission)
		{
			SCREEN::ShowControls();
		}

		if (UIScript::Data::showScaleform)
		{
			if (!scaleformRequested)
			{
				scaleformRequested = true;
				scaleform = SCREEN::RequestScaleform();
				char title[150];
				char subtitle[50];

				switch (UIScript::Data::scaleformType)
				{
					case 0:
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
						break;
					case 1:
						strcpy_s(title, "~g~survival passed");
						strcpy_s(subtitle, " ");
						break;
					case 2:
						strcpy_s(title, "~r~survival canceled");
						strcpy_s(subtitle, " ");
						break;
					case 3:
						strcpy_s(title, "~r~survival failed");
						strcpy_s(subtitle, " ");
						break;
					case 4:
						strcpy_s(title, "~y~difficulty increased");
						strcpy_s(subtitle, "Pickups regenerated.");
						break;
					case 5:
						strcpy_s(title, "~y~Pickups regenerated");
						strcpy_s(subtitle, " ");
						break;
					default:
						break;
				}

				SCREEN::SetScaleformText(scaleform, title, subtitle);
				scaleformST = GAMEPLAY::GET_GAME_TIMER();
			}

			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleform, 255, 255, 0, 255, 0);

			if (GAMEPLAY::GET_GAME_TIMER() - scaleformST >= 8000 && !scaleformFading)
			{
				scaleformFading = true;
				SCREEN::FadeOutScaleform(scaleform, 2000);
				scaleformST = GAMEPLAY::GET_GAME_TIMER();
			}
			else if (GAMEPLAY::GET_GAME_TIMER() - scaleformST >= 2000 && scaleformFading)
			{
				UIScript::Data::showScaleform = false;
				scaleformRequested = false;
				scaleformFading = false;
				SCREEN::FreeScaleform(scaleform);
			}
		}

		if (ShowCount())
		{
			SCREEN::ShowEnemyCountBadge(SURVIVAL::SurvivalData::MaxWaveSize - ENEMIES::EnemiesData::kills, SURVIVAL::SurvivalData::MaxWaveSize, SURVIVAL::SurvivalData::CurrentWave);
		}

		if (ShowInterTimeLeft())
		{
			SCREEN::ShowIntermissionBadge(TIMERS::Intermission::timeLeft, SURVIVAL::SurvivalData::CurrentWave + 1, SURVIVAL::SurvivalData::timed);
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

	if (scaleform != 0)
		SCREEN::FreeScaleform(scaleform);
}
