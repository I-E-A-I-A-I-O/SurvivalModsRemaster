#include <main.h>
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
	TASK::CLEAR_PED_TASKS(ped);
	TASK::TASK_GO_TO_ENTITY(ped, targetPed, -1, 2, 10, 1073741824.0f, 0);
	movingToPed = true;
}

void JESUS::Jesus::StartReviving()
{
	movingToPed = false;
	revivingPed = true;
	STREAMING::REQUEST_ANIM_DICT("missheistfbi3b_ig8_2");
	while (!STREAMING::HAS_ANIM_DICT_LOADED("missheistfbi3b_ig8_2"))
	{
		WAIT(250);
	}
	TASK::CLEAR_PED_TASKS(ped);
	TASK::TASK_PLAY_ANIM(ped, "missheistfbi3b_ig8_2", "cpr_loop_paramedic", 8, -8, 3250, 0, 0, 0, 0, 0);
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
	ENTITY::SET_ENTITY_HEALTH(targetPed, health, 0);
	ENTITY::SET_ENTITY_MAX_HEALTH(targetPed, health);
	TASK::CLEAR_PED_TASKS_IMMEDIATELY(targetPed);
}

bool JESUS::Jesus::CanRevive()
{
	if (!animTimerStarted)
	{
		animTimerStarted = true;
		animTimerStartTime = MISC::GET_GAME_TIMER();
		return false;
	}
	else
	{
		animTimerCurrentTime = MISC::GET_GAME_TIMER();

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
	STREAMING::REQUEST_ANIM_DICT("rcmcollect_paperleadinout@");
	while (!STREAMING::HAS_ANIM_DICT_LOADED("rcmcollect_paperleadinout@"))
	{
		WAIT(250);
	}
	TASK::CLEAR_PED_TASKS(ped);
	TASK::TASK_PLAY_ANIM(ped, "rcmcollect_paperleadinout@", "meditiate_idle", 8, -8, -1, 1, 0, 0, 0, 0);
}
