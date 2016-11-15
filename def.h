#pragma once

#include "FlyTips.h"

//////////////////////////

#define ERR_PATH -1

static const float cellWidth = 33;
static const float cellHeight = 33;

static const int MAP_TEXTURE_WIDTH = 8;
static const int MAP_TEXTURE_HEIGHT = 6;

#define INVALID_BLOCK -1

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

#define MAP_BLOCKID_MIN 0
#define MAP_BLOCKID_MAX 47

#define INVALID_POS (Vec2(-1, -1))

#define MAP_NOT_OPEN	0		//地图未打开
#define MAP_OPEN		1		//地图打开
#define MAP_FOG			2		//地图半透明

////////////////////////////////////////////

enum MAP_NODE_TYPE
{
	MAP_NODE_ROAD = 1,	//可通行的路
	MAP_NODE_CLOSE = 2,		//不能通行
	MAP_NODE_DOOR = 3,		//门,开关状态
	MAP_NODE_ITEM = 4,		//可拾取的装备
	MAP_NODE_EVENT = 5,		//事件
	MAP_NODE_ENEMY = 6,		//敌人
};

struct MAP_NODE 
{
	MAP_NODE() : node_type(MAP_NODE_CLOSE), block_id(0), new_block_id(33), event_id(0), triggered(false){}

	bool canCross()
	{
		bool ret = false;
		if ( node_type == MAP_NODE_ROAD || ( node_type != MAP_NODE_CLOSE && triggered == true ) )
		{
			ret = true;
		}
		return ret;
	}

	void setTriggered(bool b)
	{
		triggered = true;
	}

	MAP_NODE_TYPE node_type;		//类型
	int block_id;					//块ID
	int new_block_id;				//非通行的路类型， 触发之后块ID
	int event_id;					//如果是事件类型，保存事件ID。  如果是装备类型，保存装备ID。   如果是敌人类型，保存敌人ID
	bool triggered;					//是否已经触发
};

////////////////////////////////////////////

#define INVALID_UUID -1

enum ITEM_ID
{
	ITEM_NONE = 1,
	ITEM_HELMET,
	ITEM_CLOTHES,
	ITEM_WEAPON,
	ITEM_SHOE,
	ITEM_PANTS, 
};

enum ITEM_EFFECT
{
	EFFECT_NONE = 1,
};

struct ITEM_STRUCT  
{
	ITEM_STRUCT() :
	id(ITEM_NONE),
	uid(INVALID_UUID),
	attack(0),
	defense(0),
	hp(0),
	mp(0),
	endurance(0),
	ef(EFFECT_NONE){}

	ITEM_ID id;
	int uid;
	int	attack;
	int defense;
	int hp;
	int mp;
	int endurance;
	ITEM_EFFECT ef;
};
