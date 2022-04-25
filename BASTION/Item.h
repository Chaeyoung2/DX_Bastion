#pragma once
#include "gameobject.h"

class CItem :
	public CGameObject
{
public:
	CItem(void);
	virtual ~CItem(void);

public:
	void		SettingCollRect();

public:

	int			GetItemNumber()
	{
		return m_iItemNumber;
	}
	bool		GetIsGetCompleted()
	{
		return m_bIsGetCompleted; 
	}

	void		SetIsGetCompleted(bool bIsGetCompleted)
	{
		m_bIsGetCompleted = bIsGetCompleted;
	}


protected:
	bool		m_bIsGetCompleted;
	int			m_iItemNumber;
};
