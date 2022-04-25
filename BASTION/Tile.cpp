#include "StdAfx.h"
#include "Tile.h"
#include "Device.h"
#include "Player.h"
#include "Mouse.h"


CTile::CTile(void)
:m_pKeyMgr(CKeyMgr::GetInstance())
{
}

CTile::~CTile(void)
{
}

HRESULT CTile::Initialize(void)
{
	m_bIsTranslation = true;

	m_ptTileRender.x = WINCX / TILECX + 2;
	m_ptTileRender.y = static_cast<long>(WINCY / (TILECY * 0.5f)) + 3;


	LoadMapData();

	m_eRenderID = RENDER_BACK;

	m_fTransSpeed = 900.f;
	m_fScaleSpeed = 1.5f;
	m_fTrans = 0.f;
	m_fScaleX = 0.f;
	m_fScaleY = 0.f;

	m_bIsSettingScrollInChangingScene = false;

	if(g_eScene == SCENE_DIA && m_bIsSettingScrollInChangingScene == false)
	{
		m_vScroll.x = 0.f;
		m_vScroll.y = -(1980 - WINCY);
		m_bIsSettingScrollInChangingScene = true;
	}

	if(g_eScene == SCENE_STAGE2 && m_bIsSettingScrollInChangingScene == false)
	{
		m_vScroll.x = 0.f;
		m_vScroll.y = -(3135.f - WINCY);
		m_bIsSettingScrollInChangingScene = true;
	}

	return S_OK;
}

eOBJECT_RESULT CTile::Update(void)
{
	//	MouseScrollCheck();
	//	MatrixCheck();


	CalcDistanceToPlayer();

	return OR_OK;
}

void CTile::Render(void)
{

	float fTileX = 0.f;
	float fTileY = 0.f;

	if(g_eScene == SCENE_STAGE1)
	{
		fTileX = TILEX;
		fTileY = TILEY;
	}
	else if(g_eScene == SCENE_DIA)
	{
		fTileX = TILEXDIA;
		fTileY = TILEYDIA;
	}
	else if(g_eScene == SCENE_STAGE2)
	{
		fTileX = TILEX2;
		fTileY = TILEY2;
	}



	// 타일 렌더
	TCHAR szText[MIDDLE_STR] = L"";



	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = static_cast<int>(-m_vScroll.x / TILECX);
			int iCullY = static_cast<int>(-m_vScroll.y / (TILECY * 0.5f));

			int iIndex = static_cast<int>((i + iCullY) * fTileX + (j + iCullX));


			if(iIndex < 0 || iIndex >= fTileX * fTileY) continue;

			// 타일 텍스쳐
			const TEX_INFO* pTileTexture = m_pTextureMgr->GetTexture(
				L"BackGround", L"Tile", m_vecTile[iIndex]->byDrawId);

			// 옵션 텍스쳐
			const TEX_INFO* pOptionTexture
				= m_pTextureMgr->GetTexture(L"BackGround",
				L"TileOption", m_vecTile[iIndex]->byOption);

			// 윤곽선 텍스쳐
			const TEX_INFO* pBorderTexture
				= m_pTextureMgr->GetTexture(L"BackGround",
				L"TileBorder", 0);

			if(pTileTexture == NULL ||
				pOptionTexture == NULL || pBorderTexture == NULL)
				return;


			// 타일 텍스쳐의 중점
			float fPosX = pTileTexture->ImgInfo.Width * 0.5f;
			float fPosY = pTileTexture->ImgInfo.Height * 0.5f;

			// 타일 옵션 텍스쳐의 중점
			float fOptionPosX = pOptionTexture->ImgInfo.Width * 0.5f;
			float fOptionPosY = pOptionTexture->ImgInfo.Height * 0.5f;

			// 윤곽선 텍스쳐 중점
			float fBorderPosX = pBorderTexture->ImgInfo.Width * 0.5f;
			float fBorderPosY = pBorderTexture->ImgInfo.Height * 0.5f;

			// 행렬 적용
			MatrixCheck(iIndex);

			m_pSprite->SetTransform(&(m_vecTile[iIndex]->matWorld));

			m_pSprite->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(fPosX, fPosY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));


			if(m_pKeyMgr->StayKeyDown('1'))
			{
				//옵션출력
				m_pSprite->Draw(
					pOptionTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fOptionPosX, fOptionPosY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				// 윤곽선 출력
				m_pSprite->Draw(
					pBorderTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fBorderPosX, fBorderPosY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				//폰트출력
				RECT rcText;
				float fGap = -10.f;
				rcText.left = (LONG)fGap;
				rcText.top = (LONG)fGap;
				rcText.right = LONG(fPosX * 2.f + fGap);
				rcText.bottom =  LONG(fPosY * 2.f + fGap);

				wsprintf(szText, L"%d", iIndex);

				CDevice::GetInstance()->GetFont()->DrawTextW(
					m_pSprite,
					szText,
					lstrlen(szText),
					&rcText,
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255)
					);

			}


		}
	}

}

