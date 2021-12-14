#pragma once

#include "natives.h"

class Timers
{
private:
    bool spawnTimerStarted;
    int spawnCurrentTime;
    int spawnStartTime;

public:
    Timers();
    bool SpawnTimer();
};
