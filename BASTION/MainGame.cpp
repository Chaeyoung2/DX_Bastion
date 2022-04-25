#include "StdAfx.h"
#include "MainGame.h"
#include "Device.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "AStar.h"
#include "SoundMgr.h"

eSceneType g_eScene = SCENE_LOGO;
bool g_bIsFadeOut = false;
bool g_bIsFadeIn = false;
bool g_bIsZoomIn = false;
bool g_bIsZoomOut = false;
bool g_bIsMenuToStage1 = false;
bool g_bIsUIRender = false;
bool g_bIsShaking = false;



CMainGame::CMainGame(void)
:m_pDevice(CDevice::GetInstance()), m_fTime(0.f), m_iCount(0)
, m_pScene(CSceneMgr::GetInstance()), m_pSoundMgr(CSoundMgr::GetInstance()),
m_pTimeMgr(CTimeMgr::GetInstance())
{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * MIN_STR);
	D3DXMatrixIdentity(&m_matFpsTrans);
}

CMainGame::~CMainGame(void)
{
	Release();
}

HRESULT CMainGame::Initialize()
{
	m_pTimeMgr->InitTime();

	if( FAILED(m_pDevice->Init3D() ))
		return E_FAIL;

	m_pScene->Initialize();
	m_pScene->SetChangeScene(SCENE_LOGO);


	return S_OK;
}

eOBJECT_RESULT CMainGame::Update()
{
	m_pTimeMgr->SetTime();
	m_pScene->Update();
	m_pSoundMgr->UpdateSound();

	return OR_OK;
}

void CMainGame::Render()
{
	FpsRender();

	m_pDevice->GetDevice()->Clear(0,	//버퍼의 개수
		NULL, //위치의 주소값
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : 후면버퍼, 
		D3DCOLOR_ARGB(255, 0, 0, 0),		//어떤색으로?
		1.f,	//삭제한후에 깊이버퍼의 초기값
		0);	//스텐실 버퍼의 초기값

	m_pDevice->Render_Begin();

	m_pScene->Render();

	m_pDevice->GetSprite()->SetTransform(&m_matFpsTrans);

	if(g_bIsUIRender == true)
	{
		m_pDevice->GetFont()->DrawTextW(
			m_pDevice->GetSprite(),
			m_szFPS,
			lstrlen(m_szFPS),
			NULL, NULL, D3DCOLOR_ARGB(255, 0, 255, 0));
	}

	m_pDevice->Render_End();

	//
	//


	//m_pDevice->GetDevice()->Present(NULL, NULL, g_hWnd, NULL);
	//m_pDevice->GetDevice()->Present(NULL, NULL, NULL, NULL);


}

void CMainGame::Release()
{
	m_pDevice->DestroyInstance();

	m_pScene->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CAStar::GetInstance()->DestroyInstance();
	m_pSoundMgr->DestroyInstance();
}

void CMainGame::FpsRender()
{
	D3DXMatrixTranslation(&m_matFpsTrans, 50.f, 50.f, 0.f);

	++m_iCount;

	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime > 1.f)
	{
		m_fTime = 0.f;
		wsprintf(m_szFPS, L"FPS : %d", m_iCount);
		m_iCount = 0;
	}

}
