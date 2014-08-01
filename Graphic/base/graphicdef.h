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


	namespace MODEL_TYPE {
		enum TYPE {
			SKIN,
			RIGID,
			AUTO,
		};
	};


	// 경계박스 min max 값.
	struct sMinMax
	{
		Vector3 Min;
		Vector3 Max;
		sMinMax() : Min(Vector3(0,0,0)), Max(Vector3(0,0,0)) {}

		void Update(const Vector3 &pos)
		{
			if (Min.x > pos.x)
				Min.x = pos.x;
			if (Min.y > pos.y)
				Min.y = pos.y;
			if (Min.z > pos.z)
				Min.z = pos.z;

			if (Max.x < pos.x)
				Max.x = pos.x;
			if (Max.y < pos.y)
				Max.y = pos.y;
			if (Max.z < pos.z)
				Max.z = pos.z;
		}
	};


}
