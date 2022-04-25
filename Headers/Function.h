#pragma once

class CStringCmp
{
public:
	CStringCmp(const TCHAR* pKey)
		: m_pKey(pKey) {}

public:
	template <typename T>
	bool operator()(T& dst)
	{
		return !_tcscmp(dst.first, m_pKey);
	}

private:
	const TCHAR*	m_pKey;
};

template<typename T>
static void Safe_Delete(T& rData)		//��۸� ������ ������ �ϱ� ���ؼ�.
{	
	if(rData)
	{
		delete rData;
		rData = NULL;
	}
}

template <typename T>
bool CompareY(T& dst, T& src)
{
	bool bResult = (dst->GetInfo()->vPos.y) < (src->GetInfo()->vPos.y);

	return bResult;
}



template <typename T>
bool CompareY_st(T& dst, T& src)
{
	bool bResult = (dst->vPos.y) < (src->vPos.y);

	return bResult;
}

// template <typename T>
// bool CompareY_Structure(T& dst)
// {
// 	list<PMYOBJ>* pStructurList = dst->GetStructureList();
// 
// 	list<PMYOBJ>::iterator iter = pStructurList->begin();
// 	list<PMYOBJ>::iterator iter_end = pStructurList->end();
// 
// 	pStructurList->sort(CompareY<PMYOBJ>);
// 
// 	bool bResult = (dst->vPos.y) < (src->vPos.y);
// 
// 	return bResult;
// }

template <typename T>
CGameObject* CreateEffect(eObjectDir eDir)
{
	CGameObject* pObj = CFactory<T>::CreateGameObject();

	dynamic_cast<CEffect*>(pObj)->SetObjecteDir(eDir);

	return pObj;
}

template <typename T>
CGameObject* CreateEffectBreak(eObjectDir eDir, D3DXVECTOR3 vPos)
{
	//CGameObject* pObj = CFactory<T>::CreateGameObject();
	CGameObject* pObj = new T;

	dynamic_cast<CEffect*>(pObj)->SetObjecteDir(eDir);
//	dynamic_cast<CEffectBreak*>(pObj)->SetBreakID(eID);
	pObj->SetPos(vPos);
	pObj->Initialize();

	return pObj;
}


template <typename T>
static  void Safe_Delete_Array(T& Pointer)
{
	if(NULL != Pointer)
	{
		delete [] Pointer;
		Pointer = NULL;
	}
}

static D3DXVECTOR3	GetMousePos(void)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3(float(pt.x),float(pt.y), 0.f);
}



//��θ� �������ִ� �Լ��� ����� ����.
inline std::wstring RelativePath(std::wstring wstrFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory( sizeof(szDirectoryPath), szDirectoryPath);
	//������Ʈ������ �ִ� ��ġ������ ��θ� ������ �Լ�.

	PathRelativePathTo((LPWSTR)szRelativePath,
						szDirectoryPath,
						FILE_ATTRIBUTE_DIRECTORY,
						wstrFullPath.c_str(),
						FILE_ATTRIBUTE_NORMAL);
	//�Ѱ�ο��� �ٸ���η� ���� ����θ� �������ִ� �Լ�.

	return szRelativePath;
}