void CTile::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		Safe_Delete(m_vecTile[i]);
	}
	m_vecTile.clear();
}

void CTile::LoadMapData()
{
	DWORD dwByte = 0;

	if(g_eScene == SCENE_STAGE1)
	{
		HANDLE hFile = CreateFile(L"../Data/TileData.dat", GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		Release();


		while(1)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				Safe_Delete(pTile);
				break;
			}

			pTile->bIsTransform = false;

			m_vecTile.push_back(pTile);


			// 스케일 자료 담을 벡터 따로 주기
			// 옵션이 0이나 3이면 초기 스케일값을 0으로 (상승_확대)
			if(pTile->byOption == 0 || pTile->byOption == 3)
			{
				m_vecScaleX.push_back(0.f);
				m_vecScaleY.push_back(0.f);
				m_vecShaking.push_back(false);
// 				m_vecTransX
// 				m_vecTransY
			}
			// 옵션이 1이동가능이라면
			else if(pTile->byOption == 1 || pTile->byOption == 2)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// 옵션이 4면 초기 스케일값을 1으로 (하강_축소)
			else if(pTile->byOption == 4)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(true);
				// 				m_vecTransX
				// 				m_vecTransY
			}

		}

		CloseHandle(hFile);
	}

	else if(g_eScene == SCENE_DIA)
	{
		HANDLE hFile = CreateFile(L"../Data/TileData_DIA.dat", GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		Release();


		while(1)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				Safe_Delete(pTile);
				break;
			}

			pTile->bIsTransform = false;
			//pTile->vPos.y = WINCY;

			m_vecTile.push_back(pTile);

			// 스케일 자료 담을 벡터 따로 주기
			// 옵션이 0이나 3이면 초기 스케일값을 0으로 (상승_확대)
			if(pTile->byOption == 0 || pTile->byOption == 3)
			{
				m_vecScaleX.push_back(0.f);
				m_vecScaleY.push_back(0.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// 옵션이 1이동가능이라면
			else if(pTile->byOption == 1 || pTile->byOption == 2)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// 옵션이 4면 초기 스케일값을 1으로 (하강_축소)
			else if(pTile->byOption == 4)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(true);
				// 				m_vecTransX
				// 				m_vecTransY
			}
		}

		CloseHandle(hFile);
	}

	else if(g_eScene == SCENE_STAGE2)
	{
		HANDLE hFile = CreateFile(L"../Data/TileData_2.dat", GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		Release();


		while(1)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				Safe_Delete(pTile);
				break;
			}

			pTile->bIsTransform = false;
			//pTile->vPos.y = WINCY;

			m_vecTile.push_back(pTile);

			// 스케일 자료 담을 벡터 따로 주기
			// 옵션이 0이나 3이면 초기 스케일값을 0으로 (상승_확대)
			if(pTile->byOption == 0 || pTile->byOption == 3)
			{
				m_vecScaleX.push_back(0.f);
				m_vecScaleY.push_back(0.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// 옵션이 1이동가능이라면
			else if(pTile->byOption == 1 || pTile->byOption == 2)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// 옵션이 4면 초기 스케일값을 1으로 (하강_축소)
			else if(pTile->byOption == 4)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(true);
				// 				m_vecTransX
				// 				m_vecTransY
			}
		}

		CloseHandle(hFile);
	}

	else if(g_eScene == SCENE_STAGE3)
	{
		HANDLE hFile = CreateFile(L"../Data/TileData3.dat", GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		Release();


		while(1)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				Safe_Delete(pTile);
				break;
			}

			pTile->bIsTransform = false;
			//pTile->vPos.y = WINCY;

			m_vecTile.push_back(pTile);
		}

		CloseHandle(hFile);
	}
	

}

