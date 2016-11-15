#include "PlayerView.h"
#include "def.h"
#include "AStar.h"


CPlayerView::CPlayerView()
{
	m_playerData = NULL;
	m_player = NULL;
	m_mapView = NULL;
}

CPlayerView::~CPlayerView(void)
{
	if(m_playerData)
	{
		delete m_playerData;
		m_playerData = NULL;
	}
}

CPlayerView* CPlayerView::create(int id, int x /* = 0 */, int y /* = 0 */)
{
	CPlayerView* view = new CPlayerView();
	if( view && view->initWithId(id, x, y) && view->init() )
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return NULL;
}

bool CPlayerView::init()
{
	if(!Layer::init())
		return false;

	float cellWidth = 33;
	float cellHeight = 33;

	int playerId = m_playerData->getId();
	if ( playerId == 1 )
	{
		m_player = Sprite::create( "CloseNormal.png" );
	}
	else if (playerId == 2)
	{
		m_player = Sprite::create( "CloseSelected.png" );
	}
	m_player->setAnchorPoint(Vec2(0, 0));
	m_player->setScale(0.8);
	m_player->setPosition(Vec2(m_playerData->getX() * cellWidth, m_playerData->getY() * cellHeight));
	addChild(m_player);


	CCSize s = CCDirector::sharedDirector()->getWinSize();

	m_pLabelHp = Label::create("", "黑体", 18);
	m_pLabelHp->setPosition(Vec2(40, s.height - 18));
	addChild(m_pLabelHp);
	m_pLabelEndurance = Label::create("", "黑体", 18);
	m_pLabelEndurance->setPosition(Vec2(200, s.height - 18));
	addChild(m_pLabelEndurance);

	this->scheduleUpdate();

	return true;
}

bool CPlayerView::initWithId(int id, int x, int y)
{
	m_playerData = new CPlayer(id, x, y);
	return true;
}

void CPlayerView::setPos(int x, int y)
{
	if ( m_mapView )
	{
		Vec2 p = m_mapView->getPosByCoordinate(x, y);
		setPosition(p);
	}
}

void CPlayerView::setMapData(CGameMapView* mapview)
{
	m_mapView = mapview;
}

void CPlayerView::updateShadow()
{
	m_mapView->updateShadowByMidPos( Vec2( m_playerData->getX(), m_playerData->getY() ) );
}

CPlayer* CPlayerView::getPlayer()
{
	return m_playerData;
}

//移动到地图上某个坐标
void CPlayerView::moveTo(Vec2 to)
{
	if ( !m_mapView )
	{
		return;
	}
	int fromX = m_playerData->getX();
	int fromY = m_playerData->getY();
	int toX = to.x;
	int toY = to.y;

	vector<Vec2> path = m_mapView->findPath(Vec2(fromX, fromY), to);

	if (path.size() > 0 && path.size() - 1 > m_playerData->getEndurance())
	{
		std::string str = Configuration::getInstance()->getValue("endurance_less").asString();
		CFlyTips* tips = CFlyTips::create( str );
		addChild(tips);
		return;
	}
	
	m_playerData->setState(MOVE);

	Vector< FiniteTimeAction*> array;
	for( vector<Vec2>::iterator it = path.begin(); it != path.end(); it++ )
	{
		if ( !( *it == Vec2(fromX, fromY) ) )
		{
			Vec2 nowto = m_mapView->getPosByCoordinate(it->x, it->y);
			MoveTo* moveto = MoveTo::create(0.3f, nowto);
			array.pushBack(moveto);

			int x = it->x;
			int y = it->y;
			array.pushBack( CallFunc::create( [this, x, y](){
				m_playerData->setPos(x, y);
				int endurance = m_playerData->getEndurance();
				m_playerData->setEndurance( endurance - 1 );
				updateShadow();
			}  ) );
		}
	}
	array.pushBack( CallFunc::create( [&, to](){
		m_mapView->triggerEventBlock(to);
		m_playerData->setState(IDLE);
	}  ) );

	m_player->runAction(Sequence::create( array ));

}

void CPlayerView::update(float delta)
{
	char str[100] = {0};
	sprintf_s(str, "HP:%d", m_playerData->getHp());
	m_pLabelHp->setString(str);
	sprintf_s(str, "Endurance:%d", m_playerData->getEndurance());
	m_pLabelEndurance->setString(str);
}

