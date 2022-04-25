#pragma once
#include "gameobject.h"


class CBlackScreen :
	public CGameObject
{
public:
	CBlackScreen(void);
	virtual ~CBlackScreen(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					FadeIn();
	void					FadeOut();

	void					SetAlphaSpeed(float fAlphaSpeed)
	{
		m_fAlphaSpeed = fAlphaSpeed;
	}

private:
	int						m_iAlphaValue;
	float					m_fAlphaSpeed;

	float					m_fTime;
	float					m_fFadeTime;
};
