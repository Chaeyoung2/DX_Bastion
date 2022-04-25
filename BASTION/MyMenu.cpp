#include "StdAfx.h"
#include "MyMenu.h"
#include "KeyMgr.h"
#include "Factory.h"
#include "BlackScreen.h"
#include "Bandibul.h"
#include "SceneMgr.h"
#include "Device.h"
#include "SubTitle.h"
#include "SoundMgr.h"

CMyMenu::CMyMenu(void)
:m_fMenuBoySpeed(30.f), m_fInitSpeed(300.f), m_bIsMenuBoyRisingComplete(false),
m_fMenuBoyTransValue(0.f),
m_bIsMenuBoyUp(true)
, m_bIsMouseActivatedAtMenu(false), m_iPressSpaceAlphaValue(255),
m_bIsNewGameActivated(false),
m_bIsExitActivated(false), m_bIsPressSpaceAlphaPlus(false), m_bIsNextScene(false),
m_fTime(0.f), m_fBandibulTime(0.f), m_bIsMakeCompleteSub0(false)
{
	D3DXMatrixIdentity(&m_matBoyWorld);

	ZeroMemory(&m_tExitRect, sizeof(RECT) );
	ZeroMemory(&m_tNewGameRect, sizeof(RECT) );

}

CMyMenu::~CMyMenu(void)
{
	Release();
}

HRESULT CMyMenu::Initialize(void)
{
	m_tExitFrame = FRAME(0.f, 0.f, 0.f, 2.f);
	m_tNewGameFrame = FRAME(0.f, 0.f, 0.f, 2.f);


	float fSpeed = 60.f;
	float fAngle = 0.f;


	m_pObjMgr->AddObject(
		CFactory<CBandibul>::CreateBandibul(
		WINCX * 0.3f, WINCY + 300.f, fAngle, fSpeed, 10.f), OBJ_BANDIBUL);

	m_pObjMgr->AddObject(
		CFactory<CBandibul>::CreateBandibul(
		WINCX * 0.6f, WINCY + 100.f, fAngle, fSpeed - 50.f, 100.f), OBJ_BANDIBUL);

	m_pObjMgr->AddObject(
		CFactory<CBandibul>::CreateBandibul(
		WINCX * 0.1f, WINCY + 500.f, fAngle, fSpeed + 100.f, 50.f), OBJ_BANDIBUL);


	g_eScene =  SCENE_MENU;


	// �� ��ũ��
	m_pObjMgr->AddObject(
		CFactory<CBlackScreen>::CreateGameObject(), OBJ_SCREEN);


// 	CTextureMgr::GetInstance()->InsertTexture(
// 		L"../Texture/MainMenu/Menu/MenuBack.png", L"MenuBack", TEX_SINGLE);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuBoy.png", L"MenuBoy", TEX_SINGLE);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuDeco.png", L"MenuDeco", TEX_SINGLE);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuLogo.png", L"MenuLogo", TEX_SINGLE);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuPan.png", L"MenuPan", TEX_SINGLE);


	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuFont/Exit/%d.png", L"EXIT", TEX_MULTI, L"Exit", 3);

	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Menu/MenuFont/NewGame/%d.png", L"NEWGAME", TEX_MULTI, L"NewGame", 3);

	return S_OK;
}

eOBJECT_RESULT CMyMenu::Update(void)
{
	
	m_fRenderTime += m_pTimeMgr->GetTime();


	AddBandibul();
	//PressSpaceAlphaBlending();

	// �����̽� �� ������ �Ѿ
// 	if( GetAsyncKeyState(VK_SPACE) )
// 	{		
// 		CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE1);
// 	}

	// ���콺 ������
	m_pObjMgr->UpdateObj();

	MenuButtonWithMouse();

	// �޴� ��ư ������ �ٲ��ֱ�
	if(m_bIsNewGameActivated == false)
	{
		m_tNewGameFrame.fFrame = 0.f;
	}
	else
	{
		m_tNewGameFrame.fFrame = 1.f;
	}
	if(m_bIsExitActivated == false)
	{
		m_tExitFrame.fFrame = 0.f;
	}
	else
	{
		m_tExitFrame.fFrame = 1.f;
	}


	// new game Ŭ�� �� 3�� �� ���� �� ��ȯ
	if(m_bIsNextScene == true)
	{
		m_tNewGameFrame.fFrame = 2.f;	
		g_bIsFadeOut = true;
		g_bIsFadeIn = false;

		m_fTime += m_pTimeMgr->GetTime();

		// ���� ������ �� �ڸ� ����
		if(m_fTime >= 3.f)
		{
			if(m_bIsMakeCompleteSub0 == false)
			{
				// �ڸ�
				if(FAILED(CTextureMgr::GetInstance()->InsertTexture(
					L"../Texture/UI/SubTitle/%d.png",
					L"SUBTITLE",
					TEX_MULTI,
					L"SubTitle",
					31)))
				{
					MessageBox(g_hWnd, L"��Ƽ �ؽ��� �ε� ����", L"����!", MB_OK);
				}


				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(0), OBJ_SCREEN);

				CSoundMgr::GetInstance()->StopSoundAll();
				CSoundMgr::GetInstance()->PlayBGM(L"FromWharftoWilds.mp3", CHANNEL_BGM, 0.7f);

				CSoundMgr::GetInstance()->PlaySound(L"Narrator01.MP3", CHANNEL_SUBTITLE01);

				m_bIsMakeCompleteSub0 = true;
			}
		}
		
		
		// ���� ������ �� �������� ��ȯ
		if(m_fTime >= 8.5f)
		{
			m_pSceneMgr->SetChangeScene(SCENE_STAGE1);
			
		}
	}

	return OR_OK;
}

