#include "StdAfx.h"
#include "EffectBreak.h"
#include "GasPella.h"
#include "Defense.h"

CEffectBreak::CEffectBreak(void)
{
}

CEffectBreak::~CEffectBreak(void)
{
}

HRESULT CEffectBreak::Initialize(void)
{
	m_eRenderID = RENDER_EFFECT;

	m_tFrame = FRAME(0.f, 0.f, 5.f, 5.f);

	m_tInfo.vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(170.f, 176.f, 0.f);

	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Break";


	m_fDistX = 0.f;
	m_fDistY = 0.f;

	m_vDist = D3DXVECTOR3(m_fDistX, m_fDistY, 0.f);


// 	switch(m_eBreakID)
// 	{
// 	case BREAK_GASPELLA:
// 		m_pTargetInfo = (CObjMgr::GetInstance()->FindGameObject<CGasPella>()->GetInfo());
// 		break;
// 	case BREAK_DEFENSE:
// 		m_pTargetInfo = (CObjMgr::GetInstance()->FindGameObject<CDefense>()->GetInfo());
// 		break;
// 	case BREAK_BOSS:
// 		break;
// 	case BREAK_MYOBJECT:
// 		break;
// 	}

//	m_tInfo.vPos = m_pTargetInfo->vPos;

	return S_OK; 
}

eOBJECT_RESULT CEffectBreak::Update(void)
{
	

	FrameCheck();
	MatrixCheck();

// 	if(m_pTargetInfo != NULL)
// 	{
// 		m_tInfo.vPos = m_pTargetInfo->vPos;
// 	}

	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	return OR_OK;

}

void CEffectBreak::Render(void)
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

void CEffectBreak::Release(void)
{

}

void CEffectBreak::FrameCheck(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime()* 2.f;

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_bIsDead = true;
	}
}

void CEffectBreak::MatrixCheck(void)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z
		);

	m_tInfo.matWorld = matTrans;

}
