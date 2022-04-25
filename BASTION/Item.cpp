#include "StdAfx.h"
#include "Item.h"

CItem::CItem(void)
:m_bIsGetCompleted(false), m_iItemNumber(0)
{
}

CItem::~CItem(void)
{
}

void CItem::SettingCollRect()
{
	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - m_tInfo.vSize.y * 0.5f);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + m_tInfo.vSize.y * 0.5f);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - m_tInfo.vSize.x * 0.5f);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + m_tInfo.vSize.x * 0.5f);
}
