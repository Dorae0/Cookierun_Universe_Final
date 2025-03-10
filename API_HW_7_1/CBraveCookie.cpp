#include "CBraveCookie.h"

extern int PowerUpStack[4];			// 파워업 스택

CBraveCookie::CBraveCookie()
{
	Init();
}

void CBraveCookie::Init()
{
	m_nMaxHP = 100 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nHP = 100 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nPower = 30 * (1 + PowerUpStack[POWERUP_POWER] * 0.5);
	m_nSpeed = 5 * (1 + PowerUpStack[POWERUP_SPEED] * 0.5);
	m_nRange = 140 * (1 + PowerUpStack[POWERUP_RANGE] * 0.5);
	m_nCost = 10;
	m_nCookieType = COOKIETYPE_BRAVECOOKIE;
	m_nCookieState = STATE_IDLE;
}