#pragma once
#include "effect.h"

class CBandibul :
	public CEffect
{
public:
	CBandibul(void);
	virtual ~CBandibul(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

	void SetAngle(float fAngle)
	{
		m_fAngle = fAngle;
	}
	void SetDistance(float fDist)
	{
		m_fDistance = fDist;
	}

private:
//	float	m_fCenterX;
//	float	m_fCenterY;
	float	m_fAngle;
	
//	float	m_fRotAngle;
//	float	m_fRotSpeed;

	float	m_fDistance;

//	bool	m_bIsSettingCenter;

	float	m_fTime;

};
