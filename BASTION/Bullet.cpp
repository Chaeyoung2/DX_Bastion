#include "StdAfx.h"
#include "Bullet.h"
#include "Player.h"


CBullet::CBullet(void)
{
}

CBullet::~CBullet(void)
{
}

HRESULT CBullet::Initialize(void)
{
	m_tFrame = FRAME(0.f, 0.f, 9.f, 3.f);

	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(37.f, 37.f, 0.f);

	m_wstrObjKey = L"Bullet";
	m_wstrStateKey = L"PlayerBullet";

		m_eRenderID = RENDER_EFFECT;

	m_fSpeed = 300.f;

	m_iAtt = (m_pObjMgr->FindGameObject<CPlayer>()->GetData().iAtt);

	m_dwCreateCurTime = GetTickCount();
	m_dwCreateOldTime = m_dwCreateCurTime;


	return S_OK;
}

eOBJECT_RESULT CBullet::Update(void)
{

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetTime() * m_fSpeed;

// 	if(m_tInfo.vPos.x >= WINCX || m_tInfo.vPos.x <= 0
// 		|| m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY)
// 	{
// 		return OR_DELETE;
// 	}

// 	cout << "Bullet" << endl;
// 	 	cout << "m_tCollRect.top : " << m_tCollRect.top << endl;
// 	 	cout <<	"m_tCollRect.bottom : " << m_tCollRect.bottom << endl;
// 	 	cout <<	"m_tCollRect.left : " << m_tCollRect.left  << endl;
// 	 	cout << "m_tCollRect.right : " << m_tCollRect.right << endl << endl;

	m_dwCreateCurTime = GetTickCount();

	if(m_dwCreateOldTime + 5000 < m_dwCreateCurTime)
	{
		return OR_DELETE;
	}

	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	FrameCheck();
	MatrixCheck();

	SettingCollRect();

	return OR_OK;
}

void CBullet::Render(void)
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


void CBullet::MatrixCheck()
{

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z);

	m_tInfo.matWorld = matTrans;

}



void CBullet::FrameCheck()
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime() * 0.5f;

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fFrame = m_tFrame.fOriFrame;
	}
}


void CBullet::Release(void)
{

}

void CBullet::SettingCollRect()
{
	float fCollRectY = m_tInfo.vSize.y * 0.5f;
	float fCollRectX = m_tInfo.vSize.x * 0.5f;

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - fCollRectY);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fCollRectY);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - fCollRectX);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + fCollRectX);

}
