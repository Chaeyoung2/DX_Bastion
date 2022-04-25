#pragma once
#include "scene.h"

class CTimeMgr;
class CObjMgr;
class CTextureMgr;
class CDevice;
class CStage2 :
	public CScene
{
public:
	CStage2(void);
	virtual ~CStage2(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					AddStructure();
	void					AddBandibul();
	list<CGameObject*>		m_StructureList;

	float					m_fTime;

};
