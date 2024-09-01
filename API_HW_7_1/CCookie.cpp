#include "CCookie.h"

extern int PowerUpStack[4];			// 파워업 스택

CCookie::CCookie()
{
	m_nMaxHP = 10;
	m_nHP = 10;
	m_nPower = 1;
	m_nSpeed = 5;
	m_nRange = 140;
	m_nCost = 10;
	m_nX = 140;
	m_nCookieType = COOKIETYPE_MAX;
	m_nCookieState = STATE_IDLE;
	m_nFaintAni = 0;
}

int CCookie::GetStat(int StatNum)
{
	switch (StatNum)
	{
	case STATTYPE_MAXHP:
		return m_nMaxHP;
		break;
	case STATTYPE_HP:
		return m_nHP;
		break;
	case STATTYPE_POWER:
		return m_nPower;
		break;
	case STATTYPE_SPEED:
		return m_nSpeed;
		break;
	case STATTYPE_RANGE:
		return m_nRange;
		break;
	case STATTYPE_COST:
		return m_nCost;
		break;
	case STATTYPE_X:
		return m_nX;
		break;
	case STATTYPE_COOKIETYPE:
		return m_nCookieType;
		break;
	case STATTYPE_COOKIESTATE:
		return m_nCookieState;
		break;
	case STATTYPE_FAINTANI:
		return m_nFaintAni;
		break;
	}
}

void CCookie::SetStat(int StatNum, int nValue)
{
	switch (StatNum)
	{
	case STATTYPE_MAXHP:
		m_nMaxHP = nValue;
		break;
	case STATTYPE_HP:
		m_nHP = nValue;
		break;
	case STATTYPE_POWER:
		m_nPower = nValue;
		break;
	case STATTYPE_SPEED:
		m_nSpeed = nValue;
		break;
	case STATTYPE_RANGE:
		m_nRange = nValue;
		break;
	case STATTYPE_COST:
		m_nCost = nValue;
		break;
	case STATTYPE_X:
		m_nX = nValue;
		break;
	case STATTYPE_COOKIETYPE:
		m_nCookieType = nValue;
		break;
	case STATTYPE_COOKIESTATE:
		m_nCookieState = nValue;
		break;
	}
}