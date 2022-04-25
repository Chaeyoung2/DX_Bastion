#pragma once
#include "GameObject.h"

#include "Effect.h"
#include "KeyMgr.h"
#include "Astar.h"

class CSubTitle;
class CPlayer :
	public CGameObject
{
public:
	CPlayer(void);
	virtual ~CPlayer(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void					FrameCheck(void);
	void					MatrixCheck(void);
	void					KeyCheck(void);
	void					AStarMove(void);
	void					Scroll();

	void					FrameSettingInState();
	//void DirSettingInMoving(D3DXVECTOR3 vTilePos);
	void					DirSettingInMoving(D3DXVECTOR3 vDir); // 에이스타에 따른 방향 전환
	void					DirSettingInAttack(D3DXVECTOR3 vDir);

	// 오브젝트 공통
	void					SettingCollRect();

	// 해머 충돌 박스
	void					SettingHammerRect();

	// 해머 충돌시 이펙트 생성
//	void MakingHammerEffect();

	// Get, Set
public:
	CGameObject* GetPlayer() 
	{
		return this; 
	}
	bool		 GetInvincible()
	{
		return m_bIsInvincible;
	}
	const DATA&  GetData()
	{
		return m_tData;
	}
	RECT*		 GetHammerRect()
	{
		return &m_tHammerRect;
	}
	ePlayerState GetState()
	{
		return m_eCurState;
	}
	bool		 GetReadyToPortal()
	{
		return m_bIsGetReadyToPortal;
	}
// 	bool		 GetIsHammerColl()
// 	{
// 		return  m_bIsHammerColl;
// 	}
	int			GetMoveIndex()
	{
		return m_iMoveIndex;
	}
	DATA&  GetRealData()
	{
		return m_tData;
	}

public:
	void		 SetDamage(int iAtt)
	{
		m_tData.iHp -= iAtt;
	}
	void		 SetExp(int iExp)
	{
		m_tData.iXp += iExp;
	}


	void		 SetInvincible(bool bIsInvincible)
	{
		m_bIsInvincible = bIsInvincible;
	}
	// 플레이어 state 변환
	void		 SetState(ePlayerState eState) 
	{ 
		m_eCurState = eState; 
	}
// 	void		 SetIsHammerColl(bool bIsHammerColl)
// 	{
// 		m_bIsHammerColl = bIsHammerColl;
// 	}
	void		SetReadyToPortal(bool bIsReadyToPortal)
	{
		m_bIsGetReadyToPortal = bIsReadyToPortal;
	}
	void SetIsDamaged(bool bIsDamaged)
	{
		m_bIsDamaged = bIsDamaged;
	}


	// 피격 당할 시 색깔 입히기

	int				m_iColor;
	bool			m_bIsDamaged;

	float			m_fDamageTime;



	//// 이펙트
	// 프레임 장수에 따라서 이펙트 만들기 위함..
	void MakeEffectInFrame();

private:
	FRAME			m_tFrame;
//	UNIT_DATA		m_tData;
	DATA			m_tData;
	RECT			m_tHammerRect;

	ePlayerState	m_ePreState;
	ePlayerState	m_eCurState;

	bool			m_bIsStateChanged;

	float			m_fScaleX;
	float			m_fScaleY;
	float			m_fScaleSpeed;

	float			m_fTime;

	int				m_iMoveIndex;


	wstring			m_wstrStateKey;

	bool			m_bIsNoDirection; // 방향 없을 때 스프라이트 돌려주기 위함
	bool			m_bIsInvincible;  // 무적 상태

//	bool			m_bIsHammerColl;  // 해머가 충돌 상태인지

	
	float			m_fOffSetX; // 스크롤X 오프셋.
	float			m_fOffSetY; // 스크롤Y 오프셋.




	// 객체
	CKeyMgr*		m_pKeyMgr;
	CAStar*			m_pAstar;
	CSubTitle*		m_pSubTitle;
	CGameObject*	m_pHammer;


public:
	// 자막
	bool			m_bIsMakeCompleteSub2;
	bool			m_bIsMakeCompleteSub3;
	bool			m_bIsMakeCompleteSub4;
	bool			m_bIsMakeCompleteSub5;
	bool			m_bIsMakeCompleteSub6;
	bool			m_bIsMakeCompleteSub7;
	bool			m_bIsMakeCompleteSub8;
	bool			m_bIsMakeCompleteSub9;
	bool			m_bIsMakeCompleteSub10;
	bool			m_bIsMakeCompleteSub11;
	bool			m_bIsMakeCompleteSub12;
	bool			m_bIsMakeCompleteSub13;
	bool			m_bIsMakeCompleteSub14;
	bool			m_bIsMakeCompleteSub15;
	bool			m_bIsMakeCompleteSub20;
	bool			m_bIsMakeCompleteSub21;

	bool			m_bIsMakeCompleteSub23;
	bool			m_bIsMakeCompleteSub24;
	bool			m_bIsMakeCompleteSub25;
	bool			m_bIsMakeCompleteSub26;
	bool			m_bIsMakeCompleteSub27;
	bool			m_bIsMakeCompleteSub28;


	float			m_fSub12ToSub13Time;
	bool			m_bIsCountingSub12ToSub13Time;


public:
	void			CheckIndexForSubTitle(int iIndex);

	// 화면 흔들림
	float			m_fShakingRangeX;
	float			m_fShakingRangeY;
	bool			m_bIsInitSettingZoomIn;

	// 해머 습득
	void			SetIsMeetHammerInit(bool bIsMeetHammerInit)
	{
		m_bIsMeetHammerInit = bIsMeetHammerInit;
	}
	bool			GetIsMeetHammerInit()
	{
		return m_bIsMeetHammerInit;
	}
	bool			m_bIsMeetHammerInit;

	// 건 습득
	void			SetIsMeetGunInit(bool bIsMeetGunInit)
	{
		m_bIsMeetGunInit = bIsMeetGunInit;
	}
	bool			GetIsMeetGunInit()
	{
		return m_bIsMeetHammerInit;
	}
	bool			m_bIsMeetGunInit;


	// 포탈, 씬 전환
	bool			m_bIsGetReadyToPortal;
	bool			m_bIsCompletedSettingPosInSceneChange;
	int				m_iWhereToGo;

	int				GetWhereToGo()
	{
		return m_iWhereToGo;
	}
	void			SetWhereToGo(int iWhereToGo)
	{
		m_iWhereToGo = iWhereToGo;
	}


	// 사운드
	bool			m_bIsPlayedHammer;
	bool			m_bIsPlayedGun;

	// template
private:
	template <typename T>
	CGameObject* CreateBullet(D3DXVECTOR3 vDir, 
		D3DXVECTOR3 vPos)
	{
		float fPlayerCY = PLAYERCY_ORI * 0.5f;
		float fMagicCX = 10.f;

		CGameObject* pObj = CFactory<T>::CreateGameObject();

		pObj->SetPos(vPos.x, vPos.y - fPlayerCY);
		pObj->SetDir(vDir);

		return pObj;
	}

	template <typename T>
	CGameObject* CreateEffect(/*D3DXVECTOR3 vDir, */eObjectDir eDir)
	{
		CGameObject* pObj = CFactory<T>::CreateGameObject();

		dynamic_cast<CEffect*>(pObj)->SetObjecteDir(eDir);
/*		dynamic_cast<CEffect*>(pObj)->SetObjectDir(vDir);*/

		return pObj;
	}
};
