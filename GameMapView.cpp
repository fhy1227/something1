#include "GameMapView.h"

CGameMapView::CGameMapView()
{
	m_nMapWCellNum = 0.0f;
	m_nMapHCellNum = 0.0f;
}

CGameMapView::~CGameMapView(void)
{
	if (m_mapData)
	{
		delete m_mapData;
		m_mapData = NULL;
	}
}

CGameMapView* CGameMapView::create(vector<MAP_NODE> map)
{
	CGameMapView* view = new CGameMapView();
	if ( view && view->initWithMap( map ) && view->init() )
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return NULL;
}

bool CGameMapView::init()
{
	if (!Layer::init())
		return false;

	m_batchNode = SpriteBatchNode::create("tmw_desert_spacing.png");
	addChild(m_batchNode);
	m_batchNode->setPosition(Point::ZERO);

	m_nMapWCellNum = m_mapData->getHeight();
	m_nMapHCellNum = m_mapData->getWidth();

	for (int i = 0; i < m_nMapWCellNum * m_nMapHCellNum; i++)
	{
		int x = i % m_nMapWCellNum;
		int y = i / m_nMapWCellNum;

		int blockId = getBlockNode( Vec2(x, y) ).block_id;
		int rectX = blockId % MAP_TEXTURE_WIDTH;
		int rectY = MAP_TEXTURE_HEIGHT - blockId / MAP_TEXTURE_WIDTH - 1;
		Sprite* sp = Sprite::createWithTexture( m_batchNode->getTexture() );
		sp->setTextureRect( Rect(rectX * cellWidth, rectY * cellHeight, cellWidth, cellHeight) );

		sp->setAnchorPoint( Vec2( 0, 0 ) );
		sp->setPosition(Vec2( x * cellWidth, y * cellHeight ));

		if(m_mapData->m_bHadShadow && m_mapData->m_vMapShadow[i] == MAP_NOT_OPEN)
		{
			sp->setVisible(false);
		}

		sp->setTag(i);
		m_batchNode->addChild(sp);
	}

	return true;
}

bool CGameMapView::initWithMap(vector<MAP_NODE> map)
{
	m_mapData = new CGameMap(map);

	return true;
}

MAP_NODE CGameMapView::getBlockNode(Vec2 coor)
{
	return m_mapData->getMap().getBlockNode( coor );
}

void CGameMapView::setBlockTrigger(Vec2 p, bool b)
{
	m_mapData->setBlockTrigger(p, b);
}

void CGameMapView::setBlockTexture(Sprite* &block, int blockId)
{
	int rectX = blockId % MAP_TEXTURE_WIDTH;
	int rectY = MAP_TEXTURE_HEIGHT - blockId / MAP_TEXTURE_WIDTH - 1;
	block->setTextureRect( Rect(rectX * cellWidth, rectY * cellHeight, cellWidth, cellHeight) );
}

void CGameMapView::triggerEventBlock(Vec2 pos)
{
	MAP_NODE node = getBlockNode(pos);
	Sprite* block_sp = (Sprite*)(m_batchNode->getChildByTag( pos.y * MAP_WIDTH + pos.x ));

	switch (node.node_type)
	{
	case MAP_NODE_CLOSE:
	case MAP_NODE_ROAD:
		break;
	case  MAP_NODE_DOOR:
		{

			setBlockTexture(block_sp, node.new_block_id);
			setBlockTrigger(pos, true);
		}
		break;
	case  MAP_NODE_ENEMY:
		{

			setBlockTexture(block_sp, node.new_block_id);
			setBlockTrigger(pos, true);
		}
		break;
	case  MAP_NODE_ITEM:
		{

			setBlockTexture(block_sp, node.new_block_id);
			setBlockTrigger(pos, true);
		}
		break;
	case  MAP_NODE_EVENT:
		{

			if (!node.triggered)
			{
				std::string str = Configuration::getInstance()->getValue("dialog_1").asString();
				CFlyTips* tips = CFlyTips::create( str );
				addChild(tips);
			}
			setBlockTexture(block_sp, node.new_block_id);
			setBlockTrigger(pos, true);
		}
		break;
	default:
		break;
	}

	astar::AStarMap<MAP_NODE> x = m_mapData->getMap();
}

Vec2 CGameMapView::getPosByCoordinate(int x, int y)
{
	float posx = m_batchNode->getPositionX() + x * cellWidth;
	float posy = m_batchNode->getPositionY() + y * cellHeight;

	return Vec2(posx, posy);
}

Vec2 CGameMapView::getCoordinateByPos(Vec2 pos)
{
	float xInMap = pos.x - m_batchNode->getPositionX();
	float yInMap = pos.y - m_batchNode->getPositionY();

	if (xInMap < 0 || xInMap > m_nMapWCellNum * cellWidth || yInMap < 0 || yInMap > m_nMapHCellNum * cellHeight)
	{
		return Vec2(ERR_PATH, ERR_PATH);
	}

	return Vec2( (int)(xInMap / cellWidth), (int)(yInMap / cellHeight) );
}

void CGameMapView::updateShadowByMidPos(Vec2 v)
{
	Vec2 dirs[13] = 
	{ Vec2(0, 1), Vec2(0, -1), Vec2(1, 0), Vec2(-1, 0), Vec2(0, 0), 
	  Vec2(0, 2), Vec2(0, -2), Vec2(2, 0), Vec2(-2, 0),
	  Vec2(-1, -1), Vec2(1, -1), Vec2(-1, 1), Vec2(1, 1) };

	for (int i = 0; i < 13; i++)
	{
		Vec2 roundPos = v + dirs[i];
		if( roundPos.x >= 0 && roundPos.x < MAP_WIDTH && roundPos.y >= 0 && roundPos.y < MAP_HEIGHT )
		{
			int index = roundPos.y * MAP_WIDTH + roundPos.x;
			if (index >= 0 && index < m_mapData->m_vMapShadow.size())
			{
				m_mapData->m_vMapShadow[index] = 1;
				m_batchNode->getChildByTag(index)->setVisible(true);
			}
		}
	}
}

bool CGameMapView::isPosInShadow(Vec2 v)
{
	if (m_mapData->m_bHadShadow)
	{
		int state = m_mapData->getPosShadowState( v );
		return state == 0;
	}

	return false;
}

vector<Vec2> CGameMapView::findPath(Vec2 from, Vec2 to)
{
	return m_mapData->findPath(from, to);
}