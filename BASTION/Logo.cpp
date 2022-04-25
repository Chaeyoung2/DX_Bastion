#include "StdAfx.h"
#include "Logo.h"
#include "MyMouse.h"
#include "Factory.h"
#include "SceneMgr.h"
#include "Device.h"

int			g_iImageCount = 0;

CLogo::CLogo(void)
:m_hThread(NULL), m_fBlackBarTransX(WINCX * 0.5f), 
m_iImageCount(0), m_fWingTransX(0.f), m_fWheelAngle(0.f)
, m_bIsLoadTextureComplete(false), m_fWheelTime(0.f)
{
	ZeroMemory(&m_Crt, sizeof(CRITICAL_SECTION));
	D3DXMatrixIdentity(&m_matBlackBarWorld);


}

CLogo::~CLogo(void)
{
	Release();
}

UINT __stdcall CLogo::ImageLoadFunc(void* pScene)
{
	CLogo* pLogo = (CLogo*)pScene;


	EnterCriticalSection( &pLogo->GetCrt() );


// 	//스테이지에 있는 리소스를 로드한다.
	if( FAILED(CTextureMgr::GetInstance()->LoadImagePath(L"../Data/MultiImagePath.txt" )) )
	{
		MessageBox(g_hWnd, L"이미지 패스 로드 실패!!", L"에러!", MB_OK);
		return E_FAIL;
	}




	LeaveCriticalSection(&pLogo->GetCrt());


// 	wstring strTemp = L"이미지 로드 완료!";
// 	m_pTextureMgr->SetCurrentPath(strTemp);
// 
// 	while(true)
// 	{
// 		//순회, 배회
// 
// 		if(GetAsyncKeyState('9'))
// 			break;
// 	}

	//쓰레드종료
	/*cout << g_iImageCount << endl;*/
	_endthreadex(0);

	return 0;
}

CRITICAL_SECTION CLogo::GetCrt(void)
{
	return m_Crt;
}

HRESULT CLogo::Initialize(void)
{
	g_eScene =  SCENE_LOGO;

	// 타일, 백그라운드
	if(FAILED(m_pTextureMgr->InsertTexture(L"../Texture/BackGround/Tile/Tile%d.png",
		L"TILE",
		TEX_MULTI,
		L"Tile",
		124)))
	{
		MessageBox(g_hWnd, L"멀티 텍스쳐 로드 실패", L"에러!", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pTextureMgr->InsertTexture(L"../Texture/BackGround/BackGround/BackGround%d.png",
		L"BACKGROUND",
		TEX_MULTI,
		L"BackGround",
		4)))
	{
		MessageBox(g_hWnd, L"멀티 텍스쳐 로드 실패", L"에러!", MB_OK);
		return E_FAIL;
	}

	// 마우스
	if(FAILED(m_pTextureMgr->InsertTexture(L"../Texture/UI/Mouse/Default/%d.png",
		L"MOUSE",
		TEX_MULTI,
		L"Mouse",
		4)))
	{
		MessageBox(g_hWnd, L"멀티 텍스쳐 로드 실패", L"에러!", MB_OK);
		return E_FAIL;
	}

	// 로딩바
	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/LoadingBar.png", L"LoadingBar", TEX_SINGLE);

	// 블랙바
	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/BlackBar.png", L"BlackBar", TEX_SINGLE);

	// 날개
	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Wing.png", L"Wing", TEX_SINGLE);

	// 톱니바퀴
	m_pTextureMgr->InsertTexture(
		L"../Texture/MainMenu/Wheel.png", L"Wheel", TEX_SINGLE);


	// 배경
	m_pTextureMgr->InsertTexture(
		L"../Texture/BackGround/BlackBack/BlackBack.png", L"BlackBack", TEX_SINGLE);

	// 마우스
	m_pObjMgr->AddObject(
		CFactory<CMyMouse>::CreateGameObject(), OBJ_MOUSE);

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		ImageLoadFunc,
		this,
		0,
		NULL );

	return S_OK;
}

eOBJECT_RESULT CLogo::Update(void)
{
	//CSceneMgr::GetInstance()->SetScene(SCENE_STAGE);
	//CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);

	if( GetAsyncKeyState(VK_RETURN) )
	{
		WaitForSingleObject(m_hThread, INFINITE);		//주 스레드의 흐름을 정지시킨다.
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);
		CSceneMgr::GetInstance()->SetChangeScene(SCENE_LOGO2);
	}

	// 쓰레드 종료
	if( g_iImageCount >= 64)
		m_bIsLoadTextureComplete = true;
	

	// 마우스 때문에
	m_pObjMgr->UpdateObj();

	//	int iA ; //끝나지 않은 기능.

	return OR_OK;
}

