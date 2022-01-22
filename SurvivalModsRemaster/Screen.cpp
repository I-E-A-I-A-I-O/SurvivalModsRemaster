#include "pch.h"
#include "Screen.hpp"
#include <unordered_map>

struct Position
{
	float x;
	float y;

	Position(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

std::unordered_map<int, Position> badgeSlots = {
	{0, Position(0.91f, 0.97f)},
	{1, Position(0.91f, 0.918f)}
};

std::unordered_map<int, Position> contentSlots =
{
	{0, Position(0.75f, 0.952f)},
	{1, Position(0.75f, 0.90f)}
};

std::unordered_map<int, Position> titleSlots =
{
	{0, Position(0.855f, 0.96f)},
	{1, Position(0.855f, 0.91f)}
};

void SCREEN::ShowEnemyCountBadge(int count, int max, int wave)
{
	DrawBadge("WAVE", std::to_string(wave).c_str(), false, 1);
	char enemyCount[50];
	char title[50];
	bool red = false;

	if (count <= 0)
	{
		strcpy_s(title, "~r~ENEMIES");
		strcpy_s(enemyCount, "~r~OVERFLOW");
		red = true;
	}
	else
	{
		strcpy_s(title, "ENEMIES");
		strcpy_s(enemyCount, std::to_string(count).c_str());
		strcat_s(enemyCount, "/");
		strcat_s(enemyCount, std::to_string(max).c_str());
	}

	DrawBadge(title, enemyCount, red, 0);
}

void SCREEN::ShowIntermissionBadge(int time, int wave, bool timed)
{
	const char* colorModifier;
	bool red = false;

	if (time > 20 / 1.5f)
	{
		colorModifier = "~g~";
	}
	else if (time > 20 / 3)
	{
		colorModifier = "~y~";
	}
	else
	{
		colorModifier = "~r~";
		red = true;
	}

	char title[40];
	strcpy_s(title, colorModifier);

	if (timed)
	{
		strcat_s(title, "STARTING");
	}
	else
	{
		strcat_s(title, "WAVE ");
		strcat_s(title, std::to_string(wave).c_str());
	}

	strcat_s(title, " IN");
	char content[40];
	strcpy_s(content, colorModifier);
	strcat_s(content, std::to_string(time).c_str());
	DrawBadge(title, content, red, 0);
}

void SCREEN::ShowTimeLeftBadge(int time)
{
	const char* colorModifier;
	bool red = false;

	if (time > 40000 / 1.5f)
	{
		colorModifier = "~g~";
	}
	else if (time > 40000 / 3)
	{
		colorModifier = "~y~";
	}
	else
	{
		colorModifier = "~r~";
		red = true;
	}

	char title[40];
	strcpy_s(title, colorModifier);
	strcat_s(title, "TIME LEFT");
	char content[100];
	strcpy_s(content, colorModifier);

	int val = time / 1000 / 60;

	if (val < 10)
		strcat_s(content, "0");

	strcat_s(content, std::to_string(val).c_str());
	strcat_s(content, ":");
	val = time / 1000 % 60;

	if (val < 10)
		strcat_s(content, "0");

	strcat_s(content, std::to_string(val).c_str());
	DrawBadge(title, content, red, 0);
}

void SCREEN::ShowHelpTextThisFrame(const char* text, bool beep)
{
	UI::_SET_TEXT_COMPONENT_FORMAT((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((char*)text);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, beep, 0);
}

void SCREEN::ShowSubtitle(const char* text, int time)
{
	UI::_SET_TEXT_ENTRY_2((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((char*)text);
	UI::_DRAW_SUBTITLE_TIMED(time, true);
}

void SCREEN::ShowNotification(const char* text)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((char*)text);
	UI::_DRAW_NOTIFICATION(1, 1);
}

void SCREEN::DrawMarker(int type, Vector3 position, Vector3 scale)
{
	GRAPHICS::DRAW_MARKER(type, position.x, position.y, position.z, 0, 0, 1, 0, 0, 0, scale.x, scale.y, scale.z, 255, 255, 0, 100, false, false, 2, false, NULL, NULL, false);
}

void SCREEN::LoadSprites()
{
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)"timerbars", false);

	while (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)"timerbars"))
		WAIT(1);
}

