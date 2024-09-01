#pragma once
#ifndef __CMOB_H__
#define __CMOB_H__
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "DoubleBuffering.h"

enum MobStatType
{
	MSTATTYPE_MAXHP,
	MSTATTYPE_HP,
	MSTATTYPE_POWER,
	MSTATTYPE_SPEED,
	MSTATTYPE_RANGE,
	MSTATTYPE_X,
	MSTATTYPE_MOBTYPE,
	MSTATTYPE_MOBSTATE,
	MSTATTYPE_MAX
};

enum MobType
{
	MOBTYPE_YELLOWBEE,
	MOBTYPE_MAX
};

enum MobState
{
	MSTATE_IDLE,
	MSTATE_ATTACK,
	MSTATE_ATTACKTOWER,
	MSTATE_FAINT,
	MSTATE_MAX
};

class CMob
{
public:
	CMob();
	virtual void Init() = 0;
	int GetStat(int StatNum);
	void SetStat(int StatNum, int nValue);
	void Attacked(int nValue) { m_nHP = max(0, m_nHP - nValue); };
	void XMinus() { m_nX -= m_nSpeed; };

protected:
	int m_nMaxHP;
	int m_nHP;
	int m_nPower;
	int m_nSpeed;
	int m_nRange;
	int m_nX;
	int m_nMobType;
	int m_nMobState;
};

#endif