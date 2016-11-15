#pragma once

#include "cocos2d.h"
#include "GameMapView.h"
#include "PlayerView.h"
#include "Player.h"
#include "GameMap.h"

using namespace cocos2d;

class MyGame : public Layer
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(MyGame);

public:
	MyGame(void);
	virtual ~MyGame(void);

	virtual bool init();

public:
	void startGame();

public:
	void startOneRound();
	void finishOneRound();

public:
	bool  onTouchBegan(Touch  *touch, Event  *event);
	void  onTouchMoved(Touch  *touch, Event  *event);
	void  onTouchEnded(Touch  *touch, Event  *event);

private:
	bool MyGame::isBlockCanCross(Vec2 coordinate);

public:
	virtual void onEnter() override;
    virtual void onExit() override;

private:
	CPlayerView* m_pPlayerView;
	CGameMapView* m_pMapView;

private:
	EventListenerTouchOneByOne* m_listener;
};

