
#include "stdafx.h"
#include "terrainexporter.h"
#include <fstream>

using namespace graphic;
using namespace graphic::exporter;
using namespace std;


// 지형정보를 json 포맷으로 저장한다.
bool exporter::WriteRawTerrainFile( const string &fileName, const sRawTerrain &terrain )
{
	ofstream of(fileName);
	if (!of.is_open())
		return false;

	of << "{" << endl;

	of << "\t\"terrain version\" : \"v1\"," << endl;
	of << "\t\"info\" : " << endl;
	of << "\t{" << endl;
	of << "\t\"rowCellCount\" : " << terrain.rowCellCount << "," << endl;
	of << "\t\"colCellCount\" : " << terrain.colCellCount << "," << endl;
	of << "\t\"cellSize\" : " << terrain.cellSize << "," << endl;
	of << "\t\"heightMap\" : " << "\"" << terrain.heightMap << "\"," << endl;
	of << "\t\"bg texture\" : " << "\"" << terrain.bgTexture << "\"," << endl;
	of << "\t\"alpha texture\" : " << "\"" << terrain.alphaTexture << "\"," << endl;
	of << "\t\"texture factor\" : " << terrain.textureFactor << "," << endl;
	of << "\t\"height factor\" : " << terrain.heightFactor << "," << endl;

	of << "\t\"layer\" : [" << endl;
	for (int i=0; i < 4; ++i)
	{
		of << "\t\t{" << endl;
		of << "\t\t\t\"texture\" : " << "\"" << terrain.layer[ i].texture<< "\"" << endl;
		of << "\t\t}"; 
		if (i < 3)
			of << ",";
		of << endl;
	}
	of << "\t]," << endl;
	

	of << "\t\"model\" : [" << endl;
	for (u_int i=0; i < terrain.models.size(); ++i)
	{
		of << "\t\t{" << endl;
		of << "\t\t\t\"filename\" : " << "\"" << terrain.models[ i].fileName
			<< "\"," << endl;
		of << "\t\t\t\"tm1\" : " << "\"" << "0 0 0 0" << "\"" << endl;
		of << "\t\t}"; 
		if (i <  terrain.models.size()-1)
			of << ",";
		of << endl;
	}
	of << "\t]" << endl;

	of << "\t}" << endl;
	of << "}" << endl;


	return true;
}
