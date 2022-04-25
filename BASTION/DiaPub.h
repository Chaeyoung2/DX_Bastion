#pragma once
#include "scene.h"

class CTimeMgr;
class CObjMgr;
class CTextureMgr;
class CDevice;
class CDiaPub :
	public CScene
{
public:
	CDiaPub(void);
	virtual ~CDiaPub(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					AddStructure();
	void					AddBandibul();


	list<CGameObject*>		m_StructureList;

	float					m_fTime;
	float					m_fBandibulTime;

private:
	bool			m_bIsMakeCompleteSub17;
	bool			m_bIsMakeCompleteSub18;

};
