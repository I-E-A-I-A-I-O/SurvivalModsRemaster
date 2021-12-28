#include "pch.h"
#include "Survival.hpp"

bool SURVIVAL::SurvivalData::IsActive;
std::string SURVIVAL::SurvivalData::MissionID;
bool SURVIVAL::SurvivalData::InfiniteWaves;
int SURVIVAL::SurvivalData::RewardedMoney;
int SURVIVAL::SurvivalData::CurrentWave;
int SURVIVAL::SurvivalData::MaxAircraft;
int SURVIVAL::SurvivalData::MaxVehicles;
int SURVIVAL::SurvivalData::MaxWaveSize;
bool SURVIVAL::SurvivalData::Triggered;
bool SURVIVAL::SurvivalData::Started;
bool SURVIVAL::SurvivalData::timed;
int SURVIVAL::SurvivalData::timedTimeLeft;
bool SURVIVAL::SurvivalData::cheated;
bool SURVIVAL::SurvivalData::hardcore;
Ped ped;

void SURVIVAL::StartMission(bool infiniteWaves, bool timed, bool hardcore)
{
	SurvivalData::MissionID = TriggerPedsData::names.at(Data::TPIndex);
	LoadSurvival(SurvivalData::MissionID);
	SurvivalData::IsActive = true;
	SurvivalData::Started = false;
	SurvivalData::cheated = false;
	SurvivalData::hardcore = hardcore;
	SurvivalData::timedTimeLeft = 60000;
	SurvivalData::InfiniteWaves = infiniteWaves;
	SurvivalData::timed = timed;
	MUSIC::PrepareTracks();
	ped = TriggerPedsData::peds.at(Data::TPIndex);
	ENTITY::SET_ENTITY_INVINCIBLE(ped, false);

	if (!WEAPON::HAS_PED_GOT_WEAPON(PLAYER::PLAYER_PED_ID(), eWeapon::WeaponPistol, false))
	{
		WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), eWeapon::WeaponPistol, 25, true, true);
	}
	else
	{
		WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), eWeapon::WeaponPistol, true);
	}

	PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), false, 0);
	AI::TASK_AIM_GUN_AT_ENTITY(PLAYER::PLAYER_PED_ID(), ped, -1, 0);
	AI::TASK_SHOOT_AT_ENTITY(PLAYER::PLAYER_PED_ID(), ped, 60000, eFiringPattern::FiringPatternFullAuto);
	SurvivalData::Triggered = true;
}

void SURVIVAL::Initialize()
{
	PICKUPS::Initialize();
	TIMERS::RestartTimers();
	SurvivalData::CurrentWave = 0;
	SurvivalData::MaxVehicles = 0;
	SurvivalData::MaxAircraft = 0;
	INTERMISSION::Start();
	MUSIC::StartTrack();
	PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(PLAYER::PLAYER_ID(), false);

	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
	{
		Vehicle ve = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
		AI::TASK_LEAVE_VEHICLE(PLAYER::PLAYER_PED_ID(), ve, 0);
	}

	if (SpawnerData::isHalloween)
	{
		GAMEPLAY::_SET_WEATHER_TYPE_OVER_TIME((char*)"HALLOWEEN", 30);
		GAMEPLAY::_SET_RAIN_FX_INTENSITY(0.5f);
	}
	else if (SpawnerData::isXmas)
	{
		GAMEPLAY::_SET_WEATHER_TYPE_OVER_TIME((char*)"BLIZZARD", 30);
		GAMEPLAY::_SET_RAIN_FX_INTENSITY(0.5f);
	}

	SurvivalData::Started = true;
}

void SURVIVAL::SetOffTrigger()
{
	if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
	}

	if (!PED::IS_PED_DEAD_OR_DYING(ped, 1))
	{
		if (TIMERS::ProcessTriggerPedTimer())
		{
			ENTITY::SET_ENTITY_HEALTH(ped, 0);
		}
		else
		{
			return;
		}
	}

	TIMERS::RestartTriggerPedTimer();
	SurvivalData::Triggered = false;
	UI::REMOVE_BLIP(&TriggerPedsData::blips.at(Data::TPIndex));
	TriggerPedsData::blips.at(Data::TPIndex) = 0;
	ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
	TriggerPedsData::peds.at(Data::TPIndex) = 0;
	AI::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID());
	PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), true, 0);
	Initialize();
}

