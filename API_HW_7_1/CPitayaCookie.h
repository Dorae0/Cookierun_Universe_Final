#pragma once
#ifndef __CPITAYACOOKIE_H__
#define __CPITAYACOOKIE_H__

#include "CCookie.h"
#include <Windows.h>

class CPitayaCookie : public CCookie
{
public:
	CPitayaCookie();

	void Init() override;
};

#endif