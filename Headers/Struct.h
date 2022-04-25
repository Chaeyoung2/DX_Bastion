#pragma once



typedef struct tagInfo 
{
	D3DXVECTOR3 vPos;		//��ġ����
	D3DXVECTOR3 vDir;		//���⺤��
	D3DXVECTOR3 vLook;		

	D3DXVECTOR3 vSize;		//��ü ũ��.

	D3DXMATRIX  matWorld;

	tagInfo(void){}

	tagInfo(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize)
		:vPos(_vPos)
		,vSize(_vSize)
	{}


}INFO, *PINFO, *LPINFO;

//POINT;
//INFO* pInfo;
//PINFO pInfo;

typedef struct tagTexture 
{
	LPDIRECT3DTEXTURE9	pTexture;	//�̹��������� �����ϱ� ���� ����.(�̹����� �������� ����)
	D3DXIMAGE_INFO		ImgInfo;	//�̹����� ����(�ܺ�)�� ��´�.
}TEX_INFO;

typedef struct tagObj
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXMATRIX	matWorld;

	RECT		tCollRect;

	int			iObjID; // 0 structure, 1 ground
	int			iCount; // �̹��� ���
	int			iRenderOption; // 0 world, 1 backWard, 2forward
	int			iStIndex;
	int			iGrIndex;
	float		fTransformValue;

	bool		bIsClicked;
	bool		bIsAdded;
	bool		bIsInvincible;
	bool		bIsDamaged;
	bool		bIsMakingEffect;
	bool		bIsTransform;
	bool		bIsAgreeToDamage;

	int			iCollCount;


	std::wstring		wstrName;

}MYOBJ, *PMYOBJ, *pMYOBJ;

typedef struct tagData
{
	int		iHp;
	int		iAtt;
	int		iXp;
	bool	bIsGun;
	bool	bIsHammer;

}DATA;

typedef struct tagUnit
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	int			iCount;
	int			iDir;

	bool		bIsClicked;
	bool		bIsAdded; // Added list�ڽ��� �߰��ƴ���.

	std::wstring		 wstrName;
	/*eObjectType eObjType;*/

}MYUNIT, *PMYUNIT, *pMYUNIT;


typedef struct tagTile 
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXVECTOR3 vOriPos;
	D3DXMATRIX  matWorld;

	BYTE	byOption;
	BYTE	byDrawId;

	float	fMoveSpeed;
	bool	bIsTransform;

}TILE, *PTILE, *pTILE;


typedef struct tagUnitData
{
	std::wstring wstrName;
	int		iHp;
	int		iAtt;
	int		iMp;
	int		iExp;
// 	BYTE	byJobIndex;
 	DWORD	dwItem;
}UNIT_DATA;


typedef struct tagStringCompare 
{
	template<class T>
	bool operator () (T& Data)
	{
		return !lstrcmp(Data.first, m_pString);
	}

	const TCHAR* m_pString;

	tagStringCompare(const TCHAR* pKey)
		:m_pString(pKey){}

}STRING_COMPARE;


//struct DeleteMap 
//{
//	template<class T>
//	void operator () (T& Data)
//	{
//		if(Data.second)
//		{
//			delete [] Data.first;
//			Safe_Delete(Data.second);
//		}
//	}
//};

class DeleteMap
{
public:
	template<typename T>
	void operator()(T& Temp)
	{
		if(Temp.second)
		{
			delete Temp.second;
			Temp.second =NULL;
		}
	}
};

class DeleteObj
{
public:
	template<typename T>
	void operator()(T& Temp)
	{
		if(Temp)
		{
			delete Temp;
			Temp =NULL;
		}
	}
};

typedef struct tagTexturePath
{
	std::wstring			wstrObjKey;			// �̹��� ����
	std::wstring			wstrStateKey;		// �̹��� ����
	std::wstring			wstrPath;			// �̹��� ���
	int						iCount;				// �̹��� ����

	tagTexturePath(void) 
		: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
	{

	}

}IMGPATH;


typedef struct tagItemPath
{
	std::wstring wstrObjKey;		//������ �̸�
	std::wstring wstrImagePath;		//������ ���

	tagItemPath(void)
		:wstrObjKey(L"")
		,wstrImagePath(L"")
	{;}

}ITEM_PATH;


typedef struct tagItem
{
	std::wstring wstrName;
	std::wstring wstrObjKey;
	std::wstring wstrImagePath;

	int iAtt;
	int iArmor;
	int iHpRegen;
	int iPrice;
	int iBitmapNum;
//	eItemType eType;
}ITEM_DATA, *PITEM_DATA;


typedef struct tagFrame
{
	float fOriFrame;	// �̹����� �ʱ� Index
	float fFrame;		//�̹����� Index
	float fCount;	//1�ʵ��� ��������
	float fMaxFrame;

	tagFrame(void){}
	tagFrame(float _fOriFrame, float _fFrame, float _fCount, float _fMaxFrame)
		:fOriFrame(_fOriFrame), fFrame(_fFrame), fCount(_fCount), fMaxFrame(_fMaxFrame){}
}FRAME;


typedef struct tagAStarNode
{
	float			fCost;		//���˻�.
	int				iIndex;
	tagAStarNode*   pParent;
}NODE;


