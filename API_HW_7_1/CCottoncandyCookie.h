#pragma once
#ifndef __CCOTTONCANDYCOOKIE_H__
#define __CCOTTONCANDYCOOKIE_H__

#include "CCookie.h"
#include <Windows.h>

class CCottoncandyCookie : public CCookie
{
public:
	CCottoncandyCookie();

	void Init() override;
};

#endif