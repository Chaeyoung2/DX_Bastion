#include "StdAfx.h"
#include "EffectHammer.h"
#include "ObjMgr.h"
#include "Player.h"

EffectHammer::EffectHammer(void)
{
}

EffectHammer::~EffectHammer(void)
{
}

HRESULT EffectHammer::Initialize(void)
{
	m_eRenderID = RENDER_EFFECT;

	m_tFrame = FRAME(0.f, 0.f, 14.f, 14.f);

	//m_tInfo.vDir  = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(208.f, 140.f, 0.f);

	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Hammer";


	m_fDistX = 50.f;
	m_fDistY = 50.f;

	m_vDist = D3DXVECTOR3(m_fDistX, m_fDistY, 0.f);


	m_pTargetInfo = (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo());


	m_tInfo.vPos = m_pTargetInfo->vPos;

	return S_OK; 

}

eOBJECT_RESULT EffectHammer::Update(void)
{

	FrameCheck();
	MatrixCheck();

	m_tInfo.vPos = m_pTargetInfo->vPos;

	// ¹æÇâº°·Î dist ³ª´²ÁÜ
	switch(m_eDir)
	{
	case DIR_B:
		m_vDist = D3DXVECTOR3(0.f, m_fDistY, 0.f);
		break;
	case DIR_L:
		m_vDist = D3DXVECTOR3(-m_fDistX, 0.f, 0.f);
		break;
	case DIR_LB:
		m_vDist = D3DXVECTOR3(-m_fDistX, m_fDistY, 0.f);
		break;
	case DIR_LT:
		m_vDist = D3DXVECTOR3(-m_fDistX, -m_fDistY, 0.f);
		break;
	case DIR_T:
		m_vDist = D3DXVECTOR3(0.f, -m_fDistY, 0.f);
		break;
	case DIR_R:
		m_vDist = D3DXVECTOR3(m_fDistX, 0.f, 0.f);
		break;
	case DIR_RT:
		m_vDist = D3DXVECTOR3(m_fDistX, -m_fDistY, 0.f);
		break;
	case DIR_RB:
		m_vDist = D3DXVECTOR3(m_fDistX, m_fDistY, 0.f);
		break;
	}

	if(m_bIsDead )
	{
		return OR_DELETE;
	}

	return OR_OK;

}

void EffectHammer::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), static_cast<int>(m_tFrame.fFrame) );

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

void EffectHammer::Release(void)
{

}

void EffectHammer::FrameCheck(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime() * 1.5f;

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_bIsDead = true;
	}

}

void EffectHammer::MatrixCheck(void)
{


	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vDist.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vDist.y + m_vScroll.y,
		m_tInfo.vPos.z
		);

	m_tInfo.matWorld = matTrans;
}

