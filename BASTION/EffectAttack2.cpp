#include "StdAfx.h"
#include "EffectAttack2.h"
#include "GasPella.h"
#include "Player.h"

CEffectAttack2::CEffectAttack2(void)
{
}

CEffectAttack2::~CEffectAttack2(void)
{
}

HRESULT CEffectAttack2::Initialize(void)
{
	m_eRenderID = RENDER_EFFECT;

	m_tFrame = FRAME(0.f, 0.f, 2.f, 2.f);

	m_tInfo.vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(182.f, 287.f, 0.f);

	m_fDistX = 10.f;
	m_fDistY = 10.f;

	m_vDist = D3DXVECTOR3(m_fDistX, m_fDistY, 0.f);

	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Attack2";

	m_pTargetInfo = (m_pObjMgr->FindGameObject<CPlayer>()->GetInfo());

	m_tInfo.vPos = m_pTargetInfo->vPos;

	return S_OK;
}

eOBJECT_RESULT CEffectAttack2::Update(void)
{
	// 오브젝트 공통
	FrameCheck();
	MatrixCheck();

	m_tInfo.vPos = m_pTargetInfo->vPos;

	// 방향별로 dist 나눠줌
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


	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	return OR_OK;

}

void CEffectAttack2::Render(void)
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

void CEffectAttack2::Release(void)
{

}

void CEffectAttack2::FrameCheck(void)
{
// 	system("cls");
// 	cout << "m_tFrame.fFrame : " << m_tFrame.fFrame << endl;
// 	cout << "m_tFrame.fMaxFrame : " << m_tFrame.fMaxFrame << endl;
// 	cout << "m_bIsDead : " << m_bIsDead << endl;

	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime() * 4.f;

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_bIsDead = true;
	}


}

void CEffectAttack2::MatrixCheck(void)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vDist.x  + m_vScroll.x,
		m_tInfo.vPos.y + m_vDist.y  + m_vScroll.y,
		m_tInfo.vPos.z
		);

	m_tInfo.matWorld = matTrans;

}
