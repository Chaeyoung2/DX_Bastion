
// TOOLDoc.h : CTOOLDoc Ŭ������ �������̽�
//


#pragma once


class CTOOLDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CTOOLDoc();
	DECLARE_DYNCREATE(CTOOLDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CTOOLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


