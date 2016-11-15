#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
#include "cocos2d.h"
#include "def.h"

using namespace cocos2d;
using namespace std;

namespace astar{

	typedef vector<int> VI;
	typedef vector< VI > VVI; 

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//地图类
template<class T>
class AStarMap
{
public:
	AStarMap(){}
	AStarMap(vector<T> mapdata, int width)
	{
		m_vMap.assign(mapdata.begin(), mapdata.end());
		m_nWidth = width;
		m_nHeight = m_vMap.size() / width;
	}

	//获取地图上一点
	T getBlockNode(Vec2 p)
	{
		if (p.y >= 0 && p.y < m_nHeight && p.x >= 0 && p.x < m_nWidth)
		{
			return m_vMap[p.y * m_nWidth + p.x];
		}

		return T();
	}

	void setBlockTrigger(Vec2 p, bool b)
	{
		if (p.y >= 0 && p.y < m_nHeight && p.x >= 0 && p.x < m_nWidth)
		{
			m_vMap[p.y * m_nWidth + p.x].setTriggered(b);
		}
	}

	//地图上某点是否能通过
	bool isPosCanCross(Vec2 p)
	{
		T node = getBlockNode(p);
		return node.canCross();
	}

	//获取宽
	int getWidth(){return m_nWidth;}
	//获取高
	int getHeight(){return m_nHeight;}

public:
	vector<T> m_vMap;
	int m_nWidth;
	int m_nHeight;
};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//地图阴影
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
////////////////////////////////////////////////////////
//路径上一个点类
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
////////////////////////////////////////////////////////
//寻路算法类
struct myComp
{
    bool operator()(const AStarPoint* a,const AStarPoint* b)
	{
        return *a < *b;
    }
};

const Vec2 dirs[4] = { Vec2(0, 1), Vec2(0, -1), Vec2(1, 0), Vec2(-1, 0) };

template<class T>
class AStar
{
public:
	AStar(){m_pMapShadow = NULL;}
	AStar(AStarMap<T> map){m_pMapShadow = NULL;}
	~AStar()
	{
		clearOpen();
		clearClose();
	}

	void setMap(AStarMap<T> map){m_map = map;}
	void setMapShadow(MapShadow* mapshadow){m_pMapShadow = mapshadow;}

	void setBlockTrigger(Vec2 p, bool b)
	{
		m_map.setBlockTrigger(p, b);
	}

	//查找路径
	vector<Vec2> findPath(Vec2 from, Vec2 to)
	{
		clearOpen();
		clearClose();
		m_path.clear();

		m_start = from;
		m_end = to;

		if (m_start == m_end)
		{
			return m_path;
		}

		AStarPoint* p = new AStarPoint(m_start, m_start, m_end);
		m_open.insert(p);

		while( m_open.size() > 0 )
		{
			//multiset<Point*, myComp> m_open;开启列表  是根据 Point 的 F值进行排序的， 所以第一个就是列表中 F 最小的
			AStarPoint* p = *m_open.begin();
			m_open.erase(m_open.begin());
			m_close.insert(p);

			for(int i = 0; i < 4; i++)
			{
				Vec2 thisPos = p->pos + dirs[i];
				if (thisPos == m_end)
				{
					//找到终点
					AStarPoint* thisPoint = new AStarPoint(thisPos, m_start, m_end, p);
					m_close.insert(thisPoint);
					setPathWithLastPoint(thisPoint);
					return m_path;
				}
				else if( isPosCanCross(thisPos) && !getPointInClose(thisPos) )
				{
					AStarPoint* thisPoint = getPointInOpen(thisPos);
					if(thisPoint == NULL)
					{
						AStarPoint* newPoint = new AStarPoint(thisPos, m_start, m_end, p);
						m_open.insert(newPoint);
					}
					else
					{
						//如果在开启列表中， 查看该点之前的G值， 如果比 (当前点G值 + 当前点到该点的权值) 高，  则吧当前点设置为该点的父节点
						int lastG = thisPoint->getG();
						int thisG = p->getG() + 1;
						if (thisG < lastG)
						{
							thisPoint->setParent(p);
						}
					}
				}
			}
		}

		return m_path;
	}

private:
	//开启列表中找POS点
	AStarPoint* getPointInOpen(Vec2 p)
	{
		for(multiset<AStarPoint*>::iterator it = m_open.begin(); it != m_open.end(); it++)
		{
			if ((*it)->pos == p)
			{
				return *it;
			}
		}
		return NULL;
	}

	//关闭列表中找POS点
	AStarPoint* getPointInClose(Vec2 p)
	{
		for(multiset<AStarPoint*>::iterator it = m_close.begin(); it != m_close.end(); it++)
		{
			if ((*it)->pos == p)
			{
				return *it;
			}
		}
		return NULL;
	}

	//地图上某点是否能通过
	bool isPosCanCross(Vec2 p)
	{
		bool ret = false;
		if(m_map.isPosCanCross(p))
		{
			ret = true;
		}

		if (m_pMapShadow)
		{
			if(m_pMapShadow->getPosShadowState( p ) == 0)
			{
				ret = false;
			}
		}

		return ret;
	}

	//根据查找完路径得到的最后一个Point， 设置整条路径结果m_path
	void setPathWithLastPoint(AStarPoint* p)
	{
		m_path.clear();
		m_path.push_back(Vec2(p->pos.x, p->pos.y));
		AStarPoint* point = p;
		while( point->m_parent )
		{
			m_path.push_back(Vec2(point->m_parent->pos.x, point->m_parent->pos.y));
			point = point->m_parent;
		}
		reverse(m_path.begin(), m_path.end());
	}

	//清除打开列表
	void clearOpen()
	{
		for(multiset<AStarPoint*>::iterator it = m_open.begin(); it != m_open.end(); it++)
		{
			delete *it;
		}
		m_open.clear();
	}
	//清楚关闭列表
	void clearClose()
	{
		for(multiset<AStarPoint*>::iterator it = m_close.begin(); it != m_close.end(); it++)
		{
			delete *it;
		}
		m_close.clear();
	}

private:
	AStarMap<T> m_map;
	Vec2 m_start, m_end;
	multiset<AStarPoint*, myComp> m_open;	//开启列表
	multiset<AStarPoint*, myComp> m_close;	//关闭列表

	vector<Vec2> m_path;			// 结果

	MapShadow* m_pMapShadow;
};

} // namespace astar
