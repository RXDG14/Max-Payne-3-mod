// simple example

#include "stdafx.h"
#include "main.h"

#include "script.h"
#include "menu.h"

#include <string>
#include <ctime>

#pragma warning(disable : 4244 4305) // double <-> float conversions

// Uncomment this if you 
// don't want to use native namespaces.
// USING_NATIVE_NAMESPACES

#define MAX_PAINKILLERS 9
#define MAX_AMMO 25000

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
{
	// default values
	int text_col[4] = { 255, 255, 255, 255 },
		rect_col[4] = { 70, 95, 95, 255 };
	float text_scale = 0.35;
	int font = 0;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;

		rect_col[0] = 218;
		rect_col[1] = 242;
		rect_col[2] = 216;

		if (rescaleText) text_scale = 0.40;
	}

	if (title)
	{
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;

		if (rescaleText) text_scale = 0.50;
		font = 1;
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	HUD::SET_TEXT_FONT(font);
	HUD::SET_TEXT_SCALE(0.3, text_scale);
	HUD::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	HUD::SET_TEXT_CENTRE(0);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	//HUD::_SET_TEXT_ENTRY("STRING");
	//HUD::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	//HUD::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f), "STRING", (LPSTR)caption.c_str());

	// text lower part
	HUD::SET_TEXT_FONT(font);
	HUD::SET_TEXT_SCALE(0.3, text_scale);
	HUD::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	HUD::SET_TEXT_CENTRE(0);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	//HUD::_SET_TEXT_GXT_ENTRY("STRING");
	//HUD::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	//int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f), "STRING", (LPSTR)caption.c_str());

	// rect
	//draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		//lineWidthScaled, ((((float)(num25)*UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f),
		//rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		lineWidthScaled, 0.05f, rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}

bool trainer_switch_pressed()
{
	return IsKeyJustUp(VK_F4);
}

void get_button_state(bool* a, bool* b, bool* up, bool* down, bool* l, bool* r)
{
	if (a) *a = IsKeyDown(VK_NUMPAD5);
	if (b) *b = IsKeyDown(VK_NUMPAD0) || trainer_switch_pressed() || IsKeyDown(VK_BACK);
	if (up) *up = IsKeyDown(VK_NUMPAD8);
	if (down) *down = IsKeyDown(VK_NUMPAD2);
	if (r) *r = IsKeyDown(VK_NUMPAD6);
	if (l) *l = IsKeyDown(VK_NUMPAD4);
}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN");
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		HUD::SET_TEXT_FONT(0);
		HUD::SET_TEXT_SCALE(0.55, 0.55);
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
		HUD::SET_TEXT_WRAP(0.0, 1.0);
		HUD::SET_TEXT_CENTRE(1);
		HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		HUD::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			//UI::_SET_TEXT_ENTRY((char*)statusText.c_str());
			HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.5, 0.5, "STRING", (char*)statusText.c_str());
		}
		else
		{
			//UI::_SET_TEXT_ENTRY("STRING");
			//UI::_ADD_TEXT_COMPONENT_STRING((char*)statusText.c_str());
			//HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.5, 0.5, "STRING", (char*)statusText.c_str());
		}
		//UI::_DRAW_TEXT(0.5, 0.5);
		//HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.5, 0.5, "STRING", (char*)statusText.c_str());
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

//features
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;

std::string line_as_str(std::string text, bool* pState)
{
	while (text.size() < 18) text += " ";
	return text + (pState ? (*pState ? " [ON]" : " [OFF]") : "");
}

void update_features()
{
	update_status_text();

	// wait until player is ready, basically to prevent using the trainer while player is dead or arrested
	//while (PLAYER::IS_PLAYER_DEAD(PLAYER::GET_PLAYER_ID()), TRUE)
		//scriptWait(0);

	// common variables
	Player player = PLAYER::GET_PLAYER_ID();
	Ped playerPed = PLAYER::GET_PLAYER_PED(player);
	BOOL bPlayerExists = PLAYER::DOES_MAIN_PLAYER_EXIST();

	// player invincible
	if (featurePlayerInvincibleUpdated)
	{
		if (bPlayerExists && !featurePlayerInvincible)
			PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
		featurePlayerInvincibleUpdated = false;
	}
	if (featurePlayerInvincible)
	{
		if (bPlayerExists)
			PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	}

}

