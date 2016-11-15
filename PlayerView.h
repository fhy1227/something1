#pragma once

#include <iostream>
#include "cocos2d.h"
#include "Player.h"
#include "GameMapView.h"

using namespace std;
using namespace cocos2d;

class CPlayerView : public Layer
{
public:
	CPlayerView();
	virtual ~CPlayerView();

public:
	static CPlayerView* create(int id, int x = 0, int y = 0);
	void setPos(int x, int y);
	void setMapData(CGameMapView* mapview);
	void updateShadow();

	void moveTo(Vec2 to);

private:
	bool init();
	bool initWithId(int id, int x, int y);

public:
	CPlayer* getPlayer();

public:
	virtual void update(float delta);

public:
	CPlayer* m_playerData;

private:
	Sprite* m_player;
	CGameMapView* m_mapView;

	Label* m_pLabelHp;
	Label* m_pLabelEndurance;
};

