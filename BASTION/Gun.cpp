#include "StdAfx.h"
#include "Gun.h"

CGun::CGun(void)
{
}

CGun::~CGun(void)
{
}

HRESULT CGun::Initialize(void)
{
	m_eRenderID = RENDER_ITEM;

	m_tInfo.vSize = D3DXVECTOR3(80.f, 79.f, 0.f);

	m_wstrObjKey = L"Texture";

	m_iItemNumber = 4;

	SettingCollRect();

	return S_OK;
}

eOBJECT_RESULT CGun::Update(void)
{
	if(m_bIsDead == true)
		return OR_DELETE;

	return OR_OK;
}

void CGun::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		L"Item", 4);

	if(pTex == NULL)		return;

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y, 0.f);

	float fX = pTex->ImgInfo.Width * 0.5f;
	float fY = pTex->ImgInfo.Height * 0.5f;

	m_pSprite->SetTransform(&matTrans);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}

void CGun::Release(void)
{

}
