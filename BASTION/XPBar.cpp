#include "StdAfx.h"
#include "XPBar.h"
#include "UIObserver.h"

CXPBar::CXPBar(void)
{
}

CXPBar::~CXPBar(void)
{
	Release();
}

HRESULT CXPBar::Initialize(void)
{
	m_iXp = 0;

	m_tInfo.vPos = D3DXVECTOR3(180.f, 76.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(141.f, 13.f, 0.f);

	m_pObserver = new CUIObserver;
	m_pDataSubject->Subscribe(m_pObserver);

	m_wstrObjKey = L"UI";
	m_wstrStateKey = L"Default";

	m_eRenderID = RENDER_UI;


	return S_OK;
}

eOBJECT_RESULT CXPBar::Update(void)
{
	m_iXp = ((CUIObserver*)m_pObserver)->GetData()->iXp;

	D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x, 
		m_tInfo.vPos.y,
		0.f);

	m_tInfo.matWorld = matTrans;


	return OR_OK;

}

void CXPBar::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), 2);

	if(pTex == NULL)		return;

	float fX = pTex->ImgInfo.Width * 0.5f;
	float fY = pTex->ImgInfo.Height * 0.5f;

	m_pSprite->SetTransform(&m_tInfo.matWorld);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);

}

void CXPBar::Release(void)
{

}
