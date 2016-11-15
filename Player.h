#pragma once

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

enum PlayerState
{
	IDLE,
	MOVE,
};

enum E_SKILL
{
	SPEED_UP,
	BOMB,
};

class CPlayer : public CCObject
{
public:
	CPlayer(int id, int x = 0, int y = 0);
	virtual ~CPlayer(void);

	void setState(PlayerState e){m_state = e;}
	PlayerState getState(){return m_state;}

	int getX(){return m_x;}
	int getY(){return m_y;}
	void setPos(int x, int y){m_x = x; m_y = y;}

	int	getId(){return m_id;}

	int getHp(){return m_nHp;}
	int getEndurance(){return m_nEndurance;}
	void setHp(int hp){m_nHp = hp;}
	void setEndurance(int endurance){m_nEndurance = endurance;}

	int getTotalEndurance(){return m_nTotalEndurance;}
	int getTotalHp(){return m_nTotalHp;}
	void setTotalHp(int hp){m_nTotalHp = hp;}
	void setTotalEndurance(int endurance){m_nTotalEndurance = endurance;}

	void resetEndurance(){m_nEndurance = m_nTotalEndurance;}

private:
	string	m_name;
	int		m_speed;
	int		m_id;
	E_SKILL	m_skill;

	PlayerState m_state;
	int m_x, m_y;

	int		m_nTotalHp;
	int		m_nTotalEndurance;

	int		m_nHp;
	int		m_nEndurance;

};