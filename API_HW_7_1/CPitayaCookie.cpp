#include "CPitayaCookie.h"

extern int PowerUpStack[4];			// 파워업 스택

CPitayaCookie::CPitayaCookie()
{
	Init();
}

void CPitayaCookie::Init()
{
	m_nMaxHP = 150 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nHP = 150 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nPower = 150 * (1 + PowerUpStack[POWERUP_POWER] * 0.5);
	m_nSpeed = 7 * (1 + PowerUpStack[POWERUP_SPEED] * 0.5);
	m_nRange = 140 * (1 + PowerUpStack[POWERUP_RANGE] * 0.5);
	m_nCost = 50;
	m_nCookieType = COOKIETYPE_PITAYACOOKIE;
	m_nCookieState = STATE_IDLE;
}