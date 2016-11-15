#include "PackageBag.h"

CPackageBag* CPackageBag::m_instance = NULL;

CPackageBag::CPackageBag()
{	
}

CPackageBag::~CPackageBag(void)
{	
}

CPackageBag* CPackageBag::getInstance()
{
	if (!m_instance)
	{
		m_instance = new CPackageBag();
	}
	return m_instance;
}

void CPackageBag::addItem(ITEM_STRUCT item)
{
	m_mapBagItem[item.uid] = item;
}

void CPackageBag::removeItem(ITEM_STRUCT item)
{
	auto it = m_mapBagItem.find(item.uid);
	if (it != m_mapBagItem.end())
	{
		m_mapBagItem.erase( it );
	}
}
