#include "CMob.h"

CMob::CMob()
{
	m_nMaxHP = 10;
	m_nHP = 10;
	m_nPower = 10;
	m_nSpeed = 5;
	m_nRange = 10;
	m_nX = 1400;
	m_nMobType = MOBTYPE_YELLOWBEE;
	m_nMobState = MSTATE_IDLE;
}

int CMob::GetStat(int StatNum)
{
	switch (StatNum)
	{
	case MSTATTYPE_MAXHP:
		return m_nMaxHP;
		break;
	case MSTATTYPE_HP:
		return m_nHP;
		break;
	case MSTATTYPE_POWER:
		return m_nPower;
		break;
	case MSTATTYPE_SPEED:
		return m_nSpeed;
		break;
	case MSTATTYPE_RANGE:
		return m_nRange;
		break;
	case MSTATTYPE_X:
		return m_nX;
		break;
	case MSTATTYPE_MOBTYPE:
		return m_nMobType;
		break;
	case MSTATTYPE_MOBSTATE:
		return m_nMobState;
		break;
	}
}

void CMob::SetStat(int StatNum, int nValue)
{
	switch (StatNum)
	{
	case MSTATTYPE_MAXHP:
		m_nMaxHP = nValue;
		break;
	case MSTATTYPE_HP:
		m_nHP = nValue;
		break;
	case MSTATTYPE_POWER:
		m_nPower = nValue;
		break;
	case MSTATTYPE_SPEED:
		m_nSpeed = nValue;
		break;
	case MSTATTYPE_RANGE:
		m_nRange = nValue;
		break;
	case MSTATTYPE_X:
		m_nX = nValue;
		break;
	case MSTATTYPE_MOBTYPE:
		m_nMobType = nValue;
		break;
	case MSTATTYPE_MOBSTATE:
		m_nMobState = nValue;
		break;
	}
}