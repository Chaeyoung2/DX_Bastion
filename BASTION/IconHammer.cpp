#include "StdAfx.h"
#include "IconHammer.h"
#include "UIObserver.h"



CIconHammer::CIconHammer(void)
{
}

CIconHammer::~CIconHammer(void)
{
}

HRESULT CIconHammer::Initialize(void)
{
	m_bIsCarryingHammer = false;

	m_tInfo.vPos = D3DXVECTOR3(1010.f, 624.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(60.f, 60.f, 0.f);

	m_pObserver = new CUIObserver;
	m_pDataSubject->Subscribe(m_pObserver);

	m_wstrObjKey = L"UI";
	m_wstrStateKey = L"Default";

	m_eRenderID = RENDER_UI;


	return S_OK;

}

eOBJECT_RESULT CIconHammer::Update(void)
{

	m_bIsCarryingHammer = ((CUIObserver*)m_pObserver)->GetData()->bIsHammer;


	D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x, 
		m_tInfo.vPos.y,
		0.f);

	m_tInfo.matWorld = matTrans;


	return OR_OK;
}

void CIconHammer::Render(void)
{

	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), 3);

	if(pTex == NULL)		return;

	float	fX = pTex->ImgInfo.Width * 0.5f;
	float	fY = pTex->ImgInfo.Height * 0.5f;

	m_pSprite->SetTransform(&m_tInfo.matWorld);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}

void CIconHammer::Release(void)
{

}
