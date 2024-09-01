#pragma once
#ifndef __CYELLOWBEE_H__
#define __CYELLOWBEE_H__

#include "CMob.h"
#include <Windows.h>

class CYellowBee : public CMob
{
public:
	CYellowBee();

	void Init() override;
};

#endif