void CTile::MouseScrollCheck()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	float fScrollSpeed = 500;


	if(vMousePos.x < 0)	
		//m_vScroll.x += fScrollSpeed;
		m_vScroll.x += m_pTimeMgr->GetTime() * fScrollSpeed;
	else if(vMousePos.x > WINCX - 100)
		m_vScroll.x -= m_pTimeMgr->GetTime() * fScrollSpeed;
	else if(vMousePos.y < 0)
		m_vScroll.y += m_pTimeMgr->GetTime() * fScrollSpeed;
	else if(vMousePos.y > WINCY - 100)
		m_vScroll.y -= m_pTimeMgr->GetTime() * fScrollSpeed;
}
// 스크롤 만지다가 뭔가 이상해서 만들어본 함수
// void CTile::MatrixCheck()
// {
// 	D3DXMATRIX matTrans, matScale;
// 
// 	int iEndIndex = 0;
// 
// 	for(int i = 0; i < TILEY; ++i)
// 	{
// 		for(int j = 0; j < TILEY; ++j)
// 		{
// 
// 			int iIndex = (i) * TILEX + (j); // 화면상에 존재하는 좌표만
// 
// 			if(iIndex < 0 || iIndex >= TILEX * TILEY) break; // 인덱스를 벗어나면 break
// 
// 				D3DXMatrixTranslation(&matTrans, 
// 					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
// 					m_vecTile[iIndex]->vPos.y + m_vScroll.y,
// 					m_vecTile[iIndex]->vPos.z);
// 
// 
// 
// 
// 
// 			// 행렬 적용
// 			m_vecTile[iIndex]->matWorld = matTrans;
// 		}
// 	}
// 
// }


