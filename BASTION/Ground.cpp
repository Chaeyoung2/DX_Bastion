#include "StdAfx.h"
#include "Ground.h"

CGround::CGround(void)
{
}

CGround::~CGround(void)
{
}

HRESULT CGround::Initialize(void)
{
	LoadData();


	m_wstrObjKey = L"BackGround";
	m_wstrStateKey = L"Ground";

	m_eRenderID = RENDER_GROUND;

	return S_OK;
}

eOBJECT_RESULT CGround::Update(void)
{
	
	return OR_OK;
}

void CGround::Render(void)
{


	list<PMYOBJ>::iterator iter = m_GroundList.begin();
	list<PMYOBJ>::iterator iter_end = m_GroundList.end();

	for(; iter != iter_end; ++iter)
	{
		const TEX_INFO* pTexture = m_pTextureMgr->GetTexture(
			m_wstrObjKey, m_wstrStateKey, (*iter)->iCount);

		float fPosX = float(pTexture->ImgInfo.Width * 0.5f);
		float fPosY = float(pTexture->ImgInfo.Height * 0.5f);

		MatrixCheck( iter, iter_end );

		m_pSprite->SetTransform( &((*iter)->matWorld) );

		m_pSprite->Draw( pTexture->pTexture,
			NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

// 
// 		cout << "Ground" << endl;
// 		cout << "(*iter)->vPos.y: " << (*iter)->vPos.y << endl << endl;

	}
}

void CGround::Release(void)
{
	list<PMYOBJ>::iterator iter = m_GroundList.begin();
	list<PMYOBJ>::iterator iter_end = m_GroundList.end();

	for(; iter != iter_end;)
	{
		Safe_Delete((*iter));
		iter = m_GroundList.erase(iter);
	}
}

void CGround::MatrixCheck( list<PMYOBJ>::iterator iter , list<PMYOBJ>::iterator iter_end)
{
	D3DXMATRIX matTrans;

	for(; iter != iter_end; ++iter)
	{
		D3DXMatrixTranslation(&matTrans,
			(*iter)->vPos.x + m_vScroll.x,
			(*iter)->vPos.y + m_vScroll.y,
			(*iter)->vPos.z);


		(*iter)->matWorld = matTrans;
	}
}

void CGround::LoadData()
{
	if(g_eScene == SCENE_STAGE1)
	{
		HANDLE hFile = CreateFile(
			L"../Data/GroundData.dat",
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
			m_GroundList.push_back(pObj);
		}

		CloseHandle(hFile);

	}

	if(g_eScene == SCENE_DIA)
	{
		HANDLE hFile = CreateFile(
			L"../Data/GroundData_DIA.dat",
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
			m_GroundList.push_back(pObj);
		}

		CloseHandle(hFile);
	}


	if(g_eScene == SCENE_STAGE2)
	{
		HANDLE hFile = CreateFile(
			L"../Data/GroundData_2.dat",
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
			m_GroundList.push_back(pObj);
		}

		CloseHandle(hFile);
	}
	
}