int activeLineIndexMain = 0;
void process_main_menu()
{
	const float lineWidth = 300.0;
	const int lineCount = 7;

	std::string caption = "MP3 BASIC MOD";

	static struct 
	{
		LPCSTR text;
		bool* pState;
		bool* pUpdated;
	}
	lines[lineCount] = {
		{"INVINCIBLE", &featurePlayerInvincible, &featurePlayerInvincibleUpdated},
		{"WEAPON", NULL, NULL},
		{"VEHICLE", NULL, NULL },
		{"WORLD", NULL, NULL},
		{"TIME", NULL, NULL},
		{"WEATHER", NULL, NULL},
		{"MISC", NULL, NULL}
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
			{
				if (i != activeLineIndexMain)
				{
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 9.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
				}
			}
			draw_menu_line(line_as_str(lines[activeLineIndexMain].text, lines[activeLineIndexMain].pState), lineWidth + 1.0, 11.0, 56.0 + activeLineIndexMain * 36.0, 0.0, 7.0, true, false);
			update_features();
			scriptWait(0);
		} 
		while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMain)
			{
			case 0:
				featurePlayerInvincible = !featurePlayerInvincible;
				if (featurePlayerInvincible)
				{
					PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::GET_PLAYER_ID(), true);
					PLAYER::ADD_PAYNEKILLER(1);
				}
				break;
			case 1:
				//process_weapon_menu();
				break;
			case 2:
				//process_veh_menu();
				break;
			case 3:
				//process_world_menu();
				break;
			case 4:
				//process_time_menu();
				break;
			case 5:
				//process_weather_menu();
				break;
			case 6:
				//process_misc_menu();
				break;
			default:
				if (lines[activeLineIndexMain].pState)
					*lines[activeLineIndexMain].pState = !(*lines[activeLineIndexMain].pState);
				if (lines[activeLineIndexMain].pUpdated)
					*lines[activeLineIndexMain].pUpdated = true;
				set_status_text(lines[activeLineIndexMain].text);
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexMain == 0)
						activeLineIndexMain = lineCount;
					activeLineIndexMain--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexMain++;
						if (activeLineIndexMain == lineCount)
							activeLineIndexMain = 0;
						waitTime = 150;
					}
	}
}

void process_menu_menu()
{
	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			DrawMenuBackground();
			InitMenu(4);

			AddOption("option 1", 0, NullFunc);
			AddOption("option 2", 1, NullFunc);
			AddOption("option 3", 2, NullFunc);
			AddOption("option 4", 3, NullFunc);
			

			HUD::SET_TEXT_FONT(1);
			HUD::SET_TEXT_SCALE(0.25f, 0.6f);
			HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
			HUD::SET_TEXT_COLOUR(255, 255, 255, 255);

			HUD::DISPLAY_TEXT(0.3f, 0.3f, "abcdefg");
			HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.2f,0.2f, "STRING", "text 1");
			HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.5f, 0.5f, "STRING", "text 1");


			//
			scriptWait(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;
		if (trainer_switch_pressed())
		{
			menu_beep();
			break;
		}
	}
}

void main()
{
	while (true)
	{
		if (trainer_switch_pressed())
		{
			menu_beep();
			process_main_menu();
			//process_menu_menu();
			//Explode();
			//AddWeapons();
			//WantedLevel();
			//SpawnVehicle();
			//SpawnRandomNPC();
			//TeleportPlayer();
			//InvinciblePlayer();
			//SuperJump();
			//SuperSpeed();
			//vehicleSpeedBoost();
			//AddMoney();
		}
		if (IsKeyDown(VK_F8)) 
		{
			menu_beep();
			PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::GET_PLAYER_ID(), true);
		}
		if (IsKeyDown(VK_F9))
		{
			HUD::PRINT("print", 100, 100);
			HUD::PRINT_HELP("print help");
			HUD::PRINT_NOW("print now", 100, 100);
		}
		//update_features();
		scriptWait(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}