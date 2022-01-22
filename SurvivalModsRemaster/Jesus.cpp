#include "pch.h"
#include "Jesus.hpp"

JESUS::Jesus::Jesus()
{
	movingToPed = false;
	revivingPed = false;
	waiting = true;
	targetPed = 0;
	ped = 0;
}

void JESUS::Jesus::SetHandle(Ped handle)
{
	ped = handle;
}

JESUS::Jesus::Jesus(Ped handle)
{
	movingToPed = false;
	revivingPed = false;
	waiting = true;
	targetPed = 0;
	ped = handle;

	StartWaiting();
}

void JESUS::Jesus::SetTarget(Ped target)
{
	targetPed = target;
}

void JESUS::Jesus::MoveToTarget()
{
	waiting = false;
	AI::CLEAR_PED_TASKS(ped);
	AI::TASK_GO_TO_ENTITY(ped, targetPed, -1, 2, 10, 1073741824, 0);
	movingToPed = true;
}

void JESUS::Jesus::StartReviving()
{
	movingToPed = false;
	revivingPed = true;
	STREAMING::REQUEST_ANIM_DICT((char*)"missheistfbi3b_ig8_2");
	while (!STREAMING::HAS_ANIM_DICT_LOADED((char*)"missheistfbi3b_ig8_2"))
	{
		WAIT(250);
	}
	AI::CLEAR_PED_TASKS(ped);
	AI::TASK_PLAY_ANIM(ped, (char*)"missheistfbi3b_ig8_2", (char*)"cpr_loop_paramedic", 8, -8, 3250, 0, 0, 0, 0, 0);
}

void JESUS::Jesus::ReviveTarget()
{
	revivingPed = false;
	animTimerStarted = false;
	waiting = true;
	StartWaiting();
	int health = ENTITY::GET_ENTITY_MAX_HEALTH(targetPed);
	ENTITY::SET_ENTITY_COLLISION(targetPed, true, false);
	PED::RESURRECT_PED(targetPed);
	ENTITY::SET_ENTITY_HEALTH(targetPed, health);
	ENTITY::SET_ENTITY_MAX_HEALTH(targetPed, health);
	AI::CLEAR_PED_TASKS_IMMEDIATELY(targetPed);
}

bool JESUS::Jesus::CanRevive()
{
	if (!animTimerStarted)
	{
		animTimerStarted = true;
		animTimerStartTime = GAMEPLAY::GET_GAME_TIMER();
		return false;
	}
	else
	{
		animTimerCurrentTime = GAMEPLAY::GET_GAME_TIMER();

		if (animTimerCurrentTime - animTimerStartTime >= 3000)
		{
			return true;
		}
		return false;
	}
}

bool JESUS::Jesus::HasTarget() const
{
	return targetPed != 0;
}

bool JESUS::Jesus::IsInRange() const
{
	return CALC::IsInRange_2(ENTITY::GET_ENTITY_COORDS(ped, true), ENTITY::GET_ENTITY_COORDS(targetPed, true), 2);
}

void JESUS::Jesus::StartWaiting()
{
	waiting = true;
	STREAMING::REQUEST_ANIM_DICT((char*)"rcmcollect_paperleadinout@");
	while (!STREAMING::HAS_ANIM_DICT_LOADED((char*)"rcmcollect_paperleadinout@"))
	{
		WAIT(250);
	}
	AI::CLEAR_PED_TASKS(ped);
	AI::TASK_PLAY_ANIM(ped, (char*)"rcmcollect_paperleadinout@", (char*)"meditiate_idle", 8, -8, -1, 1, 0, 0, 0, 0);
}
