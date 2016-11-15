#include "MyGame.h"
#include "def.h"
#include "mapEditor/MapReader.h"

MyGame::MyGame(void)
{
}


MyGame::~MyGame(void)
{
}

Scene* MyGame::createScene()
{
    auto scene = Scene::create();
    auto layer = MyGame::create();
    scene->addChild(layer);
    return scene;
}

bool MyGame::init()
{
	if(!Layer::init())
		return false;

	//Sprite* bg = Sprite::create("bg.jpg");
	//bg->setScale(2.0f);
	//addChild(bg);

	vector<MAP_NODE> m = CMapReader::readMapData("mapData.txt");
	m_pMapView = CGameMapView::create(m);
	addChild(m_pMapView);

	m_pPlayerView = CPlayerView::create(1, 0, 0);
	addChild(m_pPlayerView);
	m_pPlayerView->setMapData(m_pMapView);
	m_pPlayerView->updateShadow();

	/////////////////

	startGame();

	return true;
}

void MyGame::startGame()
{
	this->setTouchEnabled(true);

	m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = CC_CALLBACK_2(MyGame::onTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(MyGame::onTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(MyGame::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void MyGame::startOneRound()
{
	m_pPlayerView->m_playerData->resetEndurance();
}
void MyGame::finishOneRound()
{

}

bool  MyGame::onTouchBegan(Touch  *touch, Event  *event)
{
	/*Vec2 pos = touch->getLocation();
	Vec2 coordinate = m_pMapView->getCoordinateByPos(pos);
	if ( coordinate.x == ERR_PATH || coordinate.y == ERR_PATH  )
	{
		return false;
	}*/

	return true;
}
void  MyGame::onTouchMoved(Touch  *touch, Event  *event)
{

}

bool MyGame::isBlockCanCross(Vec2 coordinate)
{
	MAP_NODE block = m_pMapView->getBlockNode( coordinate );

	bool ret = false;
	if (block.node_type != MAP_NODE_CLOSE)
	{
		ret = true;
	}

	if(m_pMapView->isPosInShadow(coordinate))
	{
		ret = false;
	}

	return ret;
}

void  MyGame::onTouchEnded(Touch  *touch, Event  *event)
{
	Vec2 pos = touch->getLocation();
	Vec2 coordinate = m_pMapView->getCoordinateByPos(pos);
	if ( coordinate.x == ERR_PATH || coordinate.y == ERR_PATH  )
	{
		return;
	}

	PlayerState state = m_pPlayerView->getPlayer()->getState();
	bool b = isBlockCanCross(coordinate);
	if ( m_pPlayerView->getPlayer()->getState() == IDLE && isBlockCanCross(coordinate) )
	{
		m_pPlayerView->moveTo(coordinate);
	}
}

////////////////
void MyGame::onEnter()
{
	Layer::onEnter();
}
void MyGame::onExit()
{
	Layer::onExit();
}