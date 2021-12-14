#include "pch.h"
#include "Intermission.hpp"

bool INTERMISSION::Data::Active;

void INTERMISSION::Start()
{
    Data::Active = true;

    if ((SURVIVAL::SurvivalData::CurrentWave > 0 && (SURVIVAL::SurvivalData::CurrentWave < 9 || SURVIVAL::SurvivalData::InfiniteWaves)) || SURVIVAL::SurvivalData::timed)
    {
        MUSIC::LowIntensityTrack();
    }
    else
    {
        MUSIC::StopTrack();
        MUSIC::FinalWaveMusic();
    }
}

void INTERMISSION::Process()
{
    if (!TIMERS::ProcessIntermissionTimer() && !SURVIVAL::SurvivalData::timed)
    {
        return;
    }
    else if (SURVIVAL::SurvivalData::CurrentWave == 0 && SURVIVAL::SurvivalData::timed && !TIMERS::ProcessIntermissionTimer())
    {
        return;
    }

    if (!SURVIVAL::SurvivalData::timed || (SURVIVAL::SurvivalData::timed && SURVIVAL::SurvivalData::CurrentWave == 0))
    {
        TIMERS::RestartIntermissionTimer();
    }

    if (SURVIVAL::SurvivalData::CurrentWave > 0)
    {
        PICKUPS::Regenerate();
    }

    TIMERS::RestartSpawnTimers();
    SURVIVAL::IncrementWave();
    ENEMIES::ResetCounters();
    SURVIVAL::SurvivalData::MaxWaveSize = 10 + (5 * SURVIVAL::SurvivalData::CurrentWave);
    SURVIVAL::UpdateModels(SURVIVAL::SurvivalData::CurrentWave);

    if (SURVIVAL::SurvivalData::CurrentWave <= 6)
    {
        MUSIC::MidIntensityTrack(SURVIVAL::SurvivalData::CurrentWave);
    }
    else if (SURVIVAL::SurvivalData::CurrentWave < 10 || SURVIVAL::SurvivalData::InfiniteWaves || SURVIVAL::SurvivalData::timed)
    {
        MUSIC::HighIntensityTrack(SURVIVAL::SurvivalData::CurrentWave);
    }

    switch (SURVIVAL::SurvivalData::CurrentWave)
    {
        case 3:
        {
            SURVIVAL::SurvivalData::MaxVehicles = SURVIVAL::SurvivalData::timed ? 1 : 2;
            break;
        }
        case 5:
        {
            SURVIVAL::SurvivalData::MaxAircraft = 1;
            SURVIVAL::SurvivalData::MaxVehicles = SURVIVAL::SurvivalData::timed ? 2 : 3;
            break;
        }
        case 7:
        {
            SURVIVAL::SurvivalData::MaxAircraft = 2;
            SURVIVAL::SurvivalData::MaxVehicles = SURVIVAL::SurvivalData::timed ? 3 : 4;
            break;
        }
        case 9:
        {
            SURVIVAL::SurvivalData::MaxAircraft = SURVIVAL::SurvivalData::timed ? 2 : 3;
            SURVIVAL::SurvivalData::MaxVehicles = SURVIVAL::SurvivalData::timed ? 3 : 5;
            break;
        }
    }

    INTERMISSION::Data::Active = false;
}