void CLogo::Render(void)
{
	D3DXMATRIX matTransBack, matTrans;

	// 블랙스크린
	const TEX_INFO*	pTextureBack = 
		m_pTextureMgr->GetTexture(L"BlackBack");

	if(NULL == pTextureBack)
		return;

	float fX = pTextureBack->ImgInfo.Width * 0.5f;
	float fY = pTextureBack->ImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTransBack, WINCX * 0.5f, WINCY * 0.5f, 0.f);

	m_pDevice->GetSprite()->SetTransform(&matTransBack);

	m_pDevice->GetSprite()->Draw(pTextureBack->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));
	
	// 로딩바
	const TEX_INFO* pTexture = 
		m_pTextureMgr->GetTexture(L"LoadingBar");

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;


	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY - fY * 2.f, 0);

	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));


	// 블랙바
	pTexture = 
		m_pTextureMgr->GetTexture(L"BlackBar");

	if(NULL == pTexture)
		return;

	float fBlackBarX = pTexture->ImgInfo.Width * 0.5f;
	float fBlackBarY = pTexture->ImgInfo.Height * 0.5f;


	BlackBarMatrixCheck(pTexture->ImgInfo.Width * 0.5f, pTexture->ImgInfo.Height * 0.5f);

	m_pDevice->GetSprite()->SetTransform(&m_matBlackBarWorld);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 날개
	pTexture = 
		m_pTextureMgr->GetTexture(L"Wing");

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;


	WingMatrixCheck( pTexture->ImgInfo.Width * 0.5f, 
		pTexture->ImgInfo.Height * 0.5f, fBlackBarX, fBlackBarY);

	m_pDevice->GetSprite()->SetTransform(&m_matWingWorld);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 톱니바퀴
	pTexture = 
		m_pTextureMgr->GetTexture(L"Wheel");

	if(NULL == pTexture)
		return;

	fX = pTexture->ImgInfo.Width * 0.5f;
	fY = pTexture->ImgInfo.Height * 0.5f;


	WheelMatrixCheck(fX, fY);

	m_pDevice->GetSprite()->SetTransform(&m_matWheelWorld);

	m_pDevice->GetSprite()->Draw(pTexture->pTexture, 
		NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
		NULL, // 출력할 이미지의 위치를 지정
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// OBJ 출력
	m_pObjMgr->RenderObj();

// 	CDevice::GetInstance()->GetFont()->DrawTextW(
// 		CDevice::GetInstance()->GetSprite(),
// 		m_pTextureMgr->GetCurrentPath().c_str(),
// 		lstrlen(m_pTextureMgr->GetCurrentPath().c_str()),
// 		NULL,
// 		NULL,
// 		D3DCOLOR_ARGB(255, 255, 255, 255)
// 		);
}

void CLogo::Release(void)
{

}

void CLogo::BlackBarMatrixCheck(float fX, float fY)
{
	D3DXMATRIX matTrans/*, matScaling*/;

	int iAllImageCount = 65;
	m_fBlackBarTransX = WINCX * 0.5f + (WINCX * 0.5f + fX - 150.f) * ( (float)g_iImageCount / (float)iAllImageCount );

	D3DXMatrixTranslation(&matTrans, m_fBlackBarTransX, WINCY - fY * 2.f, 0.f);
	/*D3DXMatrixScaling(&matScaling, m_fBlackBarScaleX, 1.f, 1.f);*/

	m_matBlackBarWorld = /*matScaling **/ matTrans;

}

void CLogo::WingMatrixCheck(float fX, float fY, float fBlackBarX,  float fBlackBarY)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, m_fBlackBarTransX - fBlackBarX - fX, WINCY - fBlackBarY * 2.5f, 0.f);

	m_matWingWorld = matTrans;
}

void CLogo::WheelMatrixCheck(float fX, float fY)
{
	m_fWheelTime += m_pTimeMgr->GetTime();

	if(m_bIsLoadTextureComplete == false)
	{
		if(m_fWheelTime >= 0.001f)
		{
			m_fWheelAngle += 0.005f;
			m_fWheelTime = 0.f;
		}
	}

	D3DXMATRIX matTrans, matRotZ;

	D3DXMatrixTranslation(&matTrans, WINCX * 0.065f, WINCY * 0.86f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fWheelAngle);

	m_matWheelWorld = matRotZ * matTrans;

}
