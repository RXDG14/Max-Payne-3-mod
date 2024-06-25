#include <sstream>
#include <iomanip>
//#include "utils.h"
#include "enums.h"
#include "stdafx.h"

#pragma once

#define maxMenuItems 10

using namespace std;

enum Menu {
	MainMenu,
	EditMapMenu,
	LoadMapMenu,
};

int currentMenu = -1;
int currentMenuIndex;
int currentMenuItems;

int currentMenuMinItem;
int currentMenuMaxItem = maxMenuItems - 1;

int currentOldMenuIndex;
int oldMenuMinItem[20];
int oldMenuMaxItem[20];
int oldMenu[20];
int oldMenuIndex[20];
int oldMenuItems[20];

bool wasRecentMenuDown;
bool invokeMenuInit;
bool invokeMenuItemCount;

void OpenMenu(Menu menu)
{
	currentOldMenuIndex++;
	oldMenu[currentOldMenuIndex] = currentMenu;
	oldMenuIndex[currentOldMenuIndex] = currentMenuIndex;
	oldMenuItems[currentOldMenuIndex] = currentMenuItems;
	oldMenuMinItem[currentOldMenuIndex] = currentMenuMinItem;
	oldMenuMaxItem[currentOldMenuIndex] = currentMenuMaxItem;
	currentMenuMinItem = 0;
	currentMenuIndex = 0;
	currentMenu = (int)menu;
	invokeMenuInit = true;
	invokeMenuItemCount = true;
}

void PlayMenuSelectSound()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_SELECT");
}

void MenuGoUp()
{
	wasRecentMenuDown = false;
	AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
	currentMenuIndex -= 1;

	if (currentMenuIndex < 0)
	{
		currentMenuIndex = currentMenuItems - 1;
	}

	if (currentMenuItems < maxMenuItems)
		return;

	if (currentMenuIndex < currentMenuMinItem || currentMenuIndex > currentMenuMaxItem) {
		if (currentMenuIndex == currentMenuItems - 1) {
			currentMenuMinItem = currentMenuItems - 1 - (maxMenuItems - 1);
			currentMenuMaxItem = currentMenuItems - 1;
		}
		else {
			currentMenuMinItem--;
			currentMenuMaxItem--;
		}
	}
}

void MenuGoDown()
{
	wasRecentMenuDown = true;
	AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
	currentMenuIndex += 1;

	if (currentMenuIndex > currentMenuItems - 1)
	{
		currentMenuIndex = 0;
	}

	if (currentMenuItems < maxMenuItems)
		return;

	if (currentMenuIndex > currentMenuMaxItem || currentMenuIndex == 0) {
		if (currentMenuIndex == 0) {
			currentMenuMinItem = 0;
			currentMenuMaxItem = maxMenuItems - 1;
		}
		else {
			currentMenuMinItem++;
			currentMenuMaxItem++;
		}
	}
}

void MenuGoBack()
{
	if (currentMenu == 0) return;

	AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_BACK");
	if (currentMenu != 0)
	{
		invokeMenuInit = true;
	}
	currentMenuMinItem = oldMenuMinItem[currentOldMenuIndex];
	currentMenuMaxItem = oldMenuMaxItem[currentOldMenuIndex];
	currentMenuItems = oldMenuItems[currentOldMenuIndex];
	currentMenuIndex = oldMenuIndex[currentOldMenuIndex];
	currentMenu = oldMenu[currentOldMenuIndex];
	currentOldMenuIndex--;
}

bool useNumpad = true;
bool useGamepad = true;

void HandleMenuInput()
{
	if ((useNumpad && IsKeyDown(GameKey_Numpad8)))// || (!useNumpad && IsKeyDown(GameKey_UpArrow)) || (useGamepad && PAD::IS_USING_CONTROLLER && IsControlBeingPressed(INPUT_FRONTEND_UP)))
	{
		MenuGoUp();
	}
	else if ((useNumpad && IsKeyDown(GameKey_Numpad2)))// || (!useNumpad && IsGameKeyBeingPressed(GameKey_DownArrow)) || (useGamepad && PAD::IS_USING_CONTROLLER && IsControlBeingPressed(INPUT_FRONTEND_DOWN)))
	{
		MenuGoDown();
	}
	else if ((useNumpad && IsKeyDown(GameKey_Numpad0)))// || (!useNumpad && IsGameKeyBeingPressed(GameKey_Backspace)) || (useGamepad && PAD::IS_USING_CONTROLLER && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_CANCEL)))
	{
		MenuGoBack();
	}
}

