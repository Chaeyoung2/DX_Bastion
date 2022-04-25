#include "StdAfx.h"
#include "DiaPub.h"
#include "Factory.h"
// #include "BlackScreen.h"
// #include "Player.h"
#include "Tile.h"
#include "Structure.h"
#include "Ground.h"
#include "Defense.h"
// #include "HPBar.h"
// #include "XPBar.h"
// #include "IconGun.h"
// #include "IconHammer.h"
#include "SceneMgr.h"
#include "SubTitle.h"
#include "Gun.h"
#include "Portal.h"
#include "Bandibul.h"

CDiaPub::CDiaPub(void)
:m_fTime(0.f), m_bIsMakeCompleteSub17(false), m_bIsMakeCompleteSub18(false),
m_fBandibulTime(0.f)
{
}

CDiaPub::~CDiaPub(void)
{
	Release();
}

HRESULT CDiaPub::Initialize(void)
{
	CSoundMgr::GetInstance()->StopSoundAll();
	CSoundMgr::GetInstance()->PlayBGM(L"Stage2BGM.MP3", CHANNEL_BGM, 1.f);

	g_eScene =  SCENE_DIA;

	m_pObjMgr->AddObject( 
		CFactory<CTile>::CreateGameObject(), OBJ_TILE);

	m_pObjMgr->AddObject( 
		CFactory<CGround>::CreateGameObject(), OBJ_GROUND);

	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(831.f, 921.f), OBJ_MONSTER);

	m_pObjMgr->AddObject( 
		CFactory<CGun>::CreateGameObject(1277.f, 1243.f), OBJ_ITEM);

	m_pObjMgr->AddObject( 
		CFactory<CPortal>::CreateGameObject(2237.f, 1334.f), OBJ_PORTAL);

	AddStructure();

	g_bIsFadeOut = false;
	g_bIsFadeIn = true;
	

	


	return S_OK;
}

eOBJECT_RESULT CDiaPub::Update(void)
{
	m_pObjMgr->UpdateObj();

	AddBandibul();

	if( CKeyMgr::GetInstance()->OnceKeyUp(VK_F2))
	{
		if(m_bIsMakeCompleteSub17 == false)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(17.f), OBJ_SCREEN);
			CSoundMgr::GetInstance()->PlaySound(L"Narrator17f.MP3", CHANNEL_SUBTITLE18);


			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(1328, 1105, 58.f), OBJ_BOX);
			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(971, 1337, 59.f), OBJ_BOX);
			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(1392, 1376, 60.f), OBJ_BOX);


			m_bIsMakeCompleteSub17 = true;
		}
		
	}
	if( CKeyMgr::GetInstance()->OnceKeyUp(VK_F3))
	{
		if(m_bIsMakeCompleteSub18 == false)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(18.f), OBJ_SCREEN);

			CSoundMgr::GetInstance()->PlaySound(L"Narrator19.MP3", CHANNEL_SUBTITLE19);

			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(1165, 1163, 62.f), OBJ_BOX);
			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(1109, 1383, 60.f), OBJ_BOX);
			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(1431, 1352, 59.f), OBJ_BOX);
			m_pObjMgr->AddObject( 
				CFactory<CDBox>::CreateDBox(1376, 1089, 58.f), OBJ_BOX);

			m_bIsMakeCompleteSub18 = true;
		}
	}

	return OR_OK;
}

void CDiaPub::Render(void)
{
	m_pObjMgr->RenderObj();
}

void CDiaPub::Release(void)
{
	m_pObjMgr->ReleaseObj();
}

void CDiaPub::AddStructure()
{
	// LoadData

	HANDLE hFile = CreateFile(
		L"../Data/StructureData_DIA.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwByte = 0;

	while(true)
	{
		MYOBJ* pObj = new MYOBJ;

		ReadFile(hFile, pObj, sizeof(MYOBJ), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pObj);
			break;
		}

		// 		CGameObject* pMapObject = new CGameObject;
		// 		dynamic_cast<CMapObject*>(pMapObject)->SetObjInfo(pObj);

		m_pObjMgr->AddObject(
			CFactory<CMapObject>::CreateStructure( pObj ), OBJ_STRUCTURE);


		//m_StructureList.push_back(pMapObject);
	}

	CloseHandle(hFile);

}

void CDiaPub::AddBandibul()
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
			WINCX * 0.1f, (float)WINCY, fAngle, fSpeed * 0.5f, fDistance * 5), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.3f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.8f, (float)WINCY, fAngle, fSpeed * 3.f, fDistance * 3), OBJ_BANDIBUL);

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
