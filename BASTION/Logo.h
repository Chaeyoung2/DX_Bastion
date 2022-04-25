#pragma once
#include "Scene.h"

class CTimeMgr;
class CObjMgr;
class CTextureMgr;
class CDevice;
class CLogo : public CScene
{
private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;

private:
	static UINT __stdcall ImageLoadFunc( void* pScene);
	CRITICAL_SECTION	  GetCrt(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void					BlackBarMatrixCheck(float fX, float fY);
	void					WingMatrixCheck(float fX, float fY, float fBlackBarX, float fBlackBarY);
	void					WheelMatrixCheck(float fX, float fY);

public:
	CLogo(void);
	virtual ~CLogo(void);

private:
	D3DXMATRIX		m_matBlackBarWorld;
	float			m_fBlackBarTransX;

	D3DXMATRIX		m_matWingWorld;
	float			m_fWingTransX;

	D3DXMATRIX		m_matWheelWorld;
	float			m_fWheelAngle;

	bool			m_bIsLoadTextureComplete;

public:
	int				m_iImageCount;
	float			m_fWheelTime;

};

