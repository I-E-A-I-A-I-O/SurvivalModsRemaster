#pragma once

#include "natives.h"
#include "Survival.hpp"
#include "Intermission.hpp"
#include <string>

namespace TIMERS
{
    struct TriggerPed
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    struct InterMission
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    struct LeavingZone
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    struct SpawnEnemy
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    struct SpawnVehicle
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    struct SpawnAircraft
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    struct TimedSurvival
    {
        static int StartTime;
        static int CurrentTime;
        static int InterWaveTime;
        static bool Started;
    };

    struct DogTimer
    {
        static int StartTime;
        static int CurrentTime;
        static bool Started;
    };

    void RestartTimers();
    void RestartLeavingZoneTimer();
    void RestartSpawnTimers();
    bool LeavingZoneTimerStarted();
    bool ProcessIntermissionTimer();
    bool ProcessLeavingZoneTimer();
    bool ProcessSpawnMoreTimer();
    bool ProcessAircraftTimer();
    bool ProcessVehicleTimer();
    bool ProcessTriggerPedTimer();
    void RestartTriggerPedTimer();
    bool ProcessTimedSurvivalTimer(int endTime);
    void RestartIntermissionTimer();
    void RestartVehicleTimer();
    void RestartAircraftTimer();
    void RestartEnemyTimer();
    bool ProcessDogTimer();
    void RestartDogTimer();
}
