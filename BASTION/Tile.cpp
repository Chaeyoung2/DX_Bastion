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



	// Ÿ�� ����
	TCHAR szText[MIDDLE_STR] = L"";



	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = static_cast<int>(-m_vScroll.x / TILECX);
			int iCullY = static_cast<int>(-m_vScroll.y / (TILECY * 0.5f));

			int iIndex = static_cast<int>((i + iCullY) * fTileX + (j + iCullX));


			if(iIndex < 0 || iIndex >= fTileX * fTileY) continue;

			// Ÿ�� �ؽ���
			const TEX_INFO* pTileTexture = m_pTextureMgr->GetTexture(
				L"BackGround", L"Tile", m_vecTile[iIndex]->byDrawId);

			// �ɼ� �ؽ���
			const TEX_INFO* pOptionTexture
				= m_pTextureMgr->GetTexture(L"BackGround",
				L"TileOption", m_vecTile[iIndex]->byOption);

			// ������ �ؽ���
			const TEX_INFO* pBorderTexture
				= m_pTextureMgr->GetTexture(L"BackGround",
				L"TileBorder", 0);

			if(pTileTexture == NULL ||
				pOptionTexture == NULL || pBorderTexture == NULL)
				return;


			// Ÿ�� �ؽ����� ����
			float fPosX = pTileTexture->ImgInfo.Width * 0.5f;
			float fPosY = pTileTexture->ImgInfo.Height * 0.5f;

			// Ÿ�� �ɼ� �ؽ����� ����
			float fOptionPosX = pOptionTexture->ImgInfo.Width * 0.5f;
			float fOptionPosY = pOptionTexture->ImgInfo.Height * 0.5f;

			// ������ �ؽ��� ����
			float fBorderPosX = pBorderTexture->ImgInfo.Width * 0.5f;
			float fBorderPosY = pBorderTexture->ImgInfo.Height * 0.5f;

			// ��� ����
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
				//�ɼ����
				m_pSprite->Draw(
					pOptionTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fOptionPosX, fOptionPosY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				// ������ ���
				m_pSprite->Draw(
					pBorderTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fBorderPosX, fBorderPosY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				//��Ʈ���
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


			// ������ �ڷ� ���� ���� ���� �ֱ�
			// �ɼ��� 0�̳� 3�̸� �ʱ� �����ϰ��� 0���� (���_Ȯ��)
			if(pTile->byOption == 0 || pTile->byOption == 3)
			{
				m_vecScaleX.push_back(0.f);
				m_vecScaleY.push_back(0.f);
				m_vecShaking.push_back(false);
// 				m_vecTransX
// 				m_vecTransY
			}
			// �ɼ��� 1�̵������̶��
			else if(pTile->byOption == 1 || pTile->byOption == 2)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// �ɼ��� 4�� �ʱ� �����ϰ��� 1���� (�ϰ�_���)
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

			// ������ �ڷ� ���� ���� ���� �ֱ�
			// �ɼ��� 0�̳� 3�̸� �ʱ� �����ϰ��� 0���� (���_Ȯ��)
			if(pTile->byOption == 0 || pTile->byOption == 3)
			{
				m_vecScaleX.push_back(0.f);
				m_vecScaleY.push_back(0.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// �ɼ��� 1�̵������̶��
			else if(pTile->byOption == 1 || pTile->byOption == 2)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// �ɼ��� 4�� �ʱ� �����ϰ��� 1���� (�ϰ�_���)
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

			// ������ �ڷ� ���� ���� ���� �ֱ�
			// �ɼ��� 0�̳� 3�̸� �ʱ� �����ϰ��� 0���� (���_Ȯ��)
			if(pTile->byOption == 0 || pTile->byOption == 3)
			{
				m_vecScaleX.push_back(0.f);
				m_vecScaleY.push_back(0.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// �ɼ��� 1�̵������̶��
			else if(pTile->byOption == 1 || pTile->byOption == 2)
			{
				m_vecScaleX.push_back(1.f);
				m_vecScaleY.push_back(1.f);
				m_vecShaking.push_back(false);
				// 				m_vecTransX
				// 				m_vecTransY
			}
			// �ɼ��� 4�� �ʱ� �����ϰ��� 1���� (�ϰ�_���)
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
// ��ũ�� �����ٰ� ���� �̻��ؼ� ���� �Լ�
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
// 			int iIndex = (i) * TILEX + (j); // ȭ��� �����ϴ� ��ǥ��
// 
// 			if(iIndex < 0 || iIndex >= TILEX * TILEY) break; // �ε����� ����� break
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
// 			// ��� ����
// 			m_vecTile[iIndex]->matWorld = matTrans;
// 		}
// 	}
// 
// }


void CTile::MatrixCheck(int iIndex)
{
	D3DXMATRIX matTrans, matScale;



	float fY = WINCY * 0.8f; // ��� Ÿ���� �� �ʱ� Y��.

	//// Translation ����

	// 1) drawID�� 123�� �ƴϾ�� �Ǹ�
	if(m_vecTile[iIndex]->byDrawId != 123)	
	{
		// 2) �ɼ��� 3����̰ų� 0�̵��Ұ���� **���**
		if(m_vecTile[iIndex]->byOption == 3 || m_vecTile[iIndex]->byOption == 0)
		{
			// 3) �÷��̾���� �����Ÿ��� �Ǿ��ٸ�
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				// 4)  ���� ��ǥ.y�� �̵����.y �� ���� �Ʒ��� ���� �� ��� �����ش�.
				if( m_vecTile[iIndex]->vPos.y - (-m_vScroll.y)
					< ((m_vecTile[iIndex]->matWorld._42) )  
					)
				{
					 // �ش� �ε��� Ÿ�ϸ��� ���ǵ带 ���� ������
					m_vecTile[iIndex]->fMoveSpeed += m_fTransSpeed * m_pTimeMgr->GetTime();
				}

				// �̵� �� ����.
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y + fY - m_vecTile[iIndex]->fMoveSpeed,
					m_vecTile[iIndex]->vPos.z);


			}

			// 3) �÷��̾�� �����Ÿ��� ���� �ʾҴٸ� ���� �Ʒ��� �־��.
			else
			{
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y + fY,
					m_vecTile[iIndex]->vPos.z);
			}
		}

		// 2-1) �ɼ��� 4�ϰ��̶�� **�ϰ�**
		else if(m_vecTile[iIndex]->byOption == 4)
		{
			// 3-1) �÷��̾�� �����Ÿ��� �Ǿ�����
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				// 4-1)  �̵���� y���� ���� ��ǥ���� ���� ���� translation ���ش�.
// 				if( m_vecTile[iIndex]->vPos.y - (-m_vScroll.y)
// 					< ((m_vecTile[iIndex]->matWorld._42) ) 
// 					)
// 				{
					m_vecTile[iIndex]->fMoveSpeed -= m_fTransSpeed * m_pTimeMgr->GetTime(); // �ش� �ε��� Ÿ�ϸ��� ���ǵ带 ���� �Ѵ�.
/*				}*/

				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y - m_vecTile[iIndex]->fMoveSpeed,
					m_vecTile[iIndex]->vPos.z);
			}

			// 3-1) �÷��̾�� �����Ÿ��� ���� �ʾ����� ���� ���� ��ǥ����
			else
			{
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x + m_vScroll.x,
					m_vecTile[iIndex]->vPos.y + m_vScroll.y ,
					m_vecTile[iIndex]->vPos.z);
			}
		}
		// �ɼ��� 4�ϰ��� �ƴϸ� ��½����� �ʿ� ����.
		else
		{
			D3DXMatrixTranslation(&matTrans, 
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);
		}	
	}

	// 1) Ÿ�� draw ID�� 123�̶�� ������ �� �ʿ� ����.
	else
	{
		D3DXMatrixTranslation(&matTrans, 
			m_vecTile[iIndex]->vPos.x + m_vScroll.x,
			m_vecTile[iIndex]->vPos.y + m_vScroll.y,
			m_vecTile[iIndex]->vPos.z);
	}





	//// Scaling ����

	// 1)  DrawID�� 123(��Ÿ��)�� �ƴϿ��� �ϰ�
	if(m_vecTile[iIndex]->byDrawId != 123)
	{
		// 2) �ɼǰ��� 3����̳� 0�̵��Ұ���� Ȯ��
		if(m_vecTile[iIndex]->byOption == 3 || m_vecTile[iIndex]->byOption == 0)
		{
			// 3) �÷��̾�� �Ÿ��� �������� �� Transform�� ���� ���¿���.
			if(m_vecTile[iIndex]->bIsTransform == true)
			{
				// 2) ũ�� ����� x���� y���� TILECX�� TILECY���� �۾ƾ� �Ѵ�.
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

				// 2) ũ�����.x���� TILECX���� �۰� && ũ�����.y���� TILECY���� ������ �ƴ϶�� ������ ���� 1.f�� ���·�
				else
				{
					D3DXMatrixScaling(&matScale, m_vecScaleX[iIndex],  m_vecScaleY[iIndex], 1.f);
				}
			}

			// 3) �÷��̾�� �Ÿ� ���� �Ŀ��� transform�� ������ ���¶�� ���� ũ�Ⱑ 0�̾�� ��
			else
			{
				D3DXMatrixScaling(&matScale, 0.f, 0.f, 1.f);
			}
		}

		// 2) �ɼǰ��� 4�ϰ��̶�� ���
		else if(m_vecTile[iIndex]->byOption == 4)
		{
			// 3) �÷��̾�� �Ÿ��� �������� �� Transform�� ���� ���¶�� �۾����� ��
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

			// 3) �÷��̾�� �Ÿ� ���� �Ŀ��� transform�� ������ ���¶�� ���� ũ�Ⱑ 1�̾�� ��
			else
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			}
		}

		// 2) �ɼ� ���� 3����̳� 4�ϰ��̳� 0�̵��Ұ��� �ƴ϶�� �������·�
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		}
	}

	// 1) DrawID�� 123�̶��
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

	// ��� ����
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



	 // ȭ�� ���� �����ϴ� Ÿ�ϸ�.
	CGameObject* pPlayer = m_pObjMgr->FindGameObject<CPlayer>();

	float fDist = 0.f; // Ÿ��_�÷��̾� �Ÿ�

	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = static_cast<int>(-m_vScroll.x / TILECX);
			int iCullY = static_cast<int>(-m_vScroll.y / (TILECY * 0.5f));

			int iIndex = static_cast<int>((i + iCullY) * fTileX + (j + iCullX));

			

			if(iIndex < 0 || iIndex >= fTileX * fTileY) continue; //�ε��� ����� ����ó��

			if(m_vecTile[iIndex]->byDrawId == 123
				|| m_vecTile[iIndex]->byOption == 1) continue; // 123�� Ÿ��, �̵� ���� Ÿ���̶��� ���� ����!

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
				// ��� Ÿ���� ��
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
					// ��3) �÷��̾� moveIndex
 					int iMoveIndex = dynamic_cast<CPlayer*>(pPlayer)->GetMoveIndex();
					int iTransIndex = iMoveIndex;
					eObjectDir eDir = (pPlayer)->GetDir();

					switch(eDir)
					{
					case DIR_R: // ����, ��, �޾�
						{
							iTransIndex = iMoveIndex - 1; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ) - 1; // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ) - 1; // �޾Ʒ�
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_RB: // ��, ����, ��
						{
							iTransIndex = iMoveIndex - TILEX * 2; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - 1; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ) - 1; // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_RT:
						// ��, �޾Ʒ�, �Ʒ�
						{
							iTransIndex = iMoveIndex + TILEX * 2; // �Ʒ�
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - 1; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ) - 1; // �޾Ʒ�
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_T:
						// �޾Ʒ�, �Ʒ�, ���Ʒ�
						{
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ) - 1; // �޾Ʒ�
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + TILEX * 2; // �Ʒ�
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ); // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_B:
						// ����, ��, ����
						{
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ) - 1; // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - TILEX * 2; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ); // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_L:
						// ����, ��, ����
						{
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ); // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + 1; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ); // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_LT:
						// ��, ����, �Ʒ�
						{
							iTransIndex = iMoveIndex + 1; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + (TILEX + (iMoveIndex / TILEX) % 2 ); // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + TILEX * 2; // �Ʒ�
							m_vecTile[iTransIndex]->bIsTransform = true;
						}
						break;
					case DIR_LB:
						// ��, ����, ��
						{
							iTransIndex = iMoveIndex - TILEX * 2; // ��
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex - (TILEX - (iMoveIndex / TILEX) % 2 ); // ����
							m_vecTile[iTransIndex]->bIsTransform = true;
							iTransIndex = iMoveIndex + 1; // ��
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

					// ��2) ��Ʈ �浹
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


					// ��1) �Ÿ� --> ����
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