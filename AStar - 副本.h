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

	//��ȡ��ͼ��һ���״̬
	int getBlockId(Vec2 p);

	//��ȡ��
	int getWidth(){return m_nWidth;}
	//��ȡ��
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

//·����һ����
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
	
	//����·��
	vector<Vec2> findPath(Vec2 from, Vec2 to);

	void setMapShadow(MapShadow* mapshadow);

private:
	//�����б�����POS��
	AStarPoint* getPointInOpen(Vec2 p);

	//�ر��б�����POS��
	AStarPoint* getPointInClose(Vec2 p);

	//POS���Ƿ���ͨ��
	bool isPosCanCross(Vec2 p);

	//���ݲ�����·���õ������һ��Point�� ��������·�����m_path
	void setPathWithLastPoint(AStarPoint* p);

	//������б�
	void clearOpen();
	//����ر��б�
	void clearClose();

private:
	AStarMap m_map;
	Vec2 m_start, m_end;
	multiset<AStarPoint*, myComp> m_open;	//�����б�
	multiset<AStarPoint*, myComp> m_close;	//�ر��б�

	vector<Vec2> m_path;			// ���

	MapShadow* m_pMapShadow;
};

} // namespace astar
