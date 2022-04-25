#include "StdAfx.h"
#include "Portal.h"

CPortal::CPortal(void)
{
}

CPortal::~CPortal(void)
{
}

HRESULT CPortal::Initialize(void)
{
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(80.f, 80.f, 0);

	m_iWhereToGo = 1;

	SettingCollRect();

	return S_OK;
}

eOBJECT_RESULT CPortal::Update(void)
{
	return OR_OK;
}

void CPortal::Render(void)
{

}

void CPortal::Release(void)
{

}

void CPortal::SettingCollRect()
{
	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - m_tInfo.vSize.y * 0.5f);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + m_tInfo.vSize.y * 0.5f);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - m_tInfo.vSize.x * 0.5f);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + m_tInfo.vSize.x * 0.5f);
}
