#include "CCottoncandyCookie.h"

extern int PowerUpStack[4];			// 파워업 스택

CCottoncandyCookie::CCottoncandyCookie()
{
	Init();
}

void CCottoncandyCookie::Init()
{
	m_nMaxHP = 100;
	m_nHP = 100 * (1 + PowerUpStack[POWERUP_HP] * 0.5);
	m_nPower = 75 * (1 + PowerUpStack[POWERUP_POWER] * 0.5);
	m_nSpeed = 3 * (1 + PowerUpStack[POWERUP_SPEED] * 0.5);
	m_nRange = 240 * (1 + PowerUpStack[POWERUP_RANGE] * 0.5);
	m_nCost = 20;
	m_nCookieType = COOKIETYPE_COTTONCANDYCOOKIE;
	m_nCookieState = STATE_IDLE;
}