
#include "stdafx.h"
#include "terrainimporter.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace graphic { namespace importer {

	using boost::property_tree::ptree;

	void ReadRawTerrainFileV1( ptree &props, OUT sRawTerrain &out );
	void ReadRawLayerV1( ptree &props, OUT sRawLayer &out );
	void ReadRawModelV1( ptree &props, OUT sRawTerrrainModel &out );

}}

using namespace graphic;
using namespace graphic::importer;


// 지형 파일을 읽는다.
// json 포맷으로 저장된 파일을 해석해서 sRawTerrain 구조체에 채워 리턴한다.
bool importer::ReadRawTerrainFile( const string &fileName, OUT sRawTerrain &out )
{
	bool result = true;
	try
	{

		ptree props;
		boost::property_tree::read_json(fileName, props);

		const string version = props.get<string>("terrain version");

		ptree::assoc_iterator itor = props.find("info");
		if (props.not_found() != itor)
		{
			ReadRawTerrainFileV1(itor->second, out);
		}

	}
	catch (std::exception &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", MB_OK);
		return false;
	}
	
	return true;
}


void importer::ReadRawTerrainFileV1( ptree &props, OUT sRawTerrain &out )
{
	const string emptyStr;

	out.rowCellCount = props.get<int>("rowCellCount");
	out.colCellCount = props.get<int>("colCellCount");
	out.cellSize = props.get<float>("cellSize");
	out.heightMap = props.get<string>("heightMap", emptyStr);
	out.bgTexture = props.get<string>("bg texture", emptyStr);
	out.alphaTexture = props.get<string>("alpha texture", emptyStr);
	out.textureFactor = props.get<float>("texture factor", 1.f);
	out.heightFactor = props.get<float>("height factor", 1.f);

	ptree::assoc_iterator itor1 = props.find("layer");
	if (props.not_found() != itor1)
	{
		int idx = 0;
		BOOST_FOREACH(auto &child, itor1->second)
		{
			if (idx < (sizeof(out.layer) / sizeof(sRawLayer)))
				ReadRawLayerV1(child.second, out.layer[ idx++]);
		}
	}

	ptree::assoc_iterator itor = props.find("model");
	if (props.not_found() != itor)
	{
		out.models.clear();
		out.models.reserve(16);
		BOOST_FOREACH(auto &child, itor->second)
		{
			out.models.push_back( sRawTerrrainModel() );
			ReadRawModelV1(child.second, out.models.back());
		}
	}

}


void importer::ReadRawLayerV1( ptree &props, OUT sRawLayer &out )
{

	out.texture = props.get<string>("texture", "");

}


void importer::ReadRawModelV1( ptree &props, OUT sRawTerrrainModel &out )
{
	out.fileName = props.get<string>("filename", "");

	string mat1 = props.get<string>("tm1", "");

}
