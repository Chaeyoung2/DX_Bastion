#pragma once 

#include "Include.h"

class CFileInfo
{
public:
	wstring		ConvertRelativePath(wstring wstrPath);

	void	DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	// 폴더 정보를 추출하는 용도의 함수

	int		DirFileCount(const wstring& wstrPath);
	// 폴더 안에 있는 이미지 개수를 반환하는 함수

public:
	CFileInfo(void);
	~CFileInfo(void);
};