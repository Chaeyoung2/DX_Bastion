#include "StdAfx.h"
#include "Hammer.h"

CHammer::CHammer(void)
{
}

CHammer::~CHammer(void)
{
}

HRESULT CHammer::Initialize(void)
{
	m_eRenderID = RENDER_ITEM;

	m_tInfo.vPos = D3DXVECTOR3(2591.f, 1704.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(45.f, 95.f, 0.f);

	m_wstrObjKey = L"Texture";

	m_iItemNumber = 3;

	SettingCollRect();

	return S_OK;
}

eOBJECT_RESULT CHammer::Update(void)
{
	if(m_bIsDead == true)
		return OR_DELETE;
	
	return OR_OK;
}

void CHammer::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		L"Item", 3 );

	if(pTex == NULL)		return;

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + m_vScroll.x,
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

void CHammer::Release(void)
{

}
