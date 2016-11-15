#pragma once

#include "AStar.h"
#include "cocos2d.h"
#include "def.h"
using namespace cocos2d;

class CGameMap : public CCObject, public astar::MapShadow
{
public:
	CGameMap(vector<MAP_NODE>);
	virtual ~CGameMap(void);

	astar::AStarMap<MAP_NODE> getMap(){return m_map;}
	void setBlockTrigger(Vec2 p, bool b);

	vector<Vec2> findPath(Vec2 from, Vec2 to);

public:
	int getWidth();
	int getHeight();
		
private:
	astar::AStarMap<MAP_NODE>	m_map;

	astar::AStar<MAP_NODE>* m_pAStar;

public:
	bool m_bHadShadow;
};

