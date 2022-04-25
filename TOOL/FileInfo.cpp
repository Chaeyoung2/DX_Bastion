#include "StdAfx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo(void)
{
}

CFileInfo::~CFileInfo(void)
{
}

std::wstring CFileInfo::ConvertRelativePath(wstring wstrPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szDirPath[MAX_PATH]		 = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	
	// vcproj 파일이 있는 위치 까지의 경로를 얻어오는 함수

	PathRelativePathTo((LPWSTR)szRelativePath, 
		szDirPath, FILE_ATTRIBUTE_DIRECTORY,
		wstrPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	// 한 경로에서 다른 경로로 가는 상대 경로를 추출(같은 드라이브에서만 가능)

	return szRelativePath;

}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList)
{
	CFileFind		Find;

	Find.FindFile(wstrPath.c_str());	// 경로 안에 있는 파일의 각종 정보를 Find 객체에 저장

	int		iContinue = 1;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();	// 파일의 유무를 검사하는 함수

		if(Find.IsDots())					// 파일 폴더 경로 중 .또는 ..인 경우를 걸러내는 조건
			continue;			

		if(Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathInfoList);
		}
		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR		    szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPathBuf);

			pImgPath->iCount = DirFileCount(szPathBuf);

			wstring wstrTextureName = Find.GetFileTitle().operator LPCWSTR(); // 경로를 제외한 파일의 이름만을 얻어내는 함수

			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// 0번부터 순수 문자열 길이만큼 잘라내고 정수 삽입

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, (LPWSTR)Find.GetFilePath().operator  LPCWSTR());

			PathRemoveFileSpec(szBuf);	

			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());	// 결과물 = 얻어 낸 경로 + 새로 만든 파일 이름
			pImgPath->wstrPath = ConvertRelativePath(szBuf);

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrStateKey = PathFindFileName(szBuf);	// 현 위치에 있는 파일 이름 추출 함수

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrObjKey = PathFindFileName(szBuf);	// 현 위치에 있는 파일 이름 추출 함수

			rPathInfoList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
	}
}

int CFileInfo::DirFileCount(const wstring& wstrPath)
{
	wstring wstrTemp = wstrPath + L"\\*.png";

	CFileFind	Find;
	Find.FindFile(wstrTemp.c_str());

	int		iContinue = 1;
	int		iFileCount = 0;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}