void DrawMenuBackground()
{
	GRAPHICS::DRAW_RECT(0.17f, currentMenuItems < maxMenuItems ? 0.12f + ((currentMenuItems - 1) * 0.02f) : 0.12f + (maxMenuItems - 1) * 0.02f, 0.3f, currentMenuItems < maxMenuItems ? 0.19f + ((currentMenuItems - 1) * 0.04f) : 0.19f + (maxMenuItems - 1) * 0.04f, 0, 0, 0, 180);
	GRAPHICS::DRAW_RECT(0.17f, currentMenuItems < maxMenuItems ? 0.17f + ((currentMenuItems - 1) * 0.04f) : 0.17f + (maxMenuItems - 1) * 0.04f, 0.3f, 0.005f, 255, 255, 255, 255);

	HUD::SET_TEXT_FONT(4);
	HUD::SET_TEXT_SCALE(0.3f, 0.5f);
	HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	HUD::SET_TEXT_WRAP(0.0f, 0.31f);
	HUD::SET_TEXT_RIGHT_JUSTIFY(true);
	string text = "~m~(" + to_string(currentMenuIndex + 1) + "/" + to_string(currentMenuItems) + ")";
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.25f, currentMenuItems < maxMenuItems ? 0.18f + ((currentMenuItems - 1) * 0.04f) : 0.18f + (maxMenuItems - 1) * 0.04f, "STRING", (char *)text.c_str());

	//HUD::SET_TEXT_FONT(7);
	//HUD::SET_TEXT_SCALE(0.2f, 0.6f);
	//HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	//HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.03f, currentMenuItems < maxMenuItems ? 0.175f + ((currentMenuItems - 1) * 0.04f) : 0.175f + (maxMenuItems - 1) * 0.04f, "STRING", "~r~By jedijosh920 & Unknown Modder");
}

void DrawMenuHighlighter()
{
	GRAPHICS::DRAW_RECT(0.17f, 0.14f + ((currentMenuIndex - currentMenuMinItem) * 0.04f), 0.28f, 0.03f, 255, 255, 255, 255);
}

void AddTitle(char* text, float scale = 0.5f)
{
	HUD::SET_TEXT_FONT(1);
	HUD::SET_TEXT_SCALE(scale, 0.9f);
	HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.04f, 0.05f, "STRING", text);
}

void NullFunc() { return; }

void AddOption(char* text, int menuIndex, void(*func)() = NullFunc)
{
	if (menuIndex > currentMenuMaxItem || menuIndex < currentMenuMinItem)
	{
		return;
	}

	HUD::SET_TEXT_FONT(0);
	HUD::SET_TEXT_SCALE(0.25f, 0.6f);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);

	if (currentMenuIndex == menuIndex)
	{
		HUD::SET_TEXT_COLOUR(0, 0, 0, 255);

		if (func != NullFunc)
		{
			if (IsKeyDown(useNumpad ? GameKey_Numpad5 : GameKey_Numpad5))// || useGamepad && PAD::IS_USING_CONTROLLER() && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT))
			{
				PlayMenuSelectSound();
				func();
			}
		}
	}
	else
	{
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	}

	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.04f, 0.12f + ((menuIndex - currentMenuMinItem) * 0.04f), "STRING", text);
}

void AddInfoOption(char* text, int menuIndex)
{
	AddOption(text, menuIndex);

	if (menuIndex > currentMenuMaxItem || menuIndex < currentMenuMinItem)
	{
		return;
	}

	if (currentMenuIndex == menuIndex)
	{
		if (wasRecentMenuDown)
		{
			MenuGoDown();
		}
		else
		{
			MenuGoUp();
		}

	}
}

