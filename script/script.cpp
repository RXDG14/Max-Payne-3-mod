#include "stdafx.h"
#include "main.h"

#include "script.h"

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

		if (rescaleText) text_scale = 0.4;
	}

	if (title)
	{
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;

		if (rescaleText) text_scale = 0.45;
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
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f), "STRING", (LPSTR)caption.c_str());

	// text lower part
	HUD::SET_TEXT_FONT(font);
	HUD::SET_TEXT_SCALE(0.3, text_scale);
	HUD::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	HUD::SET_TEXT_CENTRE(0);
	HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f), "STRING", (LPSTR)caption.c_str());

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		lineWidthScaled, 0.03f, rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
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
		HUD::SET_TEXT_RIGHT_JUSTIFY(true);
		if (statusTextGxtEntry)
		{
			HUD::DISPLAY_TEXT_WITH_LITERAL_STRING(0.5, 0.5, "STRING", (char*)statusText.c_str());
		}
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

//features
bool PlayerInvincible = false;
bool PlayerInvincibleUpdated = false;
bool SuperSpeed = false;
bool SuperSpeedUpdated = false;
bool NoReload = false;
bool NoReloadUpdated = false;
bool UnlimitedBulletTime = false;
bool ExplosiveBullets = false;

std::string line_as_str(std::string text, bool* pState)
{
	while (text.size() < 18) text += " ";
	return text + (pState ? (*pState ? " [ON]" : " [OFF]") : "");
}

int activeLineIndexMain = 0;
void process_main_menu()
{
	const float lineWidth = 500.0;
	const int lineCount = 6;

	std::string caption = "MP3 BASIC MOD";

	static struct 
	{
		LPCSTR text;
		bool* pState;
		bool* pUpdated;
	}
	lines[lineCount] = {
		{"INVINCIBLE", &PlayerInvincible, &PlayerInvincibleUpdated},
		{"SUPER SPEED", &SuperSpeed, &SuperSpeedUpdated},
		{"GET WEAPONS", NULL, NULL },
		{"NO RELOAD", &NoReload, NULL},
		{"UNLIMITED BULLET TIME", &UnlimitedBulletTime, NULL},
		{"EXPLOSIVE BULLETS", &ExplosiveBullets, NULL}
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 15, 23, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
			{
				if (i != activeLineIndexMain)
				{
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 10.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
				}
			}
			draw_menu_line(line_as_str(lines[activeLineIndexMain].text, lines[activeLineIndexMain].pState), lineWidth, 9.0, 60.0 + activeLineIndexMain * 36.0, 0.0, 9.0, true, false);
			update_status_text();
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

			Player player = PLAYER::GET_PLAYER_ID();
			Ped playerPed = PLAYER::GET_PLAYER_PED(player);

			switch (activeLineIndexMain)
			{
			case 0:
				//INVINCIBLE PLAYER
				PlayerInvincible = !PlayerInvincible;
				if (PlayerInvincible)
				{
					PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::GET_PLAYER_ID(), true);
					PLAYER::ADD_PAYNEKILLER(1);
				}
				else 
				{
					PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::GET_PLAYER_ID(), false);
				}
				break;
			case 1:
				// SUPER SPEED
				SuperSpeed = !SuperSpeed;
				break;
			case 2:
				// ADD WEAPONS - weapon limit is only 3 guns max i guess
				WEAPON::GIVE_WEAPON_TO_PED(playerPed, WEAPON_PISTOL_DEAGLE, 100, 1);
				WEAPON::GIVE_WEAPON_TO_PED(playerPed, WEAPON_RIFLE_AK47, 100, 1);
				break;
			case 3:
				// NO RELOAD
				NoReload = !NoReload;
				if (NoReload)
				{
					WEAPON::SET_PED_CONSUMING_AMMO(playerPed, false);
				}
				else 
				{
					WEAPON::SET_PED_CONSUMING_AMMO(playerPed, true);
				}
				break;
			case 4:
				// UNLIMITED BULLET TIME
				UnlimitedBulletTime = !UnlimitedBulletTime;
				break;
			case 5:
				// EXPLOSIVE BULLETS
				ExplosiveBullets = !ExplosiveBullets;
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

void main()
{
	while (true)
	{
		if (trainer_switch_pressed())
		{
			menu_beep();
			process_main_menu();
		}
		// SUPER SPEED
		if (IsKeyDown(VK_SHIFT) && SuperSpeed)
		{
			Ped playerPed = PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_ID());
			PLAYER::DEACTIVATE_BULLET_TIME(true, true);
			PED::APPLY_FORCE_TO_PED(playerPed, true, 0, 5, 0, 0, 0, 0, true, true, true, true);
		}
		// UNLIMITED BULLET TIME
		if (UnlimitedBulletTime)
		{
			PLAYER::SET_ADRENALINE_AMT(100);
		}
		// EXPLOSIVE BULLETS
		Vector3 coords;
		if (ExplosiveBullets)
		{
			Player player = PLAYER::GET_PLAYER_ID();
			Ped playerPed = PLAYER::GET_PLAYER_PED(player);
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &coords))
			{
				FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, EXP_TAG_GRENADE, 1, 1, 0, 1);
			}
		}
		scriptWait(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}