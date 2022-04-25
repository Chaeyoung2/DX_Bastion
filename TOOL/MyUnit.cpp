#include "StdAfx.h"
#include "MyUnit.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TOOLView.h"

CMyUnit::CMyUnit(void)
{
}

CMyUnit::~CMyUnit(void)
{
	Release();
}

HRESULT CMyUnit::Initialize()
{
	return S_OK;
}

int CMyUnit::Update()
{
	
	return 0;
}

void CMyUnit::Render()
{

	list<PMYUNIT>::iterator iter = m_listUnit.begin();
	list<PMYUNIT>::iterator iter_end = m_listUnit.end();


// 	//다이렉트 폰트를 사용하기 위한 임시 변수.
// 	TCHAR szIndex[MIDDLE_STR] = L"";
// 	int iIndex = 0;

	for(; iter != iter_end; ++iter)
	{
		
		TCHAR* szKey = new TCHAR[MAX_STR];
		TCHAR* szKeyPasted = L"";

		if( (*iter)->wstrName == L"Gaspella" )
			szKeyPasted = L"UnitGaspella";
		if( (*iter)->wstrName == L"Player" )
			szKeyPasted = L"UnitPlayer";
		if( (*iter)->wstrName == L"Defense" )
			szKeyPasted = L"UnitDefense";
		if( (*iter)->wstrName == L"Boss" )
			szKeyPasted = L"UnitBoss";
		
		
		lstrcpy(szKeyPasted, szKey);

		const TEX_INFO* pTexture
			= CTextureMgr::GetInstance()->GetTexture(
			szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);

		float fSizeX = pTexture->ImgInfo.Width * 0.5f;
		float fSizeY = pTexture->ImgInfo.Height * 0.5f;


		D3DXMATRIX matTrans;

		D3DXMatrixTranslation(&matTrans, 
			(*iter)->vPos.x - m_pMainView->GetScrollPos(0),
			(*iter)->vPos.y - m_pMainView->GetScrollPos(1),
			0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		// 선택됐을 때 알파 값 조정 위한 변수
		int iAlpha = 255;

		if(((*iter)->bIsClicked) == true) 
			iAlpha = int(255 * 0.5);
		else 
			iAlpha = 255;

		CDevice::GetInstance()->GetSprite()->Draw(
			pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fSizeX, fSizeY, 0.f),
			NULL,
			D3DCOLOR_ARGB(iAlpha, 255, 255, 255));

		//폰트출력
// 		wsprintf(szIndex, L"%d", iIndex);
// 
// 		CDevice::GetInstance()->GetFont()->DrawTextW(
// 			CDevice::GetInstance()->GetSprite(),
// 			szIndex,
// 			lstrlen(szIndex),
// 			NULL,
// 			NULL,
// 			D3DCOLOR_ARGB(255, 255, 0, 0)
// 			);
//		iIndex++;
	}
}



void CMyUnit::SetUnit(D3DXVECTOR3 vMousePos, int iUnitID, int iDir)
{
	PMYUNIT pMyUnit = new MYUNIT;
	pMyUnit->bIsClicked = false;
	pMyUnit->bIsAdded = false;
	pMyUnit->iDir = iDir;
	

	// 포스값을 발 밑으로 주기 위해서
	float fPosY = 0.f;

	switch(iDir)
	{
	case 0:
		pMyUnit->iCount = 0;
		break;
	case 1:
		pMyUnit->iCount = 1;
		break;
	case 2:
		pMyUnit->iCount = 2;
		break;
	case 3:
		pMyUnit->iCount = 3;
		break;
	case 4:
		pMyUnit->iCount = 4;
		break;
	case 5:
		pMyUnit->iCount = 5;
		break;
	case 6:
		pMyUnit->iCount = 6;
		break;
	case 7:
		pMyUnit->iCount = 7;
		break;
	}

	// 유닛 아이디에 따라서 포스값 조정 변수, 키값, 사이즈 설정
	switch(iUnitID)
	{
	case 0:
		{
			fPosY = PLAYERCY_ORI * 0.5f;
			pMyUnit->wstrName = L"Player";
			pMyUnit->vSize = D3DXVECTOR3((float)PLAYERCX, (float)PLAYERCY, 0.f);
		}
		break;
	case 1:
		{
			fPosY = GASCY * 0.5f;
			pMyUnit->wstrName = L"Gaspella";
			pMyUnit->vSize = D3DXVECTOR3((float)GASCX, (float)GASCY, 0.f);
		}
		break;
	case 2:
		{
			fPosY = DEFENSECY * 0.5f;
			pMyUnit->wstrName = L"Defense";
			pMyUnit->vSize = D3DXVECTOR3((float)DEFENSECX, (float)DEFENSECY, 0.f);
		}
		break;
	case 3:
		{
			fPosY = BOSSCY * 0.5f;
			pMyUnit->wstrName = L"Boss";
			pMyUnit->vSize = D3DXVECTOR3((float)BOSSCX, (float)BOSSCY, 0.f);
		}
		break;
	}
	pMyUnit->vPos = D3DXVECTOR3(vMousePos.x, vMousePos.y - fPosY * 0.5f, 0.f);
	

	m_listUnit.push_back(pMyUnit);
}

void CMyUnit::Release()
{
	list<PMYUNIT>::iterator iter = m_listUnit.begin();
	list<PMYUNIT>::iterator iter_end = m_listUnit.end();

	for(; iter != iter_end;)
	{
		Safe_Delete((*iter));
		iter = m_listUnit.erase(iter);
	}

}

void CMyUnit::MiniRender()
{

	list<PMYUNIT>::iterator iter = m_listUnit.begin();
	list<PMYUNIT>::iterator iter_end = m_listUnit.end();


	// 	//다이렉트 폰트를 사용하기 위한 임시 변수.
	// 	TCHAR szIndex[MIDDLE_STR] = L"";
	// 	int iIndex = 0;

	for(; iter != iter_end; ++iter)
	{

		TCHAR* szKey = new TCHAR[MAX_STR];
		TCHAR* szKeyPasted = L"";

		if( (*iter)->wstrName == L"Gaspella" )
			szKeyPasted = L"UnitGaspella";
		if( (*iter)->wstrName == L"Player" )
			szKeyPasted = L"UnitPlayer";
		if( (*iter)->wstrName == L"Defense" )
			szKeyPasted = L"UnitDefense";
		if( (*iter)->wstrName == L"Boss" )
			szKeyPasted = L"UnitBoss";


		lstrcpy(szKeyPasted, szKey);

		const TEX_INFO* pTexture
			= CTextureMgr::GetInstance()->GetTexture(
			szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);

		float fSizeX = pTexture->ImgInfo.Width * 0.5f;
		float fSizeY = pTexture->ImgInfo.Height * 0.5f;


		D3DXMATRIX matTrans, matScale, matWorld;

		D3DXMatrixScaling(&matScale, 0.25f, 0.25f, 1.f);

		D3DXMatrixTranslation(&matTrans, 
			(*iter)->vPos.x * 0.25f,
			(*iter)->vPos.y * 0.25f,
			0.f);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		// 선택됐을 때 알파 값 조정 위한 변수
		int iAlpha = 255;

		if(((*iter)->bIsClicked) == true) 
			iAlpha = int(255 * 0.5);
		else 
			iAlpha = 255;

		CDevice::GetInstance()->GetSprite()->Draw(
			pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fSizeX, fSizeY, 0.f),
			NULL,
			D3DCOLOR_ARGB(iAlpha, 255, 255, 255));
	}

}
