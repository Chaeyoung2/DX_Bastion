#pragma once

/*#define CHAEYOUNG*/

#define NO_COPY(ClassName)				\
private:								\
	ClassName(const ClassName&);				\
	ClassName& operator = (const ClassName&);	\

#define DECLARE_SINGLETON(ClassName)	\
	NO_COPY(ClassName)					\
private:								\
	static ClassName* m_pInstance;		\
public:									\
	static ClassName* GetInstance()		\
	{									\
	if(NULL == m_pInstance)			\
	m_pInstance = new ClassName;\
	return m_pInstance;			\
	}									\
	void DestroyInstance()				\
	{									\
	if(m_pInstance)					\
		{								\
		delete m_pInstance;			\
		m_pInstance = NULL;			\
		}								\
	}

#define IMPLEMENT_SINGLETON(ClassName)			\
	ClassName* ClassName::m_pInstance = NULL;