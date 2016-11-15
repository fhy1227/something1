#include "FlyTips.h"

CFlyTips::CFlyTips()
{	
}

CFlyTips::~CFlyTips(void)
{	
}

CFlyTips* CFlyTips::create(std::string tip)
{
	CFlyTips* tips = new CFlyTips();
	if (tips && tips->init(tip))
	{
		return tips;
	}
	tips = NULL;

	return NULL;
}

bool CFlyTips::init(std::string tip)
{
	if (!Layer::init())
	{
		return false;
	}

	CCSize s = CCDirector::sharedDirector()->getWinSize();

	m_pLabelTips = Label::create(tip, "ºÚÌå", 24);
	m_pLabelTips->setPosition(Vec2( s.width / 2, s.height / 2 - 50 ));
	addChild(m_pLabelTips);

	MoveBy* move = MoveBy::create(2.0f, Vec2( 0, 100 ));
	CallFunc* func = CallFunc::create( [this](){
		removeFromParent();
	});

	m_pLabelTips->runAction( Sequence::createWithTwoActions(move, func) );


	return true;
}