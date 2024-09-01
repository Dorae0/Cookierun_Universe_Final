#include "CYellowBee.h"

CYellowBee::CYellowBee()
{
	Init();
}

void CYellowBee::Init()
{
	m_nMaxHP = 30;
	m_nHP = 200;
	m_nPower = 100;
	m_nSpeed = 5;
	m_nRange = 160;
	m_nMobType = MOBTYPE_YELLOWBEE;
	m_nMobState = MSTATE_IDLE;
}