void CTile::MatrixCheck(int iIndex)
{
	D3DXMATRIX matTrans, matScale;



	float fY = WINCY * 0.8f; // 상승 타일일 때 초기 Y값.

	//// Translation 조건

	// 1) drawID가 123이 아니어야 되며
	if(m_vecTile[iIndex]->byDrawId != 123)	
	{
		// 2) 옵션이 3상승이거나 0이동불가라면 **상승**
		if(m_vecTile[iIndex]->byOption == 3 || m_vecTile[iIndex]->byOption == 0)
		{
			// 3) 플레이어와의 일정거리가 되었다면
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				// 4)  실제 좌표.y가 이동행렬.y 값 보다 아래에 있을 때 상승 시켜준다.
				if( m_vecTile[iIndex]->vPos.y - (-m_vScroll.y)
					< ((m_vecTile[iIndex]->matWorld._42) )  
					)
				{
					 // 해당 인덱스 타일만의 스피드를 갖게 해줬음
					m_vecTile[iIndex]->fMoveSpeed += m_fTransSpeed * m_pTimeMgr->GetTime();
				}

				// 이동 값 적용.
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y + fY - m_vecTile[iIndex]->fMoveSpeed,
					m_vecTile[iIndex]->vPos.z);


			}

			// 3) 플레이어와 일정거리가 되지 않았다면 아직 아래에 있어야.
			else
			{
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y + fY,
					m_vecTile[iIndex]->vPos.z);
			}
		}

		// 2-1) 옵션이 4하강이라면 **하강**
		else if(m_vecTile[iIndex]->byOption == 4)
		{
			// 3-1) 플레이어와 일정거리가 되었으면
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				// 4-1)  이동행렬 y값이 실제 좌표보다 작을 때만 translation 해준다.
// 				if( m_vecTile[iIndex]->vPos.y - (-m_vScroll.y)
// 					< ((m_vecTile[iIndex]->matWorld._42) ) 
// 					)
// 				{
					m_vecTile[iIndex]->fMoveSpeed -= m_fTransSpeed * m_pTimeMgr->GetTime(); // 해당 인덱스 타일만의 스피드를 갖게 한다.
/*				}*/

				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y - m_vecTile[iIndex]->fMoveSpeed,
					m_vecTile[iIndex]->vPos.z);
			}

			// 3-1) 플레이어와 일정거리가 되지 않았으면 아직 원래 좌표여야
			else
			{
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y ,
					m_vecTile[iIndex]->vPos.z);
			}
		}
		// 옵션이 4하강이 아니면 상승시켜줄 필요 없음.
		else
		{
			D3DXMatrixTranslation(&matTrans, 
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);
		}	
	}

	// 1) 타일 draw ID가 123이라면 연산해 줄 필요 없음.
	else
	{
		D3DXMatrixTranslation(&matTrans, 
			m_vecTile[iIndex]->vPos.x + m_vScroll.x,
			m_vecTile[iIndex]->vPos.y + m_vScroll.y,
			m_vecTile[iIndex]->vPos.z);
	}





	//// Scaling 조건

	// 1)  DrawID가 123(빈타일)이 아니여야 하고
	if(m_vecTile[iIndex]->byDrawId != 123)
	{
		// 2) 옵션값이 3상승이나 0이동불가라면 확대
		if(m_vecTile[iIndex]->byOption == 3 || m_vecTile[iIndex]->byOption == 0)
		{
			// 3) 플레이어와 거리를 연산했을 때 Transform이 허용된 상태여야.
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				// 2) 크기 행렬의 x값과 y값이 TILECX와 TILECY보다 작아야 한다.
				if(m_vecTile[iIndex]->matWorld._11 <= TILECX
					&& m_vecTile[iIndex]->matWorld._22 <= TILECY)
				{
					if(m_vecScaleX[iIndex] < 1.f)
						m_vecScaleX[iIndex] += m_fScaleSpeed * m_pTimeMgr->GetTime();
					else if(m_vecScaleX[iIndex] >= 1.f)
						m_vecScaleX[iIndex] = 1.f;

					if(m_vecScaleY[iIndex] < 1.f)
						m_vecScaleY[iIndex] += m_fScaleSpeed * m_pTimeMgr->GetTime();
					else if(m_vecScaleY[iIndex] >= 1.f)
						m_vecScaleY[iIndex] = 1.f;

					D3DXMatrixScaling(&matScale, m_vecScaleX[iIndex], m_vecScaleY[iIndex], 1.f);
				}

				// 2) 크기행렬.x값이 TILECX보다 작고 && 크기행렬.y값이 TILECY보다 작은게 아니라면 스케일 값이 1.f인 상태로
				else
				{
					D3DXMatrixScaling(&matScale, m_vecScaleX[iIndex],  m_vecScaleY[iIndex], 1.f);
				}
			}

			// 3) 플레이어와 거리 연산 후에도 transform이 비허용된 상태라면 아직 크기가 0이어야 함
			else
			{
				D3DXMatrixScaling(&matScale, 0.f, 0.f, 1.f);
			}
		}

		// 2) 옵션값이 4하강이라면 축소
		else if(m_vecTile[iIndex]->byOption == 4)
		{
			// 3) 플레이어와 거리를 연산했을 때 Transform이 허용된 상태라면 작아져야 함
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				if(m_vecScaleX[iIndex] > 0.f)
					m_vecScaleX[iIndex] -= m_fScaleSpeed * m_pTimeMgr->GetTime();
				else if(m_vecScaleX[iIndex] <= 0.f)
					m_vecScaleX[iIndex] = 0.f;

				if(m_vecScaleY[iIndex] > 0.f)
					m_vecScaleY[iIndex] -= m_fScaleSpeed * m_pTimeMgr->GetTime();
				else if(m_vecScaleY[iIndex] <= 0.f)
					m_vecScaleY[iIndex] = 0.f;

				D3DXMatrixScaling(&matScale, m_vecScaleX[iIndex], m_vecScaleY[iIndex], 1.f);
			}

			// 3) 플레이어와 거리 연산 후에도 transform이 비허용된 상태라면 아직 크기가 1이어야 함
			else
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			}
		}

		// 2) 옵션 값이 3상승이나 4하강이나 0이동불가가 아니라면 원래상태로
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		}
	}

	// 1) DrawID가 123이라면
	else
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	}



