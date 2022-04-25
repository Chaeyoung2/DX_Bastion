#pragma once
#include "afxwin.h"

#include "Include.h"
#include "afxcmn.h"


// CObjTool 대화 상자입니다.

class CObjTool : public CDialog
{
	DECLARE_DYNAMIC(CObjTool)
// public:
// 	enum eObjToolMode	{ OBJTOOL_GROUND, OBJTOOL_STRUCTURE };

private:
	map<CString, PMYOBJ>		m_mapGroundData;
	map<CString, CImage*>		m_mapGroundImage;

	map<CString, PMYOBJ>		m_mapStructureData;
	map<CString, CImage*>		m_mapStructureImage;

	list<PMYOBJ>*				m_pStructureList;
	list<PMYOBJ>*				m_pGroundList;

public:
	void						SetStructureList(list<PMYOBJ>* pObjList) 
	{ m_pStructureList = pObjList; }
	void						SetGroundList(list<PMYOBJ>* pObjList) 
	{ m_pGroundList = pObjList; }
	void						Release();
	void						HorizontalScroll();
	void						SetAddedObjList();

public:
	bool						m_bIsStMode;

public: // OBJ 구조체
	int							m_iObjID;
	int							m_iCount;
	float						m_fAngle;

public:
	int							m_iStIndex;
	int							m_iGrIndex;
	int							m_iSelectedIndex;
	int							m_iCollCount;

public:
	int*						GetStIndex() { return &m_iStIndex; }
	int*						GetGrIndex() { return &m_iGrIndex; }

public:
	bool						m_bIsSettingStructure;
	bool						m_bIsAgreeToDamage;


public:
	CObjTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ObjPicture;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCheckGrSt();
	CListBox m_ListBoxStructure;
	CListBox m_ListBoxGround;
	afx_msg void OnLbnSelchangeListObjStructure();
	afx_msg void OnLbnSelchangeListObjGround();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonObjDeleteall();
	CComboBox m_ComboRenderOption;
	virtual BOOL OnInitDialog();
	// // world = 0, forward = 1, backward = 2
	int m_iRenderOption;
	afx_msg void OnCbnSelchangeComboRenderingOption();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedButtonStSave();
	afx_msg void OnBnClickedButtonStLoad();
	afx_msg void OnBnClickedButtonGrSave();
	afx_msg void OnBnClickedButtonGrLoad();
	CListBox m_ListBoxGrAdded;
	CListBox m_ListBoxStAdded;
	afx_msg void OnBnClickedButtonStDeleteselected();
	afx_msg void OnBnClickedButtonGrDeleteselected();
	afx_msg void OnLbnSelchangeListStAdded();
	afx_msg void OnLbnSelchangeListGrAdded();
	afx_msg void OnBnClickedCheckAgreetocol();
};
