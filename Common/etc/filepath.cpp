
#include "stdafx.h"
#include "FilePath.h"
#include <shlwapi.h> // 이 헤더 파일에 FilePath에 관련된 함수들이 많다. 잘 이용해보자.
#pragma comment(lib, "shlwapi")


namespace common {

	bool CompareExtendName(const char *srcFileName, int srcStringMaxLength, const char *compareExtendName);

}


//------------------------------------------------------------------------
// fileName에서 파일이름과 확장자를 제외한 나머지 경로를 리턴한다.
// 마지막에 '\' 문자는 없다.
//------------------------------------------------------------------------
string common::GetFilePathExceptFileName(const std::string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	PathRemoveFileSpecA( srcFileName );
	return srcFileName;
}


/**
 @brief fileName의 디렉토리 경로를 제외한 파일이름과 확장자를 리턴한다.
 */
string common::GetFileName(const std::string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *name = PathFindFileNameA(srcFileName);
	return name;
}


//-----------------------------------------------------------------------------//
// szDirectory폴더에 pFindExt 확장자 리스트에 포함된 파일을 pFileList에 저장한다.
//
// szDirectory: 탐색하고자 하는 디렉토리 경로
// pFindExt: 찾고자 하는 확장자, 2개이상 설정할수있게 하기위해서 리스트 자료형태가 되었다.
// pFileList: 일치하는 확장자를 가진 파일이름을 저장한다.
//-----------------------------------------------------------------------------//
bool common::CollectFiles( const list<string> &findExt, const string &searchPath, OUT list<string> &out )
{
	WIN32_FIND_DATAA fd;
	const string searchDir = searchPath + "*.*";
	HANDLE hFind = FindFirstFileA(searchDir.c_str(), &fd);

	while (1)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ((string(".") != fd.cFileName) && (string("..") != fd.cFileName))
			{
				CollectFiles( findExt, searchPath + string(fd.cFileName) + "/", out ); 
			}
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			const string fileName = fd.cFileName;

			auto it = findExt.begin();
			while (findExt.end() != it)
			{
				if (CompareExtendName(fileName.c_str() , fileName.length(), it->c_str()))
				{
					out.push_back( searchPath + fileName );
					break;
				}
				++it;
			}
		}

		if (!FindNextFileA(hFind, &fd))
			break;
	}

	FindClose(hFind);
	
	return true;
}



//------------------------------------------------------------------------
// srcFileName의 확장자와 compareExtendName 이름이 같다면 true를 리턴한다.
// 확장자는 srcFileName 끝에서 '.'이 나올 때까지 이다.
//------------------------------------------------------------------------
bool common::CompareExtendName(const char *srcFileName, const int srcStringMaxLength, const char *compareExtendName)
{
	const int len = strnlen_s(srcFileName, srcStringMaxLength);
	if (len <= 0)
		return FALSE;

	int count = 0;
	char temp[5];
	for (int i=0; i < len && i < 4; ++i)
	{
		const char c = srcFileName[ len-i-1];
		if ('.' == c)
		{
			break;
		}
		else
		{
			temp[ count++] = c;
		}
	}
	temp[ count] = NULL;

	char extendName[5];
	for (int i=0; i < count; ++i)
		extendName[ i] = temp[ count-i-1];
	extendName[ count] = NULL;

	if (!strcmp(extendName, compareExtendName))
	{
		return true;
	}

	return false;
}


// searchPath 디렉토리 안에서 findName 의 파일이름을 가진 파일이 있다면 해당 경로를
// out 에 저장하고 true 를 리턴한다.
bool common::FindFile( const string &findName, const string &searchPath, string &out  )
{
	WIN32_FIND_DATAA fd;
	const string searchDir = searchPath + "*.*";
	HANDLE hFind = FindFirstFileA(searchDir.c_str(), &fd);

	while (1)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ((string(".") != fd.cFileName) && (string("..") != fd.cFileName))
			{
				if (FindFile( findName, searchPath + string(fd.cFileName) + "/", out ))
					break;
			}
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			const string fileName = fd.cFileName;
			if (fileName == GetFileName(findName))
			{
				out = searchPath + GetFileName(findName);
				break;
			}
		}

		if (!FindNextFileA(hFind, &fd))
			break;
	}

	FindClose(hFind);

	return !out.empty();
}
