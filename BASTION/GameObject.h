#pragma once

#include "Include.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

class CGameObject
{
protected:
	INFO		m_tInfo;
	wstring		m_wstrObjKey;
	float		m_fSpeed;
	eObjectDir	m_eDir;
	bool		m_bIsDead;
	RECT		m_tCollRect;
	eRenderID	m_eRenderID;
	CTimeMgr*	m_pTimeMgr;
	CTextureMgr* m_pTextureMgr;
	CObjMgr*		m_pObjMgr;
	CSoundMgr*	m_pSoundMgr;

	LPD3DXSPRITE	m_pSprite;


	static D3DXVECTOR3 m_vScroll;

	bool			m_bIsInWindow;


public:
	virtual HRESULT			Initialize(void)PURE;
	virtual eOBJECT_RESULT	Update(void)PURE;
	virtual void			Render(void)PURE;
	virtual void			Release(void)PURE;

public:
	const wstring& GetObjKey(void)
	{
		return m_wstrObjKey;
	}

	INFO* GetInfo(void)
	{
		return &m_tInfo;
	}
	eObjectDir GetDir(void)
	{
		return m_eDir;
	}


	RECT* GetCollRect(void)
	{
		return &m_tCollRect;
	}
	bool& GetDeadState()  
	{ 
		return m_bIsDead; 
	}
	eRenderID GetRenderID()
	{
		return m_eRenderID;
	}


public:
	void SetPos(const D3DXVECTOR3& vPos)
	{
		m_tInfo.vPos = vPos;
	}

	void SetPos(const float fX, const float fY)
	{
		m_tInfo.vPos.x = fX;
		m_tInfo.vPos.y = fY;
		m_tInfo.vPos.z = 0.f;
	}
	void SetDir(const D3DXVECTOR3& vDir)
	{
		m_tInfo.vDir = vDir;
	}
	void SetIsDead(bool bIsDead)
	{
		m_bIsDead = bIsDead;
	}
	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}
	


	// 컬링 처리
public:
	bool	IsInWindow();
	void	FocusOnGaspella();


	// 화면 흔들림
	bool	m_bIsShaking;

	bool	m_bIsShakingToRight;
	bool	m_bIsShakingToLeft;
	bool	m_bIsShakingToBottom;
	bool	m_bIsShakingToTop;

	bool	m_bIsSetCompletedOriginScrollValue;

	float	m_fShakingSpeed;
	float	m_fShakingTime;

	float	m_fOriginScrollX;
	float	m_fOriginScrollY;

	int		m_iShakingCount;
	int		m_iShakingMaxCount;


	void	IsShaking(float fX, float fY);

	// 포커즈 온
	bool	m_bIsFocusOnGaspella;
	bool	m_bIsFocusToEnd;
	bool	m_bIsFocusToOrigin;

	float	m_fFocusToEndAfterTime;

	// 자막11
	bool	 m_bIsMakeCompleteSub11;
	bool	m_bIsCountingEndToOrigin;




public:
	CGameObject(void);
	virtual ~CGameObject(void);
};