void AddBoolOption(char* text, bool &value, int menuIndex, void(*func)() = NullFunc)
{
	AddOption(text, menuIndex);

	if (menuIndex > currentMenuMaxItem || menuIndex < currentMenuMinItem)
	{
		return;
	}

	if (currentMenuIndex == menuIndex)
	{
		if (IsKeyDown(useNumpad ? GameKey_Numpad5 : GameKey_Numpad5))// || useGamepad && PAD::IS_USING_CONTROLLER() && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_TOGGLE_ON");
			value = !value;

			if (func != NullFunc)
			{
				func();
			}
		}
	}

	HUD::SET_TEXT_FONT(0);
	HUD::SET_TEXT_SCALE(0.25f, 0.6f);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	HUD::SET_TEXT_WRAP(0.0f, 0.31f);
	HUD::SET_TEXT_RIGHT_JUSTIFY(true);
	string toggleText = value ? "~g~On" : "~r~Off";
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.0f, 0.12f + ((menuIndex - currentMenuMinItem) * 0.04f), "STRING", (char *)toggleText.c_str());
}

void AddIntOption(char* text, int min, int max, int &value, int menuIndex, void(*func)() = NullFunc)
{
	AddOption(text, menuIndex);

	if (menuIndex > currentMenuMaxItem || menuIndex < currentMenuMinItem)
	{
		return;
	}

	if (currentMenuIndex == menuIndex)
	{
		if (IsKeyDown(useNumpad ? GameKey_Numpad4 : GameKey_Numpad4))// || useGamepad && PAD::IS_USING_CONTROLLER() && IsControlBeingPressed(INPUT_FRONTEND_LEFT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
			value -= 1;

			if (value < min)
			{
				value = max;
			}
		}
		else if (IsKeyDown(useNumpad ? GameKey_Numpad6 : GameKey_Numpad6))// || useGamepad && PAD::IS_USING_CONTROLLER() && IsControlBeingPressed(INPUT_FRONTEND_RIGHT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
			value += 1;

			if (value > max)
			{
				value = min;
			}
		}
		else if (IsKeyDown(useNumpad ? GameKey_Numpad5 : GameKey_Numpad5))// || useGamepad && PAD::IS_USING_CONTROLLER() && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT))
		{
			if (func != NullFunc)
			{
				PlayMenuSelectSound();
				func();
			}
		}
	}

	HUD::SET_TEXT_FONT(0);
	HUD::SET_TEXT_SCALE(0.25f, 0.6f);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	if (currentMenuIndex == menuIndex)
	{
		HUD::SET_TEXT_COLOUR(0, 0, 0, 255);
	}
	else
	{
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	}
	HUD::SET_TEXT_WRAP(0.0f, 0.31f);
	HUD::SET_TEXT_RIGHT_JUSTIFY(true);
	string valueText = (currentMenuIndex == menuIndex) ? "< " + to_string(value) + " >" : to_string(value);
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.0f, 0.12f + ((menuIndex - currentMenuMinItem) * 0.04f), "STRING", (char *)valueText.c_str());
}

