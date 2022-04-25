#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr(void)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

HRESULT CTextureMgr::InsertTexture(const TCHAR* pFilePath, const TCHAR* pObjKey, eTextureType eType, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE(pObjKey));

	if(iter == m_mapTexture.end())
	{
		CTexture* pTex = NULL;

		switch(eType)
		{
			case TEX_SINGLE:
				pTex = new CSingleTexture;
				break;
			case TEX_MULTI:
				pTex = new CMultiTexture;
				break;
		}

		if(FAILED( pTex->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			MessageBox(g_hWnd, pFilePath, L"TextureMgr Load Error!", MB_OK);
			return E_FAIL;
		}
		TCHAR* pKey = new TCHAR[MAX_STR];
		lstrcpy(pKey, pObjKey);

		m_mapTexture.insert( make_pair(pKey, pTex) );
	}

	return S_OK;
}

const TEX_INFO* CTextureMgr::GetTexture(const TCHAR* pObj, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter;

 	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE(pObj) );
	if(iter == m_mapTexture.end()) return NULL;

	return iter->second->GetTexture(pStateKey, iCount);
}

void CTextureMgr::Release()
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), DeleteMap());
	m_mapTexture.clear();
}
