#include "pch.h"
#include "Screen.hpp"

void SCREEN::ShowEnemyCountHelpText(int count, int max, int wave, int altCount)
{
	const char* colorModifier;

	if (count <= 0)
	{
		count = altCount;
		max += altCount;
	}

	if (count > max / 1.5f)
	{
		colorModifier = "~r~";
	}
	else if (count > max / 3)
	{
		colorModifier = "~y~";
	}
	else
	{
		colorModifier = "~g~";
	}

	char text[200];
	strcpy_s(text, "~h~Wave: ");
	strcat_s(text, std::to_string(wave).c_str());
	strcat_s(text, "\nEnemies: ");
	strcat_s(text, colorModifier);
	strcat_s(text, std::to_string(count).c_str());
	strcat_s(text, "~w~");
	strcat_s(text, "/");
	strcat_s(text, std::to_string(max).c_str());
	UI::_SET_TEXT_COMPONENT_FORMAT((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((char*)text);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, false, -1);
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
