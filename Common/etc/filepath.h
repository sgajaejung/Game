#pragma once


namespace common
{

	string GetFilePathExceptFileName(const std::string &fileName);

	string GetFileName(const std::string &fileName);

	bool CollectFiles( const list<string> &findExt, const string &searchPath, OUT list<string> &out );

	bool FindFile( const string &findName, const string &searchPath, string &out  );

}
