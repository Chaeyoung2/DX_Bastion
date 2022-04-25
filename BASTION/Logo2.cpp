#include "StdAfx.h"
#include "Logo2.h"
#include "Factory.h"
#include "Bandibul.h"
#include "SceneMgr.h"
#include "Device.h"
#include "SoundMgr.h"

CLogo2::CLogo2(void)
:m_fTransYValue(0.f), m_bIsFallingLogo(true), m_fTransYSpeed(0.1f), m_fTime(0.f)
/*, m_iPressSpaceAlphaValue(255), m_bIsPressSpaceAlphaPlus(false)*/
{
}

CLogo2::~CLogo2(void)
{
}

HRESULT CLogo2::Initialize(void)
{
	g_eScene =  SCENE_LOGO2;

	CSoundMgr::GetInstance()->PlayBGM(L"LogoBGM.mp3", CHANNEL_BGM, 1.f);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuBack.png", L"MenuBack", TEX_SINGLE);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuLogo.png", L"MenuLogo", TEX_SINGLE);


	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/PressSpace.png", L"PressSpace", TEX_SINGLE);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/PressSpace/%d.png", L"PRESSSPACE", TEX_MULTI,
		L"PressSpace", 41);

	m_tSpaceFrame = FRAME(0.f, 0.f, 40.f, 40.f);

	// 반딧불


	return S_OK;

}

eOBJECT_RESULT CLogo2::Update(void)
{
	AddBandibul();

	if( GetAsyncKeyState(VK_SPACE) )
	{
		m_pSceneMgr->SetChangeScene(SCENE_MENU);
	}

	PressSpaceFrameCheck();

	// 마우스 때문에
	m_pObjMgr->UpdateObj();

	return OR_OK;
}

void CLogo2::Render(void)
{
	// 백그라운드
	D3DXMATRIX matTrans;

	const TEX_INFO* pTexture = 
		m_pTextureMgr->GetTexture(L"MenuBack");

	if(NULL == pTexture)
		return;

	float fX = pTexture->ImgInfo.Width * 0.5f;
	float fY = pTexture->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);

	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 메뉴 로고
	pTexture = 
		m_pTextureMgr->GetTexture(L"MenuLogo");

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;

	MenuLogoMatrixCheck();

	m_pDevice->GetSprite()->SetTransform(&m_matMenuLogoWorld);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));



	// Press Space
	//PressSpaceAlphaBlending();

// 	const TEX_INFO*  pTextureSpace = 
// 		m_pTextureMgr->GetTexture(L"PressSpace");

	 	const TEX_INFO*  pTextureSpace = 
	 		m_pTextureMgr->GetTexture(L"PRESSSPACE", L"PressSpace",
			int(m_tSpaceFrame.fFrame));


	if(NULL == pTextureSpace)
		return;

	fX = pTextureSpace->ImgInfo.Width * 0.5f;
	fY = pTextureSpace->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.8f, 0.f);

	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->Draw(pTextureSpace->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));

// 	CDevice::GetInstance()->GetSprite()->Draw(pTextureSpace->pTexture, 
// 		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
// 		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
// 		NULL, // 출력할 이미지의 위치를 지정
// 		D3DCOLOR_ARGB(m_iPressSpaceAlphaValue, 255, 255, 255));

	// 마우스
	m_pObjMgr->RenderObj();

}

void CLogo2::Release(void)
{

}

void CLogo2::AddBandibul()
{
	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime >= 10.f)
	{
		float fSpeed = 5.f;
		float fAngle = 0.f;
		float fDistance = 100.f;


		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, WINCY + 300.f, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);

// 		m_pObjMgr->AddObject(
// 			CFactory<CBandibul>::CreateBandibul(
// 			WINCX * 0.6f, WINCY + 100.f, fAngle + 30.f, fSpeed, fDistance), OBJ_BANDIBUL);
// 
// 		m_pObjMgr->AddObject(
// 			CFactory<CBandibul>::CreateBandibul(
// 			WINCX * 0.1f, WINCY + 500.f, fAngle + 90.f, fSpeed, fDistance * 2.f), OBJ_BANDIBUL);
// 
// 		m_pObjMgr->AddObject(
// 			CFactory<CBandibul>::CreateBandibul(
// 			WINCX * 0.2f, WINCY, fAngle, fSpeed, fDistance * 50.f), OBJ_BANDIBUL);
// 
// 		m_pObjMgr->AddObject(
// 			CFactory<CBandibul>::CreateBandibul(
// 			WINCX * 0.8f, WINCY + 200.f, fAngle + 90.f, fSpeed, fDistance * 10.f), OBJ_BANDIBUL);
// 
// 		m_pObjMgr->AddObject(
// 			CFactory<CBandibul>::CreateBandibul(
// 			WINCX * 0.4f, WINCY + 300.f, fAngle + 120.f, fSpeed, fDistance * 4.f), OBJ_BANDIBUL);

		m_fTime = 0.f;
	}

}

void CLogo2::MenuLogoMatrixCheck()
{
	D3DXMATRIX matTrans;

	float fY = WINCY * 0.35f;

	if(m_bIsFallingLogo == true)
	{
		m_fTransYValue += m_fTransYSpeed;
	}


	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, m_fTransYValue, 0.f);

	m_matMenuLogoWorld = matTrans;

	if(m_matMenuLogoWorld._42 >= fY)
	{
		m_bIsFallingLogo = false;
	}

}

// void CLogo2::PressSpaceAlphaBlending()
// {
// 	float fSpeed = 0.0001f;
// 
// 	if(m_iPressSpaceAlphaValue >= 252)
// 	{
// 		m_bIsPressSpaceAlphaPlus = false;
// 	}
// 	if(m_iPressSpaceAlphaValue <= 125)
// 	{
// 		m_bIsPressSpaceAlphaPlus = true;
// 	}
// 
// 	if(m_bIsPressSpaceAlphaPlus == false)
// 	{
// 		m_iPressSpaceAlphaValue -= fSpeed * m_pTimeMgr->GetTime();
// 	}
// 	else
// 	{
// 		m_iPressSpaceAlphaValue += fSpeed * m_pTimeMgr->GetTime();
// 	}
// 
// }

void CLogo2::PressSpaceFrameCheck()
{
	m_tSpaceFrame.fFrame += m_tSpaceFrame.fCount * m_pTimeMgr->GetTime() * 0.5f;

	if(m_tSpaceFrame.fFrame >= m_tSpaceFrame.fMaxFrame)
	{
		m_tSpaceFrame.fFrame = m_tSpaceFrame.fOriFrame;
	}

}
