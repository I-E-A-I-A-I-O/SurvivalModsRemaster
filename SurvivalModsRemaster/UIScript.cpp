#include <main.h>
#include "pch.h"
#include "UIScript.hpp"

bool UIScript::Data::pendingNoti;
bool UIScript::Data::showScaleform;
UIScript::ScaleformType UIScript::Data::scaleformType;
std::vector<int> scaleformHandles = std::vector<int>();
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

void UIScript::Clean() {
    UIScript::Data::showScaleform = false;
    scaleformRequested = false;
    scaleformFading = false;

    for (const int& handle : scaleformHandles) {
        SCREEN::FreeScaleform(handle);
    }

    scaleformHandles.clear();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
void UIScriptMain()
{
    while (DLC::GET_IS_LOADING_SCREEN_ACTIVE())
        WAIT(1);

	while (true)
	{
		if (UIScript::Data::pendingNoti)
		{
			if (CAM::IS_SCREEN_FADED_IN() && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))
			{
				UIScript::Data::showScaleform = true;
				UIScript::Data::pendingNoti = false;
			}
		}

		if (!SURVIVAL::SurvivalData::IsActive && Data::showControls)
		{
			SCREEN::ShowControls();
		}

		if (UIScript::Data::showScaleform)
		{
			if (!scaleformRequested)
			{
				scaleformRequested = true;
				char title[150];
				char subtitle[100];

				switch (UIScript::Data::scaleformType)
				{
                    case UIScript::ScaleformType::SURVIVAL_START: {
                        scaleformHandles.push_back(SCREEN::RequestScaleform());
                        int h = scaleformHandles.at(0);
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

                        SCREEN::SetScaleformText(h, title, subtitle);
                        break;
                    }
					case UIScript::ScaleformType::SURVIVAL_PASSED: {
                        AUDIO::PLAY_SOUND_FRONTEND(-1, "BASE_JUMP_PASSED", "HUD_AWARDS", true);
                        scaleformHandles = SCREEN::LoadWallStat(SURVIVAL::SurvivalData::earnedMoney,
                                0,0,true,true,false,false);
                        break;
                    }
					case UIScript::ScaleformType::SURVIVAL_CANCELED: {
                        scaleformHandles.push_back(SCREEN::RequestScaleform());
                        int h = scaleformHandles.at(0);
                        strcpy_s(title, "~r~survival canceled");
                        strcpy_s(subtitle, "");
                        SCREEN::SetScaleformText(h, title, subtitle);
                        AUDIO::PLAY_SOUND_FRONTEND(-1, "Survival_Failed", "DLC_VW_AS_Sounds", true);
                        break;
                    }
					case UIScript::ScaleformType::PLAYER_DIED: {
                        AUDIO::PLAY_SOUND_FRONTEND(-1, "Survival_Failed", "DLC_VW_AS_Sounds", true);
                        scaleformHandles = SCREEN::LoadWallStat(SURVIVAL::SurvivalData::earnedMoney,
                                                                SURVIVAL::SurvivalData::CurrentWave,
                                                                0,false,true,
                                                                false,false);
                        break;
                    }
					case UIScript::ScaleformType::DIFFICULTY_INC: {
                        scaleformHandles.push_back(SCREEN::RequestScaleform());
                        int h = scaleformHandles.at(0);
                        strcpy_s(title, "~y~difficulty increased");
                        strcpy_s(subtitle, "Pickups regenerated.");
                        SCREEN::SetScaleformText(h, title, subtitle);
                        break;
                    }
					case UIScript::ScaleformType::PICKUPS_REGEN: {
                        scaleformHandles.push_back(SCREEN::RequestScaleform());
                        int h = scaleformHandles.at(0);
                        strcpy_s(title, "~y~Pickups regenerated");
                        strcpy_s(subtitle, "");
                        SCREEN::SetScaleformText(h, title, subtitle);
                        break;
                    }
					case UIScript::ScaleformType::SURVIVAL_PASSED_TIMED:
					{
						int total = TIMERS::TimedSurvival::CurrentTime - TIMERS::TimedSurvival::StartTime;
						AUDIO::PLAY_SOUND_FRONTEND(-1, "Survival_Passed", "DLC_VW_Survival_Sounds", true);
                        scaleformHandles = SCREEN::LoadWallStat(SURVIVAL::SurvivalData::earnedMoney,
                                                                0,
                                                                total,true,true,
                                                                false,true);
                        break;
					}
                    case UIScript::ScaleformType::WAVE_SURVIVED: {
                        AUDIO::PLAY_SOUND_FRONTEND(-1, "MP_WAVE_COMPLETE", "HUD_FRONTEND_DEFAULT_SOUNDSET", true);
                        scaleformHandles = SCREEN::LoadWallStat(SURVIVAL::SurvivalData::earnedMoney,
                                                                SURVIVAL::SurvivalData::CurrentWave,
                                                                0,false,false,
                                                                true,false);
                        break;
                    }
					default:
						break;
				}

				scaleformST = MISC::GET_GAME_TIMER();
			}

            if (scaleformHandles.size() > (size_t)1) {
                for (const int& handle: scaleformHandles) {
                    GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(handle, 255, 255, 0, 255, 0);

                    if (MISC::GET_GAME_TIMER() - scaleformST >= 15000)
                    {
                        UIScript::Data::showScaleform = false;
                        scaleformRequested = false;
                        scaleformFading = false;
                        SCREEN::FreeScaleform(handle);
                    }
                }

                if (!UIScript::Data::showScaleform)
                    scaleformHandles.clear();
            } else {
                int handle = scaleformHandles.at(0);
                GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(handle, 255, 255, 0, 255, 0);

                if (MISC::GET_GAME_TIMER() - scaleformST >= 8000 && !scaleformFading)
                {
                    scaleformFading = true;
                    SCREEN::FadeOutScaleform(handle, 2000);
                    scaleformST = MISC::GET_GAME_TIMER();
                }
                else if (MISC::GET_GAME_TIMER() - scaleformST >= 2000 && scaleformFading)
                {
                    UIScript::Data::showScaleform = false;
                    scaleformRequested = false;
                    scaleformFading = false;
                    SCREEN::FreeScaleform(handle);
                    scaleformHandles.clear();
                }
            }
		}

		if (ShowCount())
		{
			SCREEN::ShowEnemyCountBadge(ENEMIES::EnemiesData::kills, SURVIVAL::SurvivalData::MaxWaveSize, SURVIVAL::SurvivalData::CurrentWave);
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
#pragma clang diagnostic pop

void UIScript::ScriptMain()
{
	UIScriptMain();
}

void UIScript::OnAbort()
{
	SCREEN::UnloadSprites();
	Data::pendingNoti = false;

    for (const auto& scaleform : scaleformHandles) {
        if (scaleform != 0)
            SCREEN::FreeScaleform(scaleform);
    }
}
