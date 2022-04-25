#pragma once 

#include "Include.h"

class CFileInfo
{
public:
	wstring		ConvertRelativePath(wstring wstrPath);

	void	DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	// ���� ������ �����ϴ� �뵵�� �Լ�

	int		DirFileCount(const wstring& wstrPath);
	// ���� �ȿ� �ִ� �̹��� ������ ��ȯ�ϴ� �Լ�

public:
	CFileInfo(void);
	~CFileInfo(void);
};