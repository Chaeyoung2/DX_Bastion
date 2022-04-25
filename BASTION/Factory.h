#pragma once
#include "MapObject.h"
#include "Bandibul.h"
#include "SubTitle.h"
#include "DBox.h"

class CGameObject;

template<typename T>
class CFactory abstract
{
public:
	static CGameObject* CreateGameObject(void)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateGameObject(D3DXVECTOR3 vPos)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPos(vPos);

		return pObject;
	}

	static CGameObject* CreateGameObject(float fX, float fY)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(fX, fY);
		pObject->Initialize();
		

		return pObject;
	}

	static CGameObject* CreateGameObject(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(vPos);
		pObject->SetDir(vDir);
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateStructure(PMYOBJ pMyObj)
	{
		CGameObject* pObject = new T;
		dynamic_cast<CMapObject*>(pObject)->SetObjInfo(pMyObj);
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateBandibul(float fX, float fY, float fAngle, float fSpeed, float fDist)
	{
		CGameObject* pObject = new T;
		dynamic_cast<CBandibul*>(pObject)->SetAngle(fAngle);
		pObject->SetPos(fX, fY);
		pObject->SetSpeed(fSpeed);
		dynamic_cast<CBandibul*>(pObject)->SetDistance(fDist);
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateSubTitle(float fFrame)
	{
		CGameObject* pObject = new T;
		dynamic_cast<CSubTitle*>(pObject)->SetFrame(fFrame);
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateDBox(float fX, float fY, float fFrame)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(fX, fY);
		dynamic_cast<CDBox*>(pObject)->SetFrame(fFrame);
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateSpinGapella(float fX, float fY, eObjectDir eDir)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(fX, fY);
		dynamic_cast<CMonster*>(pObject)->SetDirection(eDir);
		dynamic_cast<CGasPella*>(pObject)->SetIsSpin(true);
		pObject->Initialize();

		return pObject;
	}
};
