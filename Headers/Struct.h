#pragma once



typedef struct tagInfo 
{
	D3DXVECTOR3 vPos;		//위치벡터
	D3DXVECTOR3 vDir;		//방향벡터
	D3DXVECTOR3 vLook;		

	D3DXVECTOR3 vSize;		//객체 크기.

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
	LPDIRECT3DTEXTURE9	pTexture;	//이미지한장을 보관하기 위한 변수.(이미지의 실질적인 정보)
	D3DXIMAGE_INFO		ImgInfo;	//이미지의 정보(외부)를 담는다.
}TEX_INFO;

typedef struct tagObj
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXMATRIX	matWorld;

	RECT		tCollRect;

	int			iObjID; // 0 structure, 1 ground
	int			iCount; // 이미지 장수
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
	bool		bIsAdded; // Added list박스에 추가됐는지.

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
	std::wstring			wstrObjKey;			// 이미지 종류
	std::wstring			wstrStateKey;		// 이미지 상태
	std::wstring			wstrPath;			// 이미지 경로
	int						iCount;				// 이미지 개수

	tagTexturePath(void) 
		: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
	{

	}

}IMGPATH;


typedef struct tagItemPath
{
	std::wstring wstrObjKey;		//아이템 이름
	std::wstring wstrImagePath;		//아이템 경로

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
	float fOriFrame;	// 이미지의 초기 Index
	float fFrame;		//이미지의 Index
	float fCount;	//1초동안 몇프레임
	float fMaxFrame;

	tagFrame(void){}
	tagFrame(float _fOriFrame, float _fFrame, float _fCount, float _fMaxFrame)
		:fOriFrame(_fOriFrame), fFrame(_fFrame), fCount(_fCount), fMaxFrame(_fMaxFrame){}
}FRAME;


typedef struct tagAStarNode
{
	float			fCost;		//비용검사.
	int				iIndex;
	tagAStarNode*   pParent;
}NODE;