// 	if(iIndex == 907)
// 	{
// 		system("cls");
// 		cout << "m_vecTile[iIndex]->bIsTransform: " << m_vecTile[iIndex]->bIsTransform << endl;
// 		cout << "m_vecTile[iIndex]->matWorld._11: " << m_vecTile[iIndex]->matWorld._11  << endl;
// 		cout << "m_vecTile[iIndex]->matWorld._22: " << m_vecTile[iIndex]->matWorld._22  << endl;
// 		cout << "m_fScaleX :" << m_fScaleX << endl;
// 		cout << "m_fScaleY :" << m_fScaleY << endl;
// 	}

	// 행렬 적용
	m_vecTile[iIndex]->matWorld = matScale * matTrans;

	//	m_fTrans += m_fTransSpeed;


// 	if(iIndex == 3607)
// 	{
// 		system("cls");
// 		cout << "m_vecScaleX[3607]: "				 << m_vecScaleX[3607]				<< endl;
// 		cout << "m_vecScaleY[3607]: "				 << m_vecScaleY[3607]				<< endl;
// 		cout << "m_vecTile[iIndex]->matWorld._11: "  << m_vecTile[iIndex]->matWorld._11 << endl;
// 		cout << "m_vecTile[iIndex]->matWorld._22: "  << m_vecTile[iIndex]->matWorld._22 << endl;
// 	}


}

