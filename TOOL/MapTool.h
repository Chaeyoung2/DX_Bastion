#pragma once
#include "afxwin.h"


// CMapTool 대화 상자입니다.

#include "Include.h"
#include "afxcmn.h"

class CBackGround;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

private:
	map<CString, CImage*>		m_mapPngImage;

public:
	int		m_iDrawID;
	CBackGround*	m_pBackGround;
	void SetBackGround(CBackGround* pBackGround) { m_pBackGround = pBackGround; }




private:
	void HorizontalScroll();
	void Release();

	

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	// 타일 옵션
	CComboBox m_ComboOptionList;
	// 타일 경로
	CListBox m_ListBoxTilePath;
	CStatic m_TilePicture;
	afx_msg void OnLbnSelchangeListTile();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnBnClickedButtonMapsave();
	afx_msg void OnBnClickedButtonMapload();
	afx_msg void OnBnClickedButtonMapdeleteall();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboMapoption();
	int m_iOption;
	BOOL m_bRenderOp;
	BOOL m_bRenderBor;
	BOOL m_bRenderIndex;
	CButton m_CheckRenderBor;
	CButton m_CheckRenderOp;
	CButton m_CheckRenderIndex;
	afx_msg void OnBnClickedCheckRenderBorder();
	afx_msg void OnBnClickedCheckRenderOption();
	afx_msg void OnBnClickedCheckRenderIndex();

	// 활성화되어있는지
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);


	// 높이
	CSliderCtrl m_slHeight;
	afx_msg void OnTRBNThumbPosChangingSliderHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit m_editHeight;
	float m_fHeight;
	bool	m_bIsSetHeight;
	afx_msg void OnTRBNThumbPosChangingSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fMoveSpeed;
	CEdit m_EditMoveSpeed;
	CSliderCtrl m_slMoveSpeed;
};
