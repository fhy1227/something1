#include "GameMap.h"

CGameMap::CGameMap(vector<MAP_NODE> mapData) : astar::MapShadow(MAP_WIDTH, MAP_HEIGHT)
{
	m_pAStar = new astar::AStar<MAP_NODE>();
	m_pAStar->setMapShadow(this);
	m_bHadShadow = true;

	m_map = astar::AStarMap<MAP_NODE>(mapData, MAP_WIDTH);
	m_pAStar->setMap(m_map);
}

CGameMap::~CGameMap(void)
{
	if (m_pAStar)
	{
		delete m_pAStar;
		m_pAStar = NULL;
	}
}

void CGameMap::setBlockTrigger(Vec2 p, bool b)
{
	m_map.setBlockTrigger(p, b);
	m_pAStar->setBlockTrigger(p, b);
}

vector<Vec2> CGameMap::findPath(Vec2 from, Vec2 to)
{
	return m_pAStar->findPath(from, to);
}

int CGameMap::getWidth()
{
	return m_map.getWidth();
}

int CGameMap::getHeight()
{
	return m_map.getHeight();
}
