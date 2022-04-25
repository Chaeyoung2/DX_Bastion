#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"


HRESULT CMultiTexture::InsertTexture(const TCHAR* pPath, 
									 const TCHAR* pStateKey /*= L""*/, 
									 const int& iCount /*= 0*/)
{
	TCHAR szPath[MAX_STR] = L"";

	vector<TEX_INFO*>		vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, pPath, i);

		TEX_INFO* pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->ImgInfo) ))
			return E_FAIL;

		if(FAILED( D3DXCreateTextureFromFileEx(
			CDevice::GetInstance()->GetDevice(),
			szPath,
			pTexInfo->ImgInfo.Width,
			pTexInfo->ImgInfo.Height,
			pTexInfo->ImgInfo.MipLevels,
			0,
			pTexInfo->ImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			&pTexInfo->ImgInfo,
			NULL,
			&pTexInfo->pTexture) ))
		{
			return E_FAIL;
		}

		vecTexture.push_back(pTexInfo);
	}

	TCHAR* pKey = new TCHAR[MAX_STR];
	lstrcpy(pKey, pStateKey);


	map_MultiTexture.insert( make_pair(pKey, vecTexture) );


	return S_OK;
}

const TEX_INFO* CMultiTexture::GetTexture(const TCHAR* pKey /*= L""*/, 
										  const int& iCount /*= 0*/)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter;

	iter = find_if(map_MultiTexture.begin(), map_MultiTexture.end(), STRING_COMPARE(pKey));

	if(iter == map_MultiTexture.end())
		return NULL;

	return iter->second[iCount];
}



void CMultiTexture::Release(void)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter = map_MultiTexture.begin();
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter_end = map_MultiTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete [] iter->first; 

		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();
			Safe_Delete(iter->second[i]);
		}
		iter->second.clear();
	}

	map_MultiTexture.clear();
}

CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}