bool SURVIVAL::PlayerCheated()
{
	return PLAYER::GET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID());
}

void SURVIVAL::ProcessSurvival()
{
	if (!SurvivalData::cheated && PlayerCheated())
	{
		SurvivalData::cheated = true;
	}

	PLAYER::SET_PLAYER_MAY_NOT_ENTER_ANY_VEHICLE(PLAYER::PLAYER_ID());

	PICKUPS::Process();

	if (SurvivalData::CurrentWave >= 3 || SurvivalData::hardcore)
	{
		ENEMIES::RemoveUnusedVehicles();
	}

	if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
	}

	if (!CALC::IsInRange_2(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true), SURVIVAL::SpawnerData::location, 80))
	{
		GRAPHICS::DRAW_MARKER(1, SURVIVAL::SpawnerData::location.x, SURVIVAL::SpawnerData::location.y, SURVIVAL::SpawnerData::location.z, 0, 1, 0, 0, 0, 0, 160, 160, 160, 255, 255, 0, 100, false, false, 2, false, NULL, NULL, false);
		
		if (TIMERS::ProcessLeavingZoneTimer())
		{
			QuitSurvival(false);
		}
	}
	else if (TIMERS::LeavingZoneTimerStarted())
	{
		TIMERS::RestartLeavingZoneTimer();
	}

	if (INTERMISSION::Data::Active)
	{
		INTERMISSION::Process();
	}
	else
	{
		ENEMIES::Process();
	}
}

void SURVIVAL::GiveReward(bool playerDied)
{
	int reward = 0;

	if (SurvivalData::cheated)
	{
		std::string notification = "";
		notification.append("Survival ended at wave ");
		notification.append(std::to_string(SurvivalData::CurrentWave).c_str());
		notification.append(". Reward: ~g~$");
		notification.append(std::to_string(reward).c_str());
		UIScript::Data::pendingNoti = true;
		UIScript::Data::scaleformType = SurvivalData::timed ? 6 : 3;
		UIScript::Data::notiText = notification;
		return;
	}

	Hash michaelModel = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER_ZERO");
	Hash franklinModel = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER_ONE");
	Hash trevorModel = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER_TWO");

	Hash stat;
	Ped ped = PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID());
	Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

	if (pedModel == michaelModel)
	{
		stat = GAMEPLAY::GET_HASH_KEY((char*)"SP0_TOTAL_CASH");
	}
	else if (pedModel == franklinModel)
	{
		stat = GAMEPLAY::GET_HASH_KEY((char*)"SP1_TOTAL_CASH");
	}
	else if (pedModel == trevorModel)
	{
		stat = GAMEPLAY::GET_HASH_KEY((char*)"SP2_TOTAL_CASH");
	}
	else
	{
		std::string notification = "";
		notification.append("Survival ended at wave ");
		notification.append(std::to_string(SurvivalData::CurrentWave).c_str());
		notification.append(". Reward: ~g~$");
		notification.append(std::to_string(reward).c_str());
		UIScript::Data::pendingNoti = true;
		UIScript::Data::scaleformType = SurvivalData::timed ? 6 : 3;
		UIScript::Data::notiText = notification;
		return;
	}


	int playerMoney;
	STATS::STAT_GET_INT(stat, &playerMoney, -1);
	reward = SurvivalData::CurrentWave * 5000;

	if ((SurvivalData::CurrentWave == 10 && !playerDied) || SurvivalData::CurrentWave > 10)
	{
		int bonus = 0;
		int bonusMultiplier = 0;

		if (SurvivalData::CurrentWave % 10 == 0)
		{
			bonusMultiplier = SurvivalData::CurrentWave / 10;

			if (playerDied)
			{
				bonusMultiplier -= 1;
			}
		}
		else
		{
			int reminder = SurvivalData::CurrentWave % 10;
			bonusMultiplier = (SurvivalData::CurrentWave - reminder) / 10;
		}

		bonus = 100000 * bonusMultiplier;
		reward += bonus;
	}

	std::string notification = "";
	notification.append("Survival ended at wave ");
	notification.append(std::to_string(SurvivalData::CurrentWave).c_str());
	notification.append(". Reward: ~g~$");
	notification.append(std::to_string(reward).c_str());
	UIScript::Data::pendingNoti = true;
	UIScript::Data::scaleformType = SurvivalData::timed ? 6 : 3;
	UIScript::Data::notiText = notification;
	STATS::STAT_SET_INT(stat, playerMoney + reward, 1);
}