void AddFloatOption(char* text, float min, float max, float &value, int menuIndex, void(*func)() = NullFunc)
{
	AddOption(text, menuIndex);

	if (menuIndex > currentMenuMaxItem || menuIndex < currentMenuMinItem)
	{
		return;
	}

	if (currentMenuIndex == menuIndex)
	{
		if (IsKeyDown(useNumpad ? GameKey_Numpad4 : GameKey_Numpad4))// || useGamepad && PAD::IS_USING_CONTROLLER() && IsControlBeingPressed(INPUT_FRONTEND_LEFT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
			value -= 0.01f;

			if (value < min)
			{
				value = max;
			}
		}
		else if (IsKeyDown(useNumpad ? GameKey_Numpad6 : GameKey_Numpad6))// || useGamepad && PAD::IS_USING_CONTROLLER() && IsControlBeingPressed(INPUT_FRONTEND_RIGHT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
			value += 0.01f;

			if (value > max)
			{
				value = min;
			}
		}
		else if (IsKeyDown(useNumpad ? GameKey_Numpad5 : GameKey_Numpad5))// || useGamepad && PAD::IS_USING_CONTROLLER() && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT))
		{
			if (func != NullFunc)
			{
				PlayMenuSelectSound();
				func();
			}
		}
	}

	HUD::SET_TEXT_FONT(0);
	HUD::SET_TEXT_SCALE(0.25f, 0.6f);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	if (currentMenuIndex == menuIndex)
	{
		HUD::SET_TEXT_COLOUR(0, 0, 0, 255);
	}
	else
	{
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	}
	HUD::SET_TEXT_WRAP(0.0f, 0.31f);
	HUD::SET_TEXT_RIGHT_JUSTIFY(true);

	stringstream stream;
	stream << fixed << setprecision(2) << value;

	string valueText = (currentMenuIndex == menuIndex) ? "< " + stream.str() + " >" : stream.str();
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.0f, 0.12f + ((menuIndex - currentMenuMinItem) * 0.04f), "STRING", (char *)valueText.c_str());
}

void AddListOption(char* text, string list[], const int count, int& index, int menuIndex, void(*func)() = NullFunc)
{
	AddOption(text, menuIndex);

	if (menuIndex > currentMenuMaxItem || menuIndex < currentMenuMinItem)
	{
		return;
	}

	if (currentMenuIndex == menuIndex)
	{
		if (IsKeyDown(useNumpad ? GameKey_Numpad4 : GameKey_Numpad4))// || useGamepad && PAD::IS_USING_CONTROLLER() && IsKeyDown(INPUT_FRONTEND_LEFT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
			index -= 1;

			if (index < 0)
			{
				index = count - 1;
			}
		}
		else if (IsKeyDown(useNumpad ? GameKey_Numpad6 : GameKey_Numpad6))// || useGamepad && PAD::IS_USING_CONTROLLER() && IsKeyDown(INPUT_FRONTEND_RIGHT))
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, "FRONTEND_MENU_HIGHLIGHT_DOWN_UP");
			index += 1;

			if (index > count - 1)
			{
				index = 0;
			}
		}
		else if (IsKeyDown(useNumpad ? GameKey_Numpad5 : GameKey_Numpad5))// || useGamepad && PAD::IS_USING_CONTROLLER() && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT))
		{
			if (func != NullFunc)
			{
				PlayMenuSelectSound();
				func();
			}
		}
	}

	HUD::SET_TEXT_FONT(0);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	if (currentMenuIndex == menuIndex)
	{
		HUD::SET_TEXT_COLOUR(0, 0, 0, 255);
	}
	else
	{
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
	}
	HUD::SET_TEXT_WRAP(0.0f, 0.31f);
	HUD::SET_TEXT_RIGHT_JUSTIFY(true);
	string valueText = (currentMenuIndex == menuIndex) ? "< " + list[index] + " >" : list[index];
	int strLen = HUD::GET_LENGTH_OF_LITERAL_STRING((char *)valueText.c_str());
	if (strLen < 14)
	{
		HUD::SET_TEXT_SCALE(0.25f, 0.6f);
	}
	else if (strLen < 21)
	{
		HUD::SET_TEXT_SCALE(0.21f, 0.6f);
	}
	else if (strLen < 28)
	{
		HUD::SET_TEXT_SCALE(0.18f, 0.6f);
	}
	else
	{
		HUD::SET_TEXT_SCALE(0.15f, 0.6f);
	}
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.0f, 0.12f + ((menuIndex - currentMenuMinItem) * 0.04f), "STRING", (char *)valueText.c_str());
}

void InitMenu(const int count)
{
	if (invokeMenuItemCount)
	{
		currentMenuItems = count;
		if (currentMenuItems > maxMenuItems) {
			currentMenuMaxItem = maxMenuItems - 1;
		}
		else {
			currentMenuMaxItem = currentMenuItems - 1;
		}
		invokeMenuItemCount = false;
	}

	if (invokeMenuInit)
		invokeMenuInit = false;
}
