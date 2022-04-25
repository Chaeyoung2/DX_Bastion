#include "StdAfx.h"
#include "BlackScreen.h"


CBlackScreen::CBlackScreen(void)
:m_iAlphaValue(0), m_fAlphaSpeed(10.f), m_fTime(0.f), m_fFadeTime(0.1f)
{
}

CBlackScreen::~CBlackScreen(void)
{
}

HRESULT CBlackScreen::Initialize(void)
{


	m_tInfo.vPos = D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0);
	m_tInfo.vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3((float)WINCX, (float)WINCY, 0);

	m_wstrObjKey = L"BackGround";

	m_eRenderID = RENDER_SCREEN;

	return S_OK;
}

eOBJECT_RESULT CBlackScreen::Update(void)
{
	// 씬마다 페이드되는 시간을 다르게
	if(g_eScene == SCENE_MENU)
	{
		m_fFadeTime = 0.07f;
	}
	else if(g_eScene == SCENE_STAGE1)
	{
		m_fFadeTime = 0.35f;
	}
	else if(g_eScene == SCENE_DIA)
	{
		m_fFadeTime = 0.03f;
	}



	if(g_bIsFadeOut == true)
	{
		FadeOut();
	}
	if(g_bIsFadeIn == true)
	{
		FadeIn();
	}

// 	system("cls");
// 	cout << "g_bIsFadeIn : "<< g_bIsFadeIn << endl;
// 		cout << "g_bIsFadeOut : "<< g_bIsFadeOut << endl;
// 		cout << "m_iAlphaValue : "<< m_iAlphaValue << endl;
// 		cout << "m_iAlphaSpeed : "<< m_iAlphaSpeed << endl;


	return OR_OK;
}

void CBlackScreen::Render(void)
{
	D3DXMATRIX matTrans;

	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		L"BlackBack", 0 );

	if(pTex == NULL)		return;

	float fX = pTex->ImgInfo.Width * 0.5f;
	float fY = pTex->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);																																																																																																																																																																										;

	m_pSprite->SetTransform(&matTrans);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(m_iAlphaValue, 255, 255, 255)
		);


}

void CBlackScreen::Release(void)
{

}

void CBlackScreen::FadeIn()
{
	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime >= m_fFadeTime)
	{
		m_iAlphaValue -= (int)m_fAlphaSpeed;
		m_fTime = 0.f;
	}

// 	system("cls");
// 	cout << "m_fTime: "<< m_fTime		  << endl;
// 	cout << "m_iAlphaValue: "<< m_iAlphaValue << endl;
// 	cout << "m_fAlphaSpeed: " << m_fAlphaSpeed << endl;
// 	

	if(m_iAlphaValue <= 0)
	{
		m_iAlphaValue = 0;
		g_bIsFadeIn = false;
	}


}

void CBlackScreen::FadeOut()
{
	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime >= m_fFadeTime)
	{
		m_iAlphaValue += (int)m_fAlphaSpeed;
		m_fTime = 0.f;
	}


	if(m_iAlphaValue >= 255)
	{
		m_iAlphaValue = 255;
	}

}
