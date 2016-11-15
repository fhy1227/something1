#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

namespace astar{

typedef vector<int> VI;
typedef vector< VI > VVI; 
typedef VI::iterator VIit;
typedef VVI::iterator VVIit;

////////////////////////////////////////////////////////

class AStarMap
{
public:
	AStarMap();
	AStarMap(VI mapdata, int width);

	//获取地图上一点的状态
	int getBlockId(Vec2 p);

	//获取宽
	int getWidth(){return m_nWidth;}
	//获取高
	int getHeight(){return m_nHeight;}

private:
	VI m_vMap;
	int m_nWidth;
	int m_nHeight;
};

class MapShadow
{
public:
	MapShadow(int width, int height);
	virtual ~MapShadow();

	bool getPosShadowState(Vec2 p);
	void setPosShadowState(Vec2 P, int shadowState);

public:
	VI m_vMapShadow;
	int m_nShadowWidth;
	int m_nShadowHeight;
};

////////////////////////////////////////////////////////

//路径上一个点
class AStarPoint
{
public:
	AStarPoint(Vec2 p, Vec2 start, Vec2 end, AStarPoint* parent = NULL);

	void setParent(AStarPoint* parent);

	int getF(){return f;}
	int getG(){return g;}
	int getH(){return h;}

	bool operator < (const AStarPoint& p)
	{
		return f < p.f;
	}
	const bool operator < (const AStarPoint& p)const
	{
		return f < p.f;
	}

public:
	Vec2 pos;
	int f, g, h;
	AStarPoint* m_parent;

	Vec2 m_start;
	Vec2 m_end;
};

////////////////////////////////////////////////////////

struct myComp
{
    bool operator()(const AStarPoint* a,const AStarPoint* b)
	{
        return *a < *b;
    }
};

const Vec2 dirs[4] = { Vec2(0, 1), Vec2(0, -1), Vec2(1, 0), Vec2(-1, 0) };

class AStar
{
public:
	AStar();
	AStar(AStarMap map);
	~AStar();

	void setMap(AStarMap map);
	
	//查找路径
	vector<Vec2> findPath(Vec2 from, Vec2 to);

	void setMapShadow(MapShadow* mapshadow);

private:
	//开启列表中找POS点
	AStarPoint* getPointInOpen(Vec2 p);

	//关闭列表中找POS点
	AStarPoint* getPointInClose(Vec2 p);

	//POS点是否能通过
	bool isPosCanCross(Vec2 p);

	//根据查找完路径得到的最后一个Point， 设置整条路径结果m_path
	void setPathWithLastPoint(AStarPoint* p);

	//清除打开列表
	void clearOpen();
	//清楚关闭列表
	void clearClose();

private:
	AStarMap m_map;
	Vec2 m_start, m_end;
	multiset<AStarPoint*, myComp> m_open;	//开启列表
	multiset<AStarPoint*, myComp> m_close;	//关闭列表

	vector<Vec2> m_path;			// 结果

	MapShadow* m_pMapShadow;
};

} // namespace astar
