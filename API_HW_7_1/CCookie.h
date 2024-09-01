#pragma once
#ifndef __CCOOKIE_H__
#define __CCOOKIE_H__
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "DoubleBuffering.h"

enum CookieType
{
	COOKIETYPE_BRAVECOOKIE,
	COOKIETYPE_COTTONCANDYCOOKIE,
	COOKIETYPE_PISTACHIOCOOKIE,
	COOKIETYPE_PITAYACOOKIE,
	COOKIETYPE_MAX
};

enum StatType
{
	STATTYPE_MAXHP,
	STATTYPE_HP,
	STATTYPE_POWER,
	STATTYPE_SPEED,
	STATTYPE_RANGE,
	STATTYPE_COST,
	STATTYPE_COOLDOWN,
	STATTYPE_COOLDOWNNOW,
	STATTYPE_X,
	STATTYPE_COOKIETYPE,
	STATTYPE_COOKIESTATE,
	STATTYPE_FAINTANI,
	STATTYPE_MAX
};

enum CookieState
{
	STATE_IDLE,
	STATE_ATTACK,
	STATE_ATTACKBOSS,
	STATE_FAINT,
	STATE_MAX
};

enum PowerUps
{
	POWERUP_SPEED,
	POWERUP_POWER,
	POWERUP_RANGE,
	POWERUP_HP,
	POWERUP_MAX
};

class CCookie
{
public:
	CCookie();
	virtual void Init() = 0;
	int GetStat(int StatNum);
	void SetStat(int StatNum, int nValue);
	void Attacked(int nValue) { m_nHP = max(0, m_nHP - nValue); };
	void XPlus() { m_nX += m_nSpeed; };

protected:
	int m_nMaxHP;
	int m_nHP;
	int m_nPower;
	int m_nSpeed;
	int m_nRange;
	int m_nCost;
	int m_nX;
	int m_nCookieType;
	int m_nCookieState;
	int m_nFaintAni;
};
#endif