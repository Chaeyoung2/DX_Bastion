#include "StdAfx.h"
#include "Scene.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "Device.h"


CScene::CScene(void)
:m_pTimeMgr(CTimeMgr::GetInstance()), m_pObjMgr(CObjMgr::GetInstance())
, m_pDevice(CDevice::GetInstance()), m_pTextureMgr(CTextureMgr::GetInstance()),
m_pSceneMgr(CSceneMgr::GetInstance())
{
}

CScene::~CScene(void)
{
}
