#pragma once
#ifndef __CBRAVECOOKIE_H__
#define __CBRAVECOOKIE_H__

#include "CCookie.h"
#include <Windows.h>

class CBraveCookie : public CCookie
{
public:
	CBraveCookie();

	void Init() override;
};

#endif