void CMyMenu::Render(void)
{
	// ��׶���
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
		NULL, // ����� �̹����� ũ�⸦ ����, ��Ʈ�� ����
		&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� ���� ������ ����
		NULL, // ����� �̹����� ��ġ�� ����
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// �ҳ�
	pTexture = 
		m_pTextureMgr->GetTexture(L"MenuBoy");

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;

	MenuBoyMatrixCheck();

	m_pDevice->GetSprite()->SetTransform(&m_matBoyWorld);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // ����� �̹����� ũ�⸦ ����, ��Ʈ�� ����
		&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� ���� ������ ����
		NULL, // ����� �̹����� ��ġ�� ����
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// �޴���
	pTexture = 
		m_pTextureMgr->GetTexture(L"MenuPan");

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);

	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // ����� �̹����� ũ�⸦ ����, ��Ʈ�� ����
		&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� ���� ������ ����
		NULL, // ����� �̹����� ��ġ�� ����
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// �޴��� - exit
	pTexture = 
		m_pTextureMgr->GetTexture(L"EXIT", L"Exit", (int)m_tExitFrame.fFrame);

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, 442.f, 0.f);

	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // ����� �̹����� ũ�⸦ ����, ��Ʈ�� ����
		&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� ���� ������ ����
		NULL, // ����� �̹����� ��ġ�� ����
		D3DCOLOR_ARGB(255, 255, 255, 255));

	m_tExitRect.left = LONG(WINCX * 0.5f - fX);
	m_tExitRect.top = LONG(WINCY * 0.6f - fY);
	m_tExitRect.right = LONG(WINCX * 0.5f + fX);
	m_tExitRect.bottom = LONG(WINCY * 0.6f + fY);

	// �޴��� - new game
	pTexture = 
		m_pTextureMgr->GetTexture(L"NEWGAME", L"NewGame", 
		(int)m_tNewGameFrame.fFrame );

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, 352.f, 0.f);

	m_tNewGameRect.left = LONG(WINCX * 0.5f - fX);
	m_tNewGameRect.top = LONG(WINCY * 0.5f - fY);
	m_tNewGameRect.right = LONG(WINCX * 0.5f + fX);
	m_tNewGameRect.bottom = LONG(WINCY * 0.5f + fY);

	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // ����� �̹����� ũ�⸦ ����, ��Ʈ�� ����
		&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� ���� ������ ����
		NULL, // ����� �̹����� ��ġ�� ����

		D3DCOLOR_ARGB(255, 255, 255, 255));

	


	// press space

	//

// 	if(m_fRenderTime >= 1.f)
// 	{
// 		CDevice::GetInstance()->Render_End();
// 		CDevice::GetInstance()->Render_Begin();
// 		m_fRenderTime = 0.f;
// 	}
// 
// 	PressSpaceAlphaBlending();

// 	const TEX_INFO*  pTextureSpace = 
// 		CTextureMgr::GetInstance()->GetTexture(L"PressSpace");
// 
// 	if(NULL == pTextureSpace)
// 		return;
// 
// 	fX = pTextureSpace->ImgInfo.Width * 0.5f;
// 	fY = pTextureSpace->ImgInfo.Height * 0.5f;
// 
// 	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.9f, 0.f);
// 
// 	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
// 
// 	CDevice::GetInstance()->GetSprite()->Draw(pTextureSpace->pTexture, 
// 		NULL, // ����� �̹����� ũ�⸦ ����, ��Ʈ�� ����
// 		&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� ���� ������ ����
// 		NULL, // ����� �̹����� ��ġ�� ����
// 		D3DCOLOR_ARGB(m_iPressSpaceAlphaValue, 255, 255, 255));

