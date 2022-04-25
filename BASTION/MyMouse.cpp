#include "StdAfx.h"
#include "MyMouse.h"


CMyMouse::CMyMouse(void)
{
}

CMyMouse::~CMyMouse(void)
{
}

HRESULT CMyMouse::Initialize(void)
{
	m_tFrame = FRAME(0.f, 0.f, 4.f, 4.f);

	m_tInfo.vPos = GetMousePos();
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(128.f, 128.f, 0);

	m_wstrObjKey = L"MOUSE";
	m_wstrStateKey = L"Mouse";

	m_eRenderID = RENDER_SCREEN;

	m_fAngle = 0.f;

	return S_OK;
}

eOBJECT_RESULT CMyMouse::Update(void)
{
	m_fTime += m_pTimeMgr->GetTime();

	MatrixCheck();
	FrameCheck();

	//추후 전투 상태일 때 if문 걸어주기
	switch(g_eScene)
	{
	case SCENE_LOGO:
		m_tFrame.fFrame = 2.f;
		break;
	case SCENE_MENU:
		m_tFrame.fFrame = 2.f;
		break;
	case SCENE_STAGE1:
		m_tFrame.fFrame = 0.f;
		break;
	case SCENE_STAGE2:
		m_tFrame.fFrame = 0.f;
		break;
	}

	m_tInfo.vPos = GetMousePos();

	return OR_OK;
}

void CMyMouse::Render(void)
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

void CMyMouse::Release(void)
{

}

void CMyMouse::FrameCheck(void)
{

}

void CMyMouse::MatrixCheck(void)
{
	D3DXMATRIX matTrans, matRotZ;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x,
		m_tInfo.vPos.y ,
		m_tInfo.vPos.z
		);

	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	if(m_tFrame.fFrame == 0 || m_tFrame.fFrame == 1)
	{
		if(m_fTime >= 0.05f)
		{
			m_fAngle += 0.05f;
			m_fTime = 0.f;
		}
	}

	m_tInfo.matWorld = matRotZ * matTrans;
}