void CTile::CalcDistanceToPlayer()
{
	float fTileX = 0.f;
	float fTileY = 0.f;

	if(g_eScene == SCENE_STAGE1)
	{
		fTileX = TILEX;
		fTileY = TILEY;
	}
	else if(g_eScene == SCENE_DIA)
	{
		fTileX = TILEXDIA;
		fTileY = TILEYDIA;
	}
	else if(g_eScene == SCENE_STAGE2)
	{
		fTileX = TILEX2;
		fTileY = TILEY2;
	}



	 // 화면 내에 존재하는 타일만.
	CGameObject* pPlayer = m_pObjMgr->FindGameObject<CPlayer>();

	float fDist = 0.f; // 타일_플레이어 거리

	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = static_cast<int>(-m_vScroll.x / TILECX);
			int iCullY = static_cast<int>(-m_vScroll.y / (TILECY * 0.5f));

			int iIndex = static_cast<int>((i + iCullY) * fTileX + (j + iCullX));

			

			if(iIndex < 0 || iIndex >= fTileX * fTileY) continue; //인덱스 벗어나면 예외처리

			if(m_vecTile[iIndex]->byDrawId == 123
				|| m_vecTile[iIndex]->byOption == 1) continue; // 123번 타일, 이동 가능 타일이랑은 연산 안해!

			D3DXVECTOR3 vDist = m_vecTile[iIndex]->vPos - pPlayer->GetInfo()->vPos;

			float fVecLength = D3DXVec3Length(&vDist);

			//
// 			if(iIndex == 826)
// 			{
// 				system("cls");
// 				cout << "pPlayer->GetInfo()->vPos.x : " << pPlayer->GetInfo()->vPos.x << endl;
// 				cout << "pPlayer->GetInfo()->vPos.y : " << pPlayer->GetInfo()->vPos.y << endl;
// 				cout << "m_vecTile[iIndex]->vPos.x : " << m_vecTile[iIndex]->vPos.x << endl;
// 				cout << "m_vecTile[iIndex]->vPos.y : " << m_vecTile[iIndex]->vPos.y << endl;
// 				cout  << "vDist.x : "<< vDist.x				<< endl;
// 				cout  << "vDist.y : "			<< vDist.y				<< endl;
// 				cout  << "fVecLength : "		<< fVecLength			<< endl;
// 
// 			}
			//

// 			if(dynamic_cast<CPlayer*>(pPlayer)->GetState() != PLAYER_LIE)
// 			{
				// 상승 타일일 때
				if(m_vecTile[iIndex]->byOption == 0 || m_vecTile[iIndex]->byOption == 3)
				{
					fDist = 350.f;

					if ( fVecLength <= fDist )
					{
						m_vecTile[iIndex]->bIsTransform = true;
						//CSoundMgr::GetInstance()->PlaySound(L"ObjFalling1.MP3", CHANNEL_OBJFALLING);
					}
				}

				
				else if(m_vecTile[iIndex]->byOption == 4)
				{
					// 방3) 플레이어 moveIndex
 					int iMoveIndex = dynamic_cast<CPlayer*>(pPlayer)->GetMoveIndex();
					int iTransIndex = iMoveIndex;
					eObjectDir eDir = (pPlayer)->GetDir();

					switch(eDir)
					{
					case DIR_R: // 왼위, 왼, 왼아
						{
							iTransIndex = iMoveIndex - 1; // 왼
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ) - 1; // 왼위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ) - 1; // 왼아래
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_RB: // 위, 왼위, 왼
						{
							iTransIndex = iMoveIndex - TILEX * 2; // 위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - 1; // 왼
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ) - 1; // 왼위
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_RT:
						// 왼, 왼아래, 아래
						{
							iTransIndex = iMoveIndex + TILEX * 2; // 아래
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - 1; // 왼
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ) - 1; // 왼아래
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_T:
						// 왼아래, 아래, 오아래
						{
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ) - 1; // 왼아래
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + TILEX * 2; // 아래
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ); // 오아
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_B:
						// 왼위, 위, 오위
						{
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ) - 1; // 왼위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - TILEX * 2; // 위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ); // 오위
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_L:
						// 오위, 오, 오아
						{
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ); // 오위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + 1; // 오
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ); // 오아
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_LT:
						// 오, 오아, 아래
						{
							iTransIndex = iMoveIndex + 1; // 오
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ); // 오아
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + TILEX * 2; // 아래
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_LB:
						// 위, 오위, 오
						{
							iTransIndex = iMoveIndex - TILEX * 2; // 위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ); // 오위
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + 1; // 오
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					}


					
// 
// 					int iTransIndex = iMoveIndex;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex - 90;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex + 90;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex - 44;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex + 1;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex + 46;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 					
// 					
// 
// 					iTransIndex = iMoveIndex + 45;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex - 1;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
// 
// 					iTransIndex = iMoveIndex - 45;
// 					m_vecTile[iTransIndex]->bIsTransform = true;
 
// 					system("cls");
// 					cout << iMoveIndex << endl;

					// 방2) 렉트 충돌
// 					RECT rcTempRect;
// 					RECT rcTileRect;
// 					rcTileRect.left = m_vecTile[iIndex]->vPos.x - (TILECX * 0.5f);
// 					rcTileRect.right = m_vecTile[iIndex]->vPos.x + (TILECX * 0.5f);
// 					rcTileRect.top = m_vecTile[iIndex]->vPos.y - (TILECY * 0.5f);
// 					rcTileRect.bottom = m_vecTile[iIndex]->vPos.y + (TILECY * 0.5f);
// 
// 					float fWidth = 200.f;
// 					float fHeight = 200.f;
// 
// 					RECT rcPlayerRect;
// 					rcPlayerRect.left  = pPlayer->GetCollRect()->left - fWidth;
// 					rcPlayerRect.right = pPlayer->GetCollRect()->right + fWidth;
// 					rcPlayerRect.bottom = pPlayer->GetCollRect()->bottom + fWidth;
// 					rcPlayerRect.top  = pPlayer->GetCollRect()->top - fWidth;
// 
// 					if( IntersectRect(&rcTempRect, &rcTileRect, &rcPlayerRect))
// 					{
// 						m_vecTile[iIndex]->bIsTransform = true;
// 					}


					// 방1) 거리 --> 실패
// 					fDist = 200.f;
// 
// 					if( vDist.x <= 30.f && vDist.y >= 30.f && fVecLength <= fDist)
// 					{
// 						m_vecTile[iIndex]->bIsTransform = true;
// 					}
				}
				
/*			}*/
		}
	}
}