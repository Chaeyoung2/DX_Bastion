#pragma once
#include "Scene.h"

class CTimeMgr;
class CObjMgr;
class CTextureMgr;
class CDevice;
class CStage1 : public CScene
{
public:
	CStage1(void);
	virtual ~CStage1(void);

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
