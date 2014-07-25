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
			D3DFVF_TEXCOORDSIZE4(2)) };	// ble		};

		//enum {FVF = (D3DFVF_XYZB4 | D3DFVF_NORMAL | D3DFVF_TEX3 | 
		//D3DFVF_TEXCOORDSIZE2(0)|
		//D3DFVF_TEXCOORDSIZE4(1)
		//) };
		//Vector3 p;
		//Vector3 n;
		//float u,v;
		//float weights[4];
		//float indices[4];

		//enum {FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3 |
		//	D3DFVF_TEXCOORDSIZE2(0)|		// texture
		//	D3DFVF_TEXCOORDSIZE4(1)|		// blend weight
		//	D3DFVF_TEXCOORDSIZE4(2)) };	// ble		};
	};

}
