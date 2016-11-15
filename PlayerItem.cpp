#include "PlayerItem.h"
#include "PackageBag.h"

CPlayerItem* CPlayerItem::m_instance = NULL;

CPlayerItem::CPlayerItem()
{
}

CPlayerItem::~CPlayerItem(void)
{
}

CPlayerItem* CPlayerItem::getInstance()
{
	if (!m_instance)
	{
		m_instance = new CPlayerItem();
	}
	return m_instance;
}

ITEM_STRUCT CPlayerItem::getItemById(ITEM_ID item_id)
{
	return m_mapItem[item_id];
}

void CPlayerItem::equipItemFromBag(ITEM_STRUCT item)
{
	CPackageBag* bag = CPackageBag::getInstance();

	ITEM_STRUCT old_item = getItemById(item.id);
	if ( old_item.id != ITEM_NONE )
	{
		bag->addItem(old_item);
	}
	bag->removeItem(item);
	m_mapItem[item.id] = item;
}

void CPlayerItem::equipItem(ITEM_STRUCT item)
{
	CPackageBag* bag = CPackageBag::getInstance();

	ITEM_STRUCT old_item = getItemById(item.id);
	if ( old_item.id != ITEM_NONE )
	{
		bag->addItem(old_item);
	}
	m_mapItem[item.id] = item;
}

void CPlayerItem::unloadItem(ITEM_ID item_id)
{
	CPackageBag* bag = CPackageBag::getInstance();

	ITEM_STRUCT old_item = getItemById(item_id);
	if ( old_item.id != ITEM_NONE )
	{
		bag->addItem(old_item);
		m_mapItem[item_id] = ITEM_STRUCT();
	}
}

