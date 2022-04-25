#include "StdAfx.h"
#include "EffectPotion.h"
#include "ObjMgr.h"
#include "Player.h"


CEffectPotion::CEffectPotion(void)
{
}

CEffectPotion::~CEffectPotion(void)
{
}

HRESULT CEffectPotion::Initialize(void)
{
	m_eRenderID = RENDER_EFFECT;

	m_tFrame = FRAME(0.f, 0.f, 23.f, 23.f);

	//m_tInfo.vDir  = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(172.f, 192.f, 0.f);

	m_vDist = D3DXVECTOR3(0.f,0.f,0.f);

	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Potion";

// 	m_dwCreateCurTime = GetTickCount();
// 	m_dwCreateOldTime = m_dwCreateCurTime;

	m_pTargetInfo = (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo());

	m_tInfo.vPos = m_pTargetInfo->vPos;

	return S_OK; 
}


eOBJECT_RESULT CEffectPotion::Update(void)
{
	// 오브젝트 공통
	FrameCheck();
	MatrixCheck();

	m_tInfo.vPos = m_pTargetInfo->vPos;


/*	m_dwCreateCurTime = GetTickCount();*/
	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	return OR_OK;
}

void CEffectPotion::Render(void)
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

void CEffectPotion::Release(void)
{

}

void CEffectPotion::FrameCheck(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime();

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_bIsDead = true;
	}

}

void CEffectPotion::MatrixCheck(void)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z
		);

	m_tInfo.matWorld = matTrans;

}
