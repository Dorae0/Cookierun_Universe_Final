#include "CookieMove.h"
#include "CCookie.h"

extern int x, y, Vy, Ay, Cstate, GroundY, Cost, GameState, EnterState, EnterCooldown, NumState;
extern int PowerUpStack[4];
extern COOLDOWN		CooldownState[10];
extern enum GameState
{
	GSTATE_MAIN,
	GSTATE_GAME,
	GSTATE_GAMERULE,
	GSTATE_GAMECOUNT,
	GSTATE_WIN,
	GSTATE_LOSE,
	GSTATE_OPTIONSELECT,
	GSTATE_MAX
};


void __KeyProc(HWND hWnd)
{
	//// 0x80 비트가 1이면 가상키가 눌린 것
	//   만약 0x80 비트가 1이었다가 0이 되면 가상키를 뗀 것
	//   (0x80 비트가 0인 상태가 가상키를 뗀 것이 아님에 유의)
	// 0x80 == 0b1000 0000

	if (GetKeyState(VK_RETURN) & 0x80 && GameState == GSTATE_MAIN)
	{
		if (EnterState == 0)
		{
			GameState = GSTATE_GAMERULE;
			EnterCooldown = GetTickCount64();
			EnterState = 1;
		}
	}

	if (GetKeyState(VK_RETURN) & 0x80 && GameState == GSTATE_GAMERULE)
	{
		if (EnterState == 0)
		{
			GameState = GSTATE_GAMECOUNT;
			SetWindowPos(hWnd, NULL, 50, 50, 1800, 609, 0);
			EnterState = 1;
		}
	}

	if (GetKeyState(VK_RETURN) & 0x80 && GameState == GSTATE_WIN)
	{
		if (EnterState == 0)
		{
			GameState = GSTATE_OPTIONSELECT;
			EnterState = 1;
		}
	}

	if (GetKeyState(VK_RETURN) & 0x80 && GameState == GSTATE_LOSE)
	{
		if (EnterState == 0)
		{
			GameState = GSTATE_MAIN;
			SetWindowPos(hWnd, NULL, 50, 50, 915, 645, 0);
			EnterState = 1;
		}
	}

	if (GameState == GSTATE_GAME && NumState == 0)
	{
		if (GetKeyState(0x31) & 0x80 || GetKeyState(VK_NUMPAD1) & 0x80)
		{
			if (CooldownState[COOKIETYPE_BRAVECOOKIE].CookieButtonState == false && CooldownState[COOKIETYPE_BRAVECOOKIE].CookieCooldownState == false && Cost >= 10)
			{
				CooldownState[COOKIETYPE_BRAVECOOKIE].CookieButtonState = true;
				CooldownState[COOKIETYPE_BRAVECOOKIE].CookieCooldownState = true;
			}
			NumState = 1;
		}

		if (GetKeyState(0x32) & 0x80 || GetKeyState(VK_NUMPAD2) & 0x80)
		{
			if (CooldownState[COOKIETYPE_COTTONCANDYCOOKIE].CookieButtonState == false && CooldownState[COOKIETYPE_COTTONCANDYCOOKIE].CookieCooldownState == false && Cost >= 20)
			{
				CooldownState[COOKIETYPE_COTTONCANDYCOOKIE].CookieButtonState = true;
				CooldownState[COOKIETYPE_COTTONCANDYCOOKIE].CookieCooldownState = true;
			}
			NumState = 1;
		}

		if (GetKeyState(0x33) & 0x80 || GetKeyState(VK_NUMPAD3) & 0x80)
		{
			if (CooldownState[COOKIETYPE_PISTACHIOCOOKIE].CookieButtonState == false && CooldownState[COOKIETYPE_PISTACHIOCOOKIE].CookieCooldownState == false && Cost >= 25)
			{
				CooldownState[COOKIETYPE_PISTACHIOCOOKIE].CookieButtonState = true;
				CooldownState[COOKIETYPE_PISTACHIOCOOKIE].CookieCooldownState = true;
			}
			NumState = 1;
		}

		if (GetKeyState(0x34) & 0x80 || GetKeyState(VK_NUMPAD4) & 0x80)
		{
			if (CooldownState[COOKIETYPE_PITAYACOOKIE].CookieButtonState == false && CooldownState[COOKIETYPE_PITAYACOOKIE].CookieCooldownState == false && Cost >= 50)
			{
				CooldownState[COOKIETYPE_PITAYACOOKIE].CookieButtonState = true;
				CooldownState[COOKIETYPE_PITAYACOOKIE].CookieCooldownState = true;
			}
			NumState = 1;
		}
	}

	if (GameState == GSTATE_OPTIONSELECT && NumState == 0)
	{
		if (GetKeyState(0x31) & 0x80 || GetKeyState(VK_NUMPAD1) & 0x80)
		{
			if (PowerUpStack[0] <= 5)
			{
				PowerUpStack[0]++;
				GameState = GSTATE_GAMECOUNT;
			}
			NumState = 1;
		}

		if (GetKeyState(0x32) & 0x80 || GetKeyState(VK_NUMPAD2) & 0x80)
		{
			if (PowerUpStack[1] <= 5)
			{
				PowerUpStack[1]++;
				GameState = GSTATE_GAMECOUNT;
			}
			NumState = 1;
		}

		if (GetKeyState(0x33) & 0x80 || GetKeyState(VK_NUMPAD3) & 0x80)
		{
			if (PowerUpStack[2] <= 5)
			{
				PowerUpStack[2]++;
				GameState = GSTATE_GAMECOUNT;
			}
			NumState = 1;
		}

		if (GetKeyState(0x34) & 0x80 || GetKeyState(VK_NUMPAD4) & 0x80)
		{
			if (PowerUpStack[3] <= 5)
			{
				PowerUpStack[3]++;
				GameState = GSTATE_GAMECOUNT;
			}
			NumState = 1;
		}
	}
}