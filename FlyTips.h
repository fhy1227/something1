#pragma once

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class CFlyTips : public Layer
{
public:
	CFlyTips();
	virtual ~CFlyTips(void);

	static CFlyTips* create(std::string tip);

public:
	bool init(std::string tip);

private:
	Label* m_pLabelTips;

};