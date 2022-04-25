#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

#include "Include.h"


class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

private:
	map<CString, PMYUNIT>		m_mapUnitData;
	map<CString, CImage*>		m_mapUnitImage;
	map<CString, CImage*>		m_mapUnitImage_Ex; // 유닛툴 렌더용

	bool	m_bRadioButtonStartCheck;


public:
	int				m_iUnitID; // 0플레이어, 1가스펠라, 2방어시스템, 3보스
	int				m_iUnitDir;
	

	// MyUnit
public:
	void			SetUnitList(list<PMYUNIT>* pUnitList) { m_pUnitList = pUnitList; }
	TCHAR*			wstrName;
	list<PMYUNIT>*	m_pUnitList;



public:
	void Release();
	void HorizontalScroll();
	void SetAddedUnitList();

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNITTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// // 유닛의 방향 리스트
	CComboBox m_ComboDirList;


	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListUnitAdded();
	afx_msg void OnCbnSelchangeComboUnitDir();
	// 유닛 방향
	CString m_strComboUnitDir;
	CListBox m_ListBoxUnit;
	afx_msg void OnBnClickedButtonUnitAdd();
	// 추가된 유닛 리스트
	CListBox m_ListBoxUnitAdded;
	CStatic m_UnitPicture;
	afx_msg void OnLbnSelchangeListUnit();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonUnitSave();
	afx_msg void OnBnClickedButtonUnitLoad();
	afx_msg void OnBnClickedButtonUnitDeleteall();
	afx_msg void OnBnClickedButtonUnitDeleteselected();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
