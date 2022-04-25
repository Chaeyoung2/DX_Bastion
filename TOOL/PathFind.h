#pragma once


// CPathFind 대화 상자입니다.

#include "Include.h"
#include "afxwin.h"

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

private:
	list<IMGPATH*>		m_ListPathInfo;

public:
	void Release();

public:
	CPathFind(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFind();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPathfindload();
	afx_msg void OnBnClickedButtonPathfindsave();
	CListBox m_ListBoxPathFind;
	afx_msg void OnLbnSelchangeListBoxPathFind();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
