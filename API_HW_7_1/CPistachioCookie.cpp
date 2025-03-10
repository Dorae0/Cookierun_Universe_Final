#include "CPistachioCookie.h"

extern int PowerUpStack[4];			// 파워업 스택

CPistachioCookie::CPistachioCookie()
{
	Init();
}

void CPistachioCookie::Init()
{
	m_nMaxHP = 300 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nHP = 300 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nPower = 45 * (1 + PowerUpStack[POWERUP_POWER] * 0.5);
	m_nSpeed = 6 * (1 + PowerUpStack[POWERUP_SPEED] * 0.5);
	m_nRange = 170 * (1 + PowerUpStack[POWERUP_RANGE] * 0.5);
	m_nCost = 25;
	m_nCookieType = COOKIETYPE_PISTACHIOCOOKIE;
	m_nCookieState = STATE_IDLE;
}