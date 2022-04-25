#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"
#include "Logo.h"

IMPLEMENT_SINGLETON(CTextureMgr)

HRESULT CTextureMgr::LoadImagePath(wstring wstrImagePath)
{
	wifstream			LoadFile;
	LoadFile.open(wstrImagePath.c_str(), ios::in);

	TCHAR		szObjKey[MIN_STR]		= L"";
	TCHAR		szStateKey[MIN_STR] 	= L"";
	TCHAR		szCount[MIN_STR]		= L"";
	TCHAR		szImgPath[MAX_PATH] 	= L"";

	wstring		wstrCombine = L"";


	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int		iCount = _ttoi(szCount);

		if( FAILED(InsertTexture(szImgPath, szObjKey, TEX_MULTI, szStateKey, iCount)) )
		{
			return E_FAIL;
		}

		

	}

	LoadFile.close();

	return S_OK;
}

HRESULT CTextureMgr::InsertTexture(const wstring& wstrFilePath, 
								   const wstring& wstrObjKey, 
								   eTextureType eeTextureType, 
								   const wstring& wstrStateKey /*= L""*/, 
								   const int& iCount /*= 0*/)
{
	MAPTEXTURE::iterator	iter  = m_mapTexture.find(wstrObjKey);

	m_wstrFullPath = wstrFilePath;

	if(iter == m_mapTexture.end())
	{
		CTexture*		pTexture = NULL;

		switch(eeTextureType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED(pTexture->InsertTexture(wstrFilePath, wstrStateKey, iCount)))
		{
			MSG_BOX(wstrObjKey.c_str());
			return E_FAIL;
		}

		m_mapTexture.insert(MAPTEXTURE::value_type(wstrObjKey, pTexture));
	}
	else
	{
		if(TEX_MULTI == eeTextureType)
		{
			iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCount);
		}
	}

	g_iImageCount++;
// 
// 	system("cls");
// 	cout << g_iImageCount << endl;

	return	S_OK;
}

const TEX_INFO* CTextureMgr::GetTexture(const wstring& wstrObjKey, 
										const wstring& wstrStateKey /*= L""*/, 
										const int& iCount /*= 0*/)
{
	MAPTEXTURE::iterator	iter  = m_mapTexture.find(wstrObjKey);

	if(iter == m_mapTexture.end())
		return NULL;

	return iter->second->GetTexture(wstrStateKey, iCount);
}

size_t CTextureMgr::GetImageCnt(const wstring& wstrObjKey, const wstring& wstrStateKey)
{
	MAPTEXTURE::iterator	iter = m_mapTexture.find(wstrObjKey);

	if(iter == m_mapTexture.end())
		return -1;

	return ((CMultiTexture*)iter->second)->GetImageCnt(wstrStateKey);
}


void CTextureMgr::Release()
{
	/*map<const TCHAR*, CTexture*>::iterator iter;
	map<const TCHAR*, CTexture*>::iterator iter_end;

	iter	 = m_mapTexture.begin();
	iter_end = m_mapTexture.end();
	
	for(iter; iter != iter_end; ++iter)
	{
		delete [] iter->first;
		Safe_Delete(iter->second);
	}*/

	for_each(m_mapTexture.begin(), m_mapTexture.end(), DeleteMap());
	m_mapTexture.clear();
}

CTextureMgr::CTextureMgr(void)
{
	
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}
