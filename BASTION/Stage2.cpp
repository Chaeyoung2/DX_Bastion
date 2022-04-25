#include "StdAfx.h"
#include "Stage2.h"
#include "Tile.h"
#include "Ground.h"
#include "MapObject.h"
#include "Factory.h"
#include "Device.h"
#include "Boss.h"
#include "Defense.h"
#include "Bandibul.h"

CStage2::CStage2(void)
:m_fTime(0.f)
{
}

CStage2::~CStage2(void)
{
	Release();
}

HRESULT CStage2::Initialize(void)
{
	g_eScene =  SCENE_STAGE2;

	
	CSoundMgr::GetInstance()->StopSoundAll();
	CSoundMgr::GetInstance()->PlayBGM(L"Stage1BGM.mp3", CHANNEL_BGM, 1.f);


	m_pObjMgr->AddObject( 
		CFactory<CSubTitle>::CreateSubTitle(22.f), OBJ_SCREEN);

	CSoundMgr::GetInstance()->PlaySound(L"Narrator22", CHANNEL_SUBTITLE22, 1.f);


	// 타일, 스트럭쳐, 그라운드
	m_pObjMgr->AddObject( 
		CFactory<CTile>::CreateGameObject(), OBJ_TILE);

	AddStructure();

	m_pObjMgr->AddObject( 
		CFactory<CGround>::CreateGameObject(), OBJ_GROUND);

	// 오브젝트
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(2351.f, 2388.f), OBJ_MONSTER);
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(3073.f, 2847.f), OBJ_MONSTER);
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(3432.f, 2250.f), OBJ_MONSTER);
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(4091.f, 2745.f), OBJ_MONSTER);

	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(2890.f, 1026.f), OBJ_MONSTER);
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(3669.f, 660.f), OBJ_MONSTER);
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(4451.f, 1488.f), OBJ_MONSTER);
	m_pObjMgr->AddObject( 
		CFactory<CDefense>::CreateGameObject(4752.f, 927.f), OBJ_MONSTER);



	m_pObjMgr->AddObject( 
		CFactory<CBoss>::CreateGameObject(947.f, 1179.f), OBJ_MONSTER);

	// 페이드인
	g_bIsFadeOut = false;
	g_bIsFadeIn = true;

	return S_OK;
}

eOBJECT_RESULT CStage2::Update(void)
{
	m_pObjMgr->UpdateObj();

	AddBandibul();

	return OR_OK;
}

void CStage2::Render(void)
{
	if(g_eScene != SCENE_DIA)
	{
		// 맵 렌더
		const TEX_INFO* pMapTexture
			= m_pTextureMgr->GetTexture(L"BackGround", L"BackGround", 0);
		float fMapX = pMapTexture->ImgInfo.Height * 0.5f;
		float fMapY = pMapTexture->ImgInfo.Width * 0.5f;

		D3DXMATRIX matMapTrans;

		D3DXMatrixTranslation(&matMapTrans, 
			fMapX /* + m_vScroll.x*/, 
			fMapY /*+ m_vScroll.y*/, 0.f);

		m_pDevice->GetSprite()->SetTransform(&matMapTrans);


		m_pDevice->GetSprite()->Draw(
			pMapTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fMapX, fMapY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	m_pObjMgr->RenderObj();



}

void CStage2::Release(void)
{
	m_pObjMgr->ReleaseObj();

}

void CStage2::AddStructure()
{
	// LoadData

	HANDLE hFile = CreateFile(
		L"../Data/StructureData_2.dat",
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

void CStage2::AddBandibul()
{
	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime >= 1.f)
	{
		float fSpeed = 2.f;
		float fAngle = 0.f;
		float fDistance = 500.f;


		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);

		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.1f, (float)WINCY, fAngle, fSpeed * 0.5f, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.3f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.8f, (float)WINCY, fAngle, fSpeed * 2.f, fDistance), OBJ_BANDIBUL);

		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance * 0.1f), OBJ_BANDIBUL);

		m_fTime = 0.f;
	}



}
