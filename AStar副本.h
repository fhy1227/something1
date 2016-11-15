#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>

using namespace std;

namespace astar{

typedef vector<int> VI;
typedef vector< VI > VVI; 
typedef VI::iterator VIit;
typedef VVI::iterator VVIit;

////////////////////////////////////////////////////////
struct Pos
{
	Pos(int a = 0, int b = 0) : x(a), y(b) {}
	bool operator ==(Pos p)
	{
		return p.x == x && p.y == y;
	}
	Pos operator +(Pos p)
	{
		return Pos(p.x + x, p.y + y);
	}
	int x;
	int y;
};

////////////////////////////////////////////////////////

class Map
{
public:
	Map();
	Map(VVI mapdata);

	//����[]
	VI operator [](int n);

	int size(){return m_vMap.size();}

	//��ȡ��ͼ��һ���״̬
	int getBlockId(Pos p);

	//��ȡ��
	int getWidth();

	//��ȡ��
	int getHeight();

private:
	VVI m_vMap;
};

class MapShadow
{
public:
	MapShadow();
	virtual ~MapShadow();

	//����[]
	VI operator [](int n);

	int size(){return m_vMapShadow.size();}

	bool getPosShadowState(int x, int y);
	void setPosShadowState(int x, int y, int shadowState);

public:
	VVI m_vMapShadow;
};

////////////////////////////////////////////////////////

//·����һ����
class Point
{
public:
	Point(Pos p, Pos start, Pos end, Point* parent = NULL);

	void setParent(Point* parent);

	int getF(){return f;}
	int getG(){return g;}
	int getH(){return h;}

	bool operator < (const Point& p)
	{
		return f < p.f;
	}
	const bool operator < (const Point& p)const
	{
		return f < p.f;
	}

public:
	Pos pos;
	int f, g, h;
	Point* m_parent;

	Pos m_start;
	Pos m_end;
};

////////////////////////////////////////////////////////

struct myComp
{
    bool operator()(const Point* a,const Point* b)
	{
        return *a < *b;
    }
};

const Pos dirs[4] = { Pos(0, 1), Pos(0, -1), Pos(1, 0), Pos(-1, 0) };

class AStar
{
public:
	AStar();
	AStar(Map map);
	~AStar();

	void setMap(Map map);
	
	//����·��
	vector<Pos> findPath(int x1, int y1, int x2, int y2);

	void setMapShadow(MapShadow* mapshadow);

private:
	//�����б�����POS��
	Point* getPointInOpen(Pos p);

	//�ر��б�����POS��
	Point* getPointInClose(Pos p);

	//POS���Ƿ���ͨ��
	bool isPosCanCross(Pos p);

	//���ݲ�����·���õ������һ��Point�� ��������·�����m_path
	void setPathWithLastPoint(Point* p);

	//������б�
	void clearOpen();
	//����ر��б�
	void clearClose();

private:
	Map m_map;
	Pos m_start, m_end;
	multiset<Point*, myComp> m_open;	//�����б�
	multiset<Point*, myComp> m_close;	//�ر��б�

	vector<Pos> m_path;			// ���

	MapShadow* m_pMapShadow;
};

} // namespace astar
