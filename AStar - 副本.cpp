#include "AStar.h"
#include "def.h"

using namespace astar;

///////////////////////

AStarMap::AStarMap()
{

}

AStarMap::AStarMap(VI mapdata, int width)
{
	m_vMap.assign(mapdata.begin(), mapdata.end());
	m_nWidth = width;
	m_nHeight = m_vMap.size() / width;
}

int AStarMap::getBlockId(Vec2 p)
{
	if (p.y >= 0 && p.y < m_nHeight)
	{
		if (p.x >= 0 && p.x < m_nWidth )
		{
			return m_vMap[p.y * m_nWidth + p.x];
		}
	}
	return ERR_PATH;
}

///////////////////////
MapShadow::MapShadow(int width, int height)
{
	for (int i = 0; i < width * height; i++)
	{
		m_vMapShadow.push_back(MAP_NOT_OPEN);
	}

	m_nShadowWidth = width;
	m_nShadowHeight = height;
}

MapShadow::~MapShadow()
{

}

bool MapShadow::getPosShadowState(Vec2 pos)
{
	if (pos.y >= 0 && pos.y < m_nShadowHeight)
	{
		if (pos.x >= 0 && pos.x < m_nShadowWidth )
		{
			return m_vMapShadow[pos.y * m_nShadowWidth + pos.x];
		}
	}
	return ERR_PATH;
}

void MapShadow::setPosShadowState(Vec2 pos, int shadowState)
{
	if (pos.y >= 0 && pos.y < m_nShadowHeight)
	{
		if (pos.x >= 0 && pos.x < m_nShadowWidth)
		{
			m_vMapShadow[pos.y * m_nShadowWidth + pos.x] = shadowState;
		}
	}
}

///////////////////////
AStarPoint::AStarPoint(Vec2 p, Vec2 start, Vec2 end, AStarPoint* parent)
{
	pos = Vec2(p.x, p.y);
	m_start = Vec2(start.x, start.y);
	m_end = Vec2(end.x, end.y);

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

void AStarPoint::setParent(AStarPoint* parent)
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

AStar::AStar(AStarMap map) : m_map(map)
{
	m_pMapShadow = NULL;
}

AStar::~AStar()
{
	clearOpen();
	clearClose();
}

void AStar::setMap(AStarMap map)
{
	m_map = map;
}

void AStar::setMapShadow(MapShadow* mapshadow)
{
	m_pMapShadow = mapshadow;
}

vector<Vec2> AStar::findPath(Vec2 from, Vec2 to)
{
	clearOpen();
	clearClose();
	m_path.clear();

	m_start = from;
	m_end = to;

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

AStarPoint* AStar::getPointInOpen(Vec2 p)
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
AStarPoint* AStar::getPointInClose(Vec2 p)
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

bool AStar::isPosCanCross(Vec2 p)
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
		if(m_pMapShadow->getPosShadowState( p ) == 0)
		{
			ret = false;
		}
	}

	return ret;
}

void AStar::setPathWithLastPoint(AStarPoint* p)
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

void AStar::clearOpen()
{
	for(multiset<AStarPoint*>::iterator it = m_open.begin(); it != m_open.end(); it++)
	{
		delete *it;
	}
	m_open.clear();
}

void AStar::clearClose()
{
	for(multiset<AStarPoint*>::iterator it = m_close.begin(); it != m_close.end(); it++)
	{
		delete *it;
	}
	m_close.clear();
}
