#pragma once


namespace graphic
{

	struct sRawTerrrainModel
	{
		string fileName;
		Matrix44 tm;
	};

	struct sRawLayer
	{
		string texture;
	};


	struct sRawTerrain
	{
		int colCellCount;
		int rowCellCount;
		float cellSize;
		string heightMap;
		string bgTexture;
		string alphaTexture;
		float textureFactor;
		float heightFactor;
		sRawLayer layer[4];
		vector<sRawTerrrainModel> models;
	};


}
