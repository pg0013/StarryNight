#pragma once
#include"DxLib.h"

namespace camera
{
	class Camera
	{
	public:
		VECTOR position_;
		VECTOR target_;

		float clip_near_;
		float clip_far_;
	};
}