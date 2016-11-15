#include "AStar.h"
#include "def.h"

using namespace astar;

///////////////////////

Map::Map()
{

}

Map::Map(VVI mapdata)
{
	m_vMap.assign(mapdata.begin(), mapdata.end());
}

VI Map::operator [](int n)
{
	return m_vMap[n];
}

int Map::getBlockId(Pos p)
{
	if (p.y >= 0 && p.y < m_vMap.size())
	{
		if (p.x >= 0 && p.x < m_vMap[p.y].size() )
		{
			return m_vMap[p.y][p.x];
		}
	}
	return ERR_PATH;
}

int Map::getWidth()
{
	if( m_vMap.size() > 0 )
	{
		return m_vMap[1].size();
	}
	return 0;
}

int Map::getHeight()
{
	return m_vMap.size();
}

///////////////////////
MapShadow::MapShadow()
{
	for ( int h = 0; h < MAP_HEIGHT; h++ )
	{
		vector<int> v;
		for ( int w = 0; w < MAP_WIDTH; w++ )
		{
			v.push_back(0);
		}
		m_vMapShadow.push_back(v);
	}
}

MapShadow::~MapShadow()
{

}

VI MapShadow::operator [](int n)
{
	return m_vMapShadow[n];
}

bool MapShadow::getPosShadowState(int x, int y)
{
	if (y >= 0 && y < m_vMapShadow.size())
	{
		if (x >= 0 && x < m_vMapShadow[y].size() )
		{
			return m_vMapShadow[y][x];
		}
	}
	return ERR_PATH;
}

void MapShadow::setPosShadowState(int x, int y, int shadowState)
{
	if (y >= 0 && y < m_vMapShadow.size())
	{
		if (x >= 0 && x < m_vMapShadow[y].size() )
		{
			m_vMapShadow[y][x] = shadowState;
		}
	}
}

///////////////////////
Point::Point(Pos p, Pos start, Pos end, Point* parent)
{
	pos = Pos(p.x, p.y);
	m_start = Pos(start.x, start.y);
	m_end = Pos(end.x, end.y);

	m_parent = parent;
	if(parent == NULL)
	{
		//起点
		g = 0;
		h = abs(end.x - start.x) + abs(end.y - start.y);
		f = g + h;
	}
	else
	{
		g = parent->getG() + 1;
		h = abs(end.x - pos.x) + abs(end.y - pos.y);
		f = g + h;
	}
}

void Point::setParent(Point* parent)
{
	m_parent = parent;
	g = parent->getG() + 1;	//本例只有4个方向，如果有8个方向，则判断是否是斜的，  加的权值不同
	f = g + h;
}

///////////////////////

AStar::AStar()
{
	m_pMapShadow = NULL;
}

AStar::AStar(Map map) : m_map(map)
{
	m_pMapShadow = NULL;
}

AStar::~AStar()
{
	clearOpen();
	clearClose();
}

void AStar::setMap(Map map)
{
	m_map = map;
}

void AStar::setMapShadow(MapShadow* mapshadow)
{
	m_pMapShadow = mapshadow;
}

vector<Pos> AStar::findPath(int x1, int y1, int x2, int y2)
{
	clearOpen();
	clearClose();
	m_path.clear();

	m_start = Pos(x1, y1);
	m_end = Pos(x2, y2);

	Point* p = new Point(m_start, m_start, m_end);
	m_open.insert(p);

	while( m_open.size() > 0 )
	{
		//multiset<Point*, myComp> m_open;开启列表  是根据 Point 的 F值进行排序的， 所以第一个就是列表中 F 最小的
		Point* p = *m_open.begin();
		m_open.erase(m_open.begin());
		m_close.insert(p);

		for(int i = 0; i < 4; i++)
		{
			Pos thisPos = p->pos + dirs[i];
			if (thisPos == m_end)
			{
				//找到终点
				Point* thisPoint = new Point(thisPos, m_start, m_end, p);
				m_close.insert(thisPoint);
				setPathWithLastPoint(thisPoint);
				return m_path;
			}
			else if( isPosCanCross(thisPos) && !getPointInClose(thisPos) )
			{
				Point* thisPoint = getPointInOpen(thisPos);
				if(thisPoint == NULL)
				{
					Point* newPoint = new Point(thisPos, m_start, m_end, p);
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

Point* AStar::getPointInOpen(Pos p)
{
	for(multiset<Point*>::iterator it = m_open.begin(); it != m_open.end(); it++)
	{
		if ((*it)->pos == p)
		{
			return *it;
		}
	}
	return NULL;
}
Point* AStar::getPointInClose(Pos p)
{
	for(multiset<Point*>::iterator it = m_close.begin(); it != m_close.end(); it++)
	{
		if ((*it)->pos == p)
		{
			return *it;
		}
	}
	return NULL;
}

bool AStar::isPosCanCross(Pos p)
{
	bool ret = false;
	for(int i = 0; i < 3; i++)
	{
		if (CAN_CROSS_BLOCKID[i] == m_map.getBlockId(p))
		{
			ret = true;
			break;
		}
	}

	if (m_pMapShadow)
	{
		if(m_pMapShadow->getPosShadowState(p.x, p.y) == 0)
		{
			ret = false;
		}
	}

	return ret;
}

void AStar::setPathWithLastPoint(Point* p)
{
	m_path.clear();
	m_path.push_back(Pos(p->pos.x, p->pos.y));
	Point* point = p;
	while( point->m_parent )
	{
		m_path.push_back(Pos(point->m_parent->pos.x, point->m_parent->pos.y));
		point = point->m_parent;
	}
	reverse(m_path.begin(), m_path.end());
}

void AStar::clearOpen()
{
	for(multiset<Point*>::iterator it = m_open.begin(); it != m_open.end(); it++)
	{
		delete *it;
	}
	m_open.clear();
}

void AStar::clearClose()
{
	for(multiset<Point*>::iterator it = m_close.begin(); it != m_close.end(); it++)
	{
		delete *it;
	}
	m_close.clear();
}
