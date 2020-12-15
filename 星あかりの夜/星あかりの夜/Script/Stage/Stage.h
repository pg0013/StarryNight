#pragma once
#include"appframe.h"
#include<vector>

namespace starrynight
{
	class Stage
	{
	public:
		Stage();
		~Stage();

		void Initialize();
		void Process();
		void Render();

		void ClearHandle();

	private:
		parameters::Parameters stage_param_;
		std::vector<handle> stage_handle_;
	};
}