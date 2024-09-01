#pragma once
#ifndef __CPISTACHIOCOOKIE_H__
#define __CPISTACHIOCOOKIE_H__

#include "CCookie.h"
#include <Windows.h>

class CPistachioCookie : public CCookie
{
public:
	CPistachioCookie();

	void Init() override;
};

#endif