void SCREEN::UnloadSprites()
{
	GRAPHICS::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED((char*)"timerbars");
}

void DrawSprite(float x, float y, bool red)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)"timerbars"))
		SCREEN::LoadSprites();

	GRAPHICS::DRAW_SPRITE((char*)"timerbars", (char*)"all_black_bg", x, y, 0.15f, 0.045f, 0, 0, 0, 0, 200);

	if (red)
		GRAPHICS::DRAW_SPRITE((char*)"timerbars", (char*)"all_red_bg", x, y, 0.15f, 0.045f, 0, 165, 15, 1, 255);
}

void DrawSpriteText(float x, float y, float scale, const char* text, bool right)
{
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((char*)text);
	if (right)
	{
		UI::SET_TEXT_WRAP(0.6f, 0.975f);
		UI::SET_TEXT_JUSTIFICATION(2);
	}
	UI::SET_TEXT_SCALE(1.0f, scale);
	UI::_DRAW_TEXT(x, y);
}

void SCREEN::DrawBadge(const char* title, const char* content, bool red, int slot)
{
	Position bPos = badgeSlots.at(slot);
	Position cPos = contentSlots.at(slot);
	Position tPos = titleSlots.at(slot);
	DrawSprite(bPos.x, bPos.y, red);
	DrawSpriteText(cPos.x, cPos.y, 0.42f, content, true);
	DrawSpriteText(tPos.x, tPos.y, 0.295f, title, false);
}

void SCREEN::ShowControls()
{
	char controlA[200];
	char controlB[200];
	strcpy_s(controlA, "Press ~");
	strcat_s(controlA, controlsNames[static_cast<int>(Data::infiniteWaveControl)]);
	strcat_s(controlA, "~ for endless survival\n");
	strcat_s(controlA, "Press ~");
	strcat_s(controlA, controlsNames[static_cast<int>(Data::timedSurvivalControl)]);
	strcat_s(controlA, "~ for timed");
	strcpy_s(controlB, "survival\n");
	strcat_s(controlB, "Press ~");
	strcat_s(controlB, controlsNames[static_cast<int>(Data::hardcoreSurvivalControl)]);
	strcat_s(controlB, "~ for harcore survival");
	UI::_SET_TEXT_COMPONENT_FORMAT((char*)"TWOSTRINGS");
	UI::_ADD_TEXT_COMPONENT_STRING(controlA);
	UI::_ADD_TEXT_COMPONENT_STRING(controlB);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, false, 0);
}

int SCREEN::RequestScaleform()
{
	int handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE((char*)"mp_big_message_freemode");

	while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(handle))
		WAIT(1);

	return handle;
}

void SCREEN::SetScaleformText(int scaleform, const char* title, const char* subtitle)
{
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleform, (char*)"SHOW_SHARD_CENTERED_TOP_MP_MESSAGE");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((char*)title);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((char*)subtitle);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(5);
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}

void SCREEN::SetScaleformTextPassed(int scaleform, const char* title, const char* subtitle)
{
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleform, (char*)"SHOW_SHARD_CENTERED_TOP_MP_MESSAGE");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((char*)title);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((char*)subtitle);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(100);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(true);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(true);
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}

void SCREEN::SetScaleformTextFailed(int scaleform, const char* title, const char* subtitle)
{
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleform, (char*)"SHOW_MISSION_END_MP_MESSAGE");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((char*)title);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((char*)subtitle);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(5);
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}

void SCREEN::FadeOutScaleform(int handle, int duration)
{
	GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(handle, (char*)"SHARD_ANIM_OUT");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(5);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(duration);
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(true);
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}

void SCREEN::FreeScaleform(int handle)
{
	GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&handle);
}
