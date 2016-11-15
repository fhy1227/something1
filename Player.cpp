#include "Player.h"
#include "def.h"


CPlayer::CPlayer(int id, int x, int y):
m_x(x),
m_y(y),
m_state(IDLE)
{
	m_id = id;
	switch (id)
	{
	case 1:
		{
			m_name = "aa";
			m_speed = 10;
			m_skill = SPEED_UP;
			m_nTotalHp = 100;
			m_nTotalEndurance = 50;
		}
		break;
	case 2:
		{
			m_name = "bb";
			m_speed = 20;
			m_skill = BOMB;
			m_nTotalHp = 100;
			m_nTotalEndurance = 50;
		}
		break;
	default:
		break;
	}

	m_nHp = m_nTotalHp;
	m_nEndurance = m_nTotalEndurance;
}


CPlayer::~CPlayer(void)
{
}


