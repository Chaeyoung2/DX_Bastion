#include "StdAfx.h"
#include "MonBullet.h"


CMonBullet::CMonBullet(void)
{
}

CMonBullet::~CMonBullet(void)
{
}

HRESULT CMonBullet::Initialize(void)
{
	m_tFrame = FRAME(0.f, 0.f, 9.f, 3.f);

	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(37.f, 37.f, 0.f);

	m_wstrObjKey = L"Bullet";
	m_wstrStateKey = L"MonsterBullet";

	m_fSpeed = 200.f;

	m_eRenderID = RENDER_EFFECT;

	return S_OK;
}

eOBJECT_RESULT CMonBullet::Update(void)
{

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetTime() * m_fSpeed;

// #ifdef CHAEYOUNG
// 	//Test할만한 코드
// 
// 	if(m_tInfo.vPos.x >= WINCX || m_tInfo.vPos.x <= 0
// 		|| m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY)
// 	{
// 		return OR_DELETE;
// 	}
// #else
// 	// CHAEYOUNG이 Define 없을때
// 	int a = 1;
// #endif

	FrameCheck();
	MatrixCheck();

	return OR_OK;
}

void CMonBullet::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), static_cast<int>(m_tFrame.fFrame) );

	if(pTex == NULL)		return;


	m_pSprite->SetTransform(&m_tInfo.matWorld);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}


void CMonBullet::MatrixCheck()
{

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x, 
		m_tInfo.vPos.y + m_vScroll.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matTrans;

}



void CMonBullet::FrameCheck()
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime() * 0.5f;

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fFrame = m_tFrame.fOriFrame;
	}
}


void CMonBullet::Release(void)
{

}

void CMonBullet::SettingCollRect()
{
	float fCollRectY = m_tInfo.vSize.y * 0.5f;
	float fCollRectX = m_tInfo.vSize.x * 0.5f;

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - fCollRectY);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fCollRectY);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - fCollRectX);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + fCollRectX);
}
