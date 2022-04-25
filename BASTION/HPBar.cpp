#include "StdAfx.h"
#include "HPBar.h"
#include "UIObserver.h"


CHPBar::CHPBar(void)
{
}

CHPBar::~CHPBar(void)
{
	Release();
}

HRESULT CHPBar::Initialize( void )
{
	m_iHp = 0;

	m_tInfo.vPos = D3DXVECTOR3(253.f, 52.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(356.f, 25.f, 0.f);
 
	m_pObserver = new CUIObserver;
	m_pDataSubject->Subscribe(m_pObserver);

	m_wstrObjKey = L"UI";
	m_wstrStateKey = L"Default";

	m_eRenderID = RENDER_UI;

	m_iHp =		((CUIObserver*)m_pObserver)->GetData()->iHp;
	m_iHpMax = 2000;




	m_fHpValue = ((float)m_iHp / (float)m_iHpMax);

	return S_OK;

}

eOBJECT_RESULT CHPBar::Update( void )
{
	m_iHp = ((CUIObserver*)m_pObserver)->GetData()->iHp;

	m_fHpValue = ((float)m_iHp / (float)m_iHpMax);


	D3DXMATRIX		matTrans, matScale;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x - m_fWidth * (1 - m_fHpValue) * 0.5f * 0.5f * 0.65f, 
		m_tInfo.vPos.y,
		0.f);

	D3DXMatrixScaling(&matScale, m_fHpValue, 1.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;


	return OR_OK;
}

void CHPBar::Render( void )
{
	///////// UI 배경 여기에서 렌더할고임
	const TEX_INFO* pBackTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), 0);

	if(pBackTex == NULL)		return;

	m_fWidth = pBackTex->ImgInfo.Width; 

	float fX = pBackTex->ImgInfo.Width * 0.5f;
	float fY = pBackTex->ImgInfo.Height * 0.5f;

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);

	m_pSprite->SetTransform(&matTrans);

	m_pSprite->Draw(
		pBackTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);



	///////// HP BAR 
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), 1);

	if(pTex == NULL)		return;

 fX = pTex->ImgInfo.Width * 0.5f;
 fY = pTex->ImgInfo.Height * 0.5f;

	m_pSprite->SetTransform(&m_tInfo.matWorld);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);


}

void CHPBar::Release( void )
{

}
