#include "StdAfx.h"
#include "SubTitle.h"

CSubTitle::CSubTitle(void)
:m_fCreateTime(0.f)
{
}

CSubTitle::~CSubTitle(void)
{
}

HRESULT CSubTitle::Initialize(void)
{
	m_eRenderID = RENDER_SUBTITLE;


	m_tInfo.vSize = D3DXVECTOR3(1000.f, 156.f, 0.f);
	m_tInfo.vPos = D3DXVECTOR3(WINCX * 0.5f, WINCY - 150.f, 0.f);

	m_wstrObjKey = L"SUBTITLE";
	m_wstrStateKey = L"SubTitle";

	return S_OK; 
}

eOBJECT_RESULT CSubTitle::Update(void)
{
	m_fCreateTime += m_pTimeMgr->GetTime();

	//FrameCheck();
	MatrixCheck();

	if(m_bIsDead)
	{
		if(m_tFrame.fFrame == 1.f)
			g_bIsUIRender = true;
		return OR_DELETE;
	}

	if(m_tFrame.fFrame == 10.f)
	{
		if(m_fCreateTime >= 2.5f)
		{
			m_bIsDead = true;
		}
	}
	else
	{
		if(m_fCreateTime >= 4.f)
		{
			m_bIsDead = true;
		}
	}
	

	return OR_OK;
}

void CSubTitle::Render(void)
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

void CSubTitle::Release(void)
{

}

void CSubTitle::FrameCheck(void)
{

}

void CSubTitle::MatrixCheck(void)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x ,
		m_tInfo.vPos.y ,
		m_tInfo.vPos.z
		);

	m_tInfo.matWorld = matTrans;
}
