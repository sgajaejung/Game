#pragma once


namespace graphic
{

	struct sVertexDiffuse
	{
		Vector3 p;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};


	struct sVertexNormDiffuse
	{
		Vector3 p;
		Vector3 n;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE };
	};


	struct sVertexNormTex
	{
		Vector3 p;
		Vector3 n;
		float u,v;

		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};


	// vertex skinning
	// using texcoord
	struct sVertexNormTexSkin
	{
		Vector3 p;
		Vector3 n;
		float u,v;
		float weights[4];
		float matrixIndices[4];

		enum {FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3 |
			D3DFVF_TEXCOORDSIZE2(0)|		// texture
			D3DFVF_TEXCOORDSIZE4(1)|		// blend weight
			D3DFVF_TEXCOORDSIZE4(2)) };	// blend indices
	};


	// vertex skinning
	// using D3DFVF_XYZB5 | D3DFVF_LASTBETA_D3DCOLOR
	struct sVertexNormTexSkin2
	{
		Vector3 p;
		float weights[4];
		DWORD matrixIndices;
		Vector3 n;
		float u,v;	

		enum {FVF = (D3DFVF_XYZB5 | D3DFVF_LASTBETA_D3DCOLOR | 
			D3DFVF_NORMAL | D3DFVF_TEX1)};
	};

}