// 	CDevice::GetInstance()->Render_End();
// 	CDevice::GetInstance()->Render_Begin();

	// ���콺
	m_pObjMgr->RenderObj();


	
}

void CMyMenu::Release(void)
{

}

void CMyMenu::MenuBoyMatrixCheck()
{
// 	system("cls");
// 	cout << "m_matBoyWorld._41: " << m_matBoyWorld._41 << endl;
// 		cout << "m_matBoyWorld._42: " << m_matBoyWorld._42 << endl;
	D3DXMATRIX matTrans;

	// ���� ��ǥ
	float fX = 160.f;
	float fY = WINCY * 0.5f;

	// �Դٰ��� �� �Ÿ�
	float fGapY = 30.f;

	// �ʱ� ����� ���� �ȵ����� ��� �����ְ�
	if(m_bIsMenuBoyRisingComplete == false)
	{
		m_fMenuBoyTransValue += m_fInitSpeed * m_pTimeMgr->GetTime();
	}

	// �ʱ� ����� �����ٸ�
	if(m_bIsMenuBoyRisingComplete == true)
	{
		// ������� �ƴ϶��
		if(m_bIsMenuBoyUp == false)
		{
			// trans���� --
			m_fMenuBoyTransValue -= m_fMenuBoySpeed * m_pTimeMgr->GetTime();
		}
		// ������̶��
		if(m_bIsMenuBoyUp == true)
		{
			// trans ���� ++
			m_fMenuBoyTransValue += m_fMenuBoySpeed * m_pTimeMgr->GetTime();
		}

		m_fInitSpeed = 0;
	}


	if(m_bIsNewGameActivated == true)
	{
		m_fMenuBoyY = 352.f;
		D3DXMatrixTranslation(&matTrans, fX, m_fMenuBoyY, 0.f );

	}
	if(m_bIsExitActivated == true)
	{
		m_fMenuBoyY = 442.f;
		D3DXMatrixTranslation(&matTrans, fX, m_fMenuBoyY, 0.f );
	}
	
	// ��� ����
	if(m_bIsNewGameActivated == false && m_bIsExitActivated == false)
	{
		D3DXMatrixTranslation(&matTrans, fX, WINCY * 1.5f - m_fMenuBoyTransValue, 0.f );
	}
	


	// �ʱ� ����� ������ ���� ���¿��� -> �ʱ� ��� ������ �� ��Ȳ ������ ��
	if(m_bIsMenuBoyRisingComplete == false)
	{
		if( matTrans._42 <= fY)
			m_bIsMenuBoyRisingComplete = true;
	}

	// �ʱ� ����� ���� ���� -> ���Ʒ� �Դٰ���
	else
	{
		if( matTrans._42 < fY - fGapY)
		{
			m_bIsMenuBoyUp = false;
		}
		else if( matTrans._42 > fY + fGapY)
		{
			m_bIsMenuBoyUp = true;
		}
	}
	

	// 160, 190 ���� �� �ø���

	m_matBoyWorld = matTrans;
}

void CMyMenu::PressSpaceAlphaBlending()
{
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
 }

void CMyMenu::MenuButtonWithMouse()
{
	

	D3DXVECTOR3 vMousePos = GetMousePos();

	POINT ptMouse; 
	ptMouse.x = (LONG)vMousePos.x;
	ptMouse.y = (LONG)vMousePos.y;

	if( PtInRect(&m_tExitRect, ptMouse) )
	{
		m_bIsExitActivated = true;
	}
	else
	{
		m_bIsExitActivated = false;
	}

	if( PtInRect(&m_tNewGameRect, ptMouse) )
	{
		m_bIsNewGameActivated = true;

		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON))
		{
			m_bIsNextScene = true;
		}
		
	}
	else
	{
		m_bIsNewGameActivated = false;
	}





}

void CMyMenu::AddBandibul()
{
	m_fBandibulTime += m_pTimeMgr->GetTime();

	if(m_fBandibulTime >= 5.f)
	{
		float fSpeed = 2.f;
		float fAngle = 0.f;
		float fDistance = 500.f;


		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);

		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.1f, (float)WINCY, fAngle, fSpeed * 0.5f, fDistance * 5.f), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.3f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.8f, (float)WINCY, fAngle, fSpeed * 3.f, fDistance * 3.f), OBJ_BANDIBUL);

		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance * 0.1f), OBJ_BANDIBUL);

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

		m_fBandibulTime = 0.f;
	}
}
