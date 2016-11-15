#pragma once

#include "cocos2d.h"
#include "GameMap.h"
#include "def.h"
using namespace cocos2d;

class CGameMapView : public Layer
{
public:
	CGameMapView();
	virtual ~CGameMapView(void);

public:
	static CGameMapView* create(vector<MAP_NODE>);

private:
	virtual bool init();
	bool initWithMap( vector<MAP_NODE> );

public:
	Vec2 getPosByCoordinate(int x, int y);	//根据position获取地图上坐标
	Vec2 getCoordinateByPos(Vec2 pos);		//根据地图上坐标获取position

	void updateShadowByMidPos(Vec2 v);
	bool isPosInShadow(Vec2 v);

	vector<Vec2> findPath(Vec2 from, Vec2 to);

	MAP_NODE getBlockNode(Vec2 coor);

	void setBlockTrigger(Vec2 p, bool b);

	void setBlockTexture(Sprite* &block, int blockId);

	void triggerEventBlock(Vec2 pos);

private:
	CGameMap* m_mapData;

	SpriteBatchNode* m_batchNode;

private:
	int m_nMapWCellNum;
	int m_nMapHCellNum;
};