void SURVIVAL::CompleteSurvival()
{
	MUSIC::MissionCompletedSound();
	UIScript::Data::showScaleform = true;
	UIScript::Data::scaleformType = 1;
	GiveReward(false);
	PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(PLAYER::PLAYER_ID(), true);
	ENEMIES::ClearVectors();
	SURVIVAL::ClearVectors();
	PICKUPS::Delete();

	SurvivalData::IsActive = false;
	SurvivalData::Started = false;
	SurvivalData::cheated = false;

	if (SpawnerData::isXmas || SpawnerData::isHalloween)
	{
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		WAIT(0);
		GAMEPLAY::_SET_WEATHER_TYPE_OVER_TIME((char*)"CLEAR", 30);
		GAMEPLAY::_SET_RAIN_FX_INTENSITY(-1);
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	}

	TriggerDelayedSpawn();
}

void SURVIVAL::IncrementWave()
{
	SurvivalData::CurrentWave += 1;
}

void SURVIVAL::QuitSurvival(bool playerDied)
{
	if (playerDied)
	{
		GiveReward(true);
	}
	else
	{
		if (SurvivalData::timed && TIMERS::TimedSurvival::timeLeft <= 0)
			UIScript::Data::scaleformType = 6;
		else
			UIScript::Data::scaleformType = 2;

		UIScript::Data::showScaleform = true;
	}

	MUSIC::StopTrack();
	PICKUPS::Delete();
	ENEMIES::ClearVectors();
	SURVIVAL::ClearVectors();
	PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(PLAYER::PLAYER_ID(), true);

	SurvivalData::IsActive = false;
	SurvivalData::Started = false;
	SurvivalData::cheated = false;

	if (SpawnerData::isXmas || SpawnerData::isHalloween)
	{
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		WAIT(0);
		GAMEPLAY::_SET_WEATHER_TYPE_OVER_TIME((char*)"CLEAR", 30);
		GAMEPLAY::_SET_RAIN_FX_INTENSITY(-1);
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	}

	TriggerDelayedSpawn();
}

void SURVIVAL::ScriptQuit()
{
	MUSIC::StopTrack();
	PICKUPS::Delete();
	ENEMIES::ClearVectors();
	PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(PLAYER::PLAYER_ID(), true);

	SurvivalData::IsActive = false;
	SurvivalData::Started = false;
	SurvivalData::cheated = false;

	if (SpawnerData::isXmas || SpawnerData::isHalloween)
	{
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		WAIT(0);
		GAMEPLAY::_SET_WEATHER_TYPE_OVER_TIME((char*)"CLEAR", 30);
		GAMEPLAY::_SET_RAIN_FX_INTENSITY(-1);
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	}
}

void SURVIVAL::TriggerDelayedSpawn()
{
	size_t size = TriggerPedsData::names.size();

	for (size_t i = 0; i < size; i++)
	{
		if (TriggerPedsData::names.at(i) == SurvivalData::MissionID)
		{
			TriggerPedsData::timerActive.at(i) = true;
			TriggerPedsData::starTime.at(i) = GAMEPLAY::GET_GAME_TIMER();
			return;
		}
	}
}
