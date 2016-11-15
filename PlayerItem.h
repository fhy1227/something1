#pragma once

#include <iostream>
#include <map>
#include "cocos2d.h"
#include "def.h"

using namespace std;
using namespace cocos2d;

class CPlayerItem : public CCObject
{
public:
	CPlayerItem();
	virtual ~CPlayerItem(void);

	static CPlayerItem* getInstance();

public:
	void equipItem(ITEM_STRUCT item);
	void equipItemFromBag(ITEM_STRUCT item);
	void unloadItem(ITEM_ID item_id);

	ITEM_STRUCT getItemById(ITEM_ID item_id);

private:
	map<ITEM_ID, ITEM_STRUCT> m_mapItem;

	static CPlayerItem* m_instance;
};