#pragma once


namespace graphic
{

	namespace RESOURCE_TYPE {
		enum TYPE {
			NONE,
			MESH,
			ANIMATION,
		};
	};


	// 경계박스 min max 값.
	struct sMinMax
	{
		Vector3 Min;
		Vector3 Max;
		sMinMax() : Min(Vector3(0,0,0)), Max(Vector3(0,0,0)) {}
	};


}
