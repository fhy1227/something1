#pragma once

#include <iostream>
#include <map>
#include "cocos2d.h"
#include "def.h"

using namespace std;
using namespace cocos2d;

class CPackageBag : public CCObject
{
public:
	CPackageBag();
	virtual ~CPackageBag(void);

	static CPackageBag* getInstance();

public:
	void addItem(ITEM_STRUCT item);
	void removeItem(ITEM_STRUCT item);

private:
	map<int, ITEM_STRUCT> m_mapBagItem;

	static CPackageBag* m_instance;
};