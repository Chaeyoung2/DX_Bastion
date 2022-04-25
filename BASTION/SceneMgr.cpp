#include "StdAfx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MyMenu.h"
#include "Stage1.h"
#include "Logo2.h"
#include "DiaPub.h"
#include "Stage2.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr(void)
{
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}

void CSceneMgr::Render()
{
	if(m_pScene)
		m_pScene->Render();
}

eOBJECT_RESULT CSceneMgr::Update()
{
	if(m_bChangeCheck)
	{
		m_bChangeCheck = false;
		SetScene(m_eSceneType);
	}

	if(m_pScene)
		m_pScene->Update();

	return OR_OK;
}

HRESULT CSceneMgr::Initialize()
{
	m_pScene = NULL;
	m_bChangeCheck = false;
	m_eSceneType = SCENE_END;

	return S_OK;
}

void CSceneMgr::SetScene(eSceneType eType)
{
	Safe_Delete(m_pScene);

	switch(eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;
	case SCENE_LOGO2:
		m_pScene = new CLogo2;
		break;
	case SCENE_MENU:
		m_pScene = new CMyMenu;
		break;
	case SCENE_STAGE1:
		m_pScene = new CStage1;
		break;
	case SCENE_STAGE2:
		m_pScene = new CStage2;
		break;
	case SCENE_DIA:
		m_pScene = new CDiaPub;
		break;
	}

	m_pScene->Initialize();
}

void CSceneMgr::SetChangeScene(eSceneType eType)
{
	m_eSceneType = eType;
	m_bChangeCheck = true;
}
