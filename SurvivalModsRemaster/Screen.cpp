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

void DrawSprite(float x, float y, bool red)
{
    if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED("timerbars"))
        SCREEN::LoadSprites();

    GRAPHICS::DRAW_SPRITE("timerbars", "all_black_bg", x, y, 0.15f, 0.045f, 0, 0, 0, 0, 150, 0, 0);

    if (red)
        GRAPHICS::DRAW_SPRITE("timerbars", "all_red_bg", x, y, 0.15f, 0.045f, 0, 165, 15, 1, 150, 0, 0);
}

void DrawSpriteText(float x, float y, float scale, const char* text, bool right)
{
    HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
    if (right)
    {
        HUD::SET_TEXT_WRAP(0.6f, 0.975f);
        HUD::SET_TEXT_JUSTIFICATION(2);
    }
    HUD::SET_TEXT_SCALE(1.0f, scale);
    HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
}

void DrawBar(float x, float y, float percentage) {
    float barWidth = 0.1f * percentage;
    float diff = (0.1f - barWidth) / 2;
    float offsetPercentage = 0.05f;

    if (percentage > 0.6f)
        offsetPercentage = 0.01f;

    float fillOffset = diff - (diff * offsetPercentage);
    GRAPHICS::DRAW_SPRITE("timerbars", "damagebar_128", x + 0.18f, y + 0.016f, 0.1f, 0.03f, 0, 0, 0, 0, 255, 1, 0);
    GRAPHICS::DRAW_SPRITE("timerbars", "damagebarfill_128", x + 0.18f - fillOffset, y + 0.016f, barWidth, 0.029f, 0, 255, 255, 255, 255, 1, 0);
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

void SCREEN::DrawBadge(const char* title, float percentage, int slot)
{
    Position bPos = badgeSlots.at(slot);
    Position cPos = contentSlots.at(slot);
    Position tPos = titleSlots.at(slot);
    DrawSprite(bPos.x, bPos.y, false);
    DrawBar(cPos.x, cPos.y, percentage);
    DrawSpriteText(tPos.x - 0.025f, tPos.y, 0.295f, title, false);
}

void SCREEN::ShowEnemyCountBadge(int count, int max, int wave)
{
	char title[50];
    strcpy_s(title, "WAVE ");
    strcat_s(title, std::to_string(wave).c_str());
    float percentage = static_cast<float>(count) / static_cast<float>(max);

    if (percentage > 1.0f)
        percentage = 1.0f;

	DrawBadge(title, percentage, 0);
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

void SCREEN::ShowHelpText(const char* text, bool beep)
{
    if (HUD::IS_HELP_MESSAGE_BEING_DISPLAYED())
        return;

	HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, beep, -1);
}

void SCREEN::ShowSubtitle(const char* text, int time)
{
    HUD::BEGIN_TEXT_COMMAND_PRINT("CELL_EMAIL_BCON");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
    HUD::END_TEXT_COMMAND_PRINT(time, true);
}

void SCREEN::ShowNotification(const char* text)
{
	HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(1, 1);
}

void SCREEN::LoadSprites()
{
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT("timerbars", false);

	while (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED("timerbars"))
		WAIT(1);
}

void SCREEN::UnloadSprites()
{
	GRAPHICS::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED("timerbars");
}

void SCREEN::ShowControls()
{
    if (HUD::IS_HELP_MESSAGE_BEING_DISPLAYED())
        return;

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
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("TWOSTRINGS");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(controlA);
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(controlB);
	HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, false, true, -1);
}

int SCREEN::RequestScaleform()
{
	int handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("mp_big_message_freemode");

	while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(handle))
		WAIT(1);

	return handle;
}

int CreateWallBG(bool doCash, bool time) {
    int handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("mp_celebration_bg");

    while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(handle))
        WAIT(1);

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handle, "CREATE_STAT_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING("HUD_COLOUR_BLACK");
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handle, "ADD_WAVE_REACHED_TO_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING("a");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING("CELEB_FAILED");
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    if (time) {
        GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handle, "ADD_TIME_TO_WALL");
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(10000);
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING("CELEB_TIME");
        GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
    }

    if (doCash) {
        GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handle, "ADD_CASH_TO_WALL");
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5000);
        GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
    }

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handle, "ADD_BACKGROUND_TO_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(75);
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(2);
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    return handle;
}

int CreateWallFG(int cash, int wave, int milis, bool passed, bool doCash, bool isWave, bool time) {
    char text[70];
    char label[70];

    if (passed || time) {
        strcpy_s(text, "PASSED");
    } else {
        strcpy_s(text, "WAVE ");
        strcat_s(text, std::to_string(wave).c_str());
    }

    if (passed || time) {
        strcpy_s(label, "CELEB_SURVIVAL");
    } else {
        if (isWave) {
            strcpy_s(label, "CELEB_SURVIVED");
        } else {
            strcpy_s(label, "CELEB_REACHED");
        }
    }

    int handleB = GRAPHICS::REQUEST_SCALEFORM_MOVIE("mp_celebration");

    while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(handleB))
        WAIT(1);

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handleB, "CREATE_STAT_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(6);
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handleB, "ADD_WAVE_REACHED_TO_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(6);
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(text);
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(label);
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    if (time) {
        GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handleB, "ADD_TIME_TO_WALL");
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(6);
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(milis);
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING("CELEB_TIME");
        GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
    }

    if (doCash) {
        GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handleB, "ADD_CASH_TO_WALL");
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(6);
        GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(cash);
        GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
    }

    return handleB;
}

std::vector<int> SCREEN::LoadWallStat(int cash, int wave, int milis, bool passed, bool doCash, bool isWave, bool time) {
    int fgHandle = CreateWallFG(cash, wave, milis, passed , doCash, isWave, time);
    int bgHandle = CreateWallBG(doCash, time);

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(bgHandle, "SHOW_STAT_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(fgHandle, "SHOW_STAT_WALL");
    GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(6);
    GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

    return std::vector<int> { bgHandle, fgHandle };
}

void SCREEN::SetScaleformText(int scaleform, const char* title, const char* subtitle)
{
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(scaleform, "SHOW_SHARD_CENTERED_TOP_MP_MESSAGE");
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(title);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(subtitle);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
}

void SCREEN::SetScaleformTextPassed(int scaleform, const char* title, const char* subtitle)
{
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(scaleform, "SHOW_SHARD_CENTERED_TOP_MP_MESSAGE");
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(title);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(subtitle);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(100);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(true);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(true);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
}

void SCREEN::FadeOutScaleform(int handle, int duration)
{
	GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(handle, "SHARD_ANIM_OUT");
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(duration);
	GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(true);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
}

void SCREEN::FreeScaleform(int handle)
{
	GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&handle);
}
