#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	TCHAR		szPath[MAX_PATH] = L"";

	vector<TEX_INFO*>		vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, wstrFilePath.c_str(), i);

		TEX_INFO*		pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->ImgInfo)))
		{
			MSG_BOX(szPath);
			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice->GetDevice(), 
			szPath, 
			pTexInfo->ImgInfo.Width, 
			pTexInfo->ImgInfo.Height, 
			pTexInfo->ImgInfo.MipLevels,
			0, 
			pTexInfo->ImgInfo.Format,
			D3DPOOL_MANAGED, 
			D3DX_DEFAULT,		// 테두리 표현을 어떻게 할 것인가
			D3DX_DEFAULT,		// 확대, 축소 했을 경우
			NULL,				// 제거할 이미지 색상 값
			&pTexInfo->ImgInfo,
			NULL,				// 팔레트(8비트 미만의 이미지) 처리
			&pTexInfo->pTexture)))
		{
			MSG_BOX(L"Multi Texture Image Load Failed");
			return E_FAIL;
		}

		vecTexture.push_back(pTexInfo);
	}

	m_MapMultiTexture.insert(MAPTEX_INFO::value_type(wstrStateKey, vecTexture));


	return S_OK;
}

const TEX_INFO* CMultiTexture::GetTexture(const wstring& wstrStateKey /*= L""*/, 
										  const int& iCount /*= 0*/)
{
	MAPTEX_INFO::iterator	iter = m_MapMultiTexture.find(wstrStateKey);

	if(iter == m_MapMultiTexture.end())
		return NULL;

	return iter->second[iCount];
}

void CMultiTexture::Release(void)
{
	MAPTEX_INFO::iterator iter		= m_MapMultiTexture.begin();
	MAPTEX_INFO::iterator iter_end	= m_MapMultiTexture.end();

	for(	;iter != iter_end; ++iter)
	{
		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();
			Safe_Delete(iter->second[i]);
		}
		iter->second.clear();
	}
	m_MapMultiTexture.clear();
}


size_t CMultiTexture::GetImageCnt(const wstring& wstrStateKey)
{
	MAPTEX_INFO::iterator		iter = m_MapMultiTexture.find(wstrStateKey);

	if(m_MapMultiTexture.end() == iter)
		return -1;

	return iter->second.size();
}
