/**
 * @file    Parameters.cpp
 * @brief  ƒQ[ƒ€“à’è”ŠÇ—ƒNƒ‰ƒX
 *
 * @author Takuya Fujisawa
 * @date   2020/11/02
 */

#include"Parameters.h"
#include"../Picojson/picojson.h"
#include"../Resource/ResourceServer.h"
#include"../Utility/DefineMacro.h"
#include<fstream>

namespace parameters
{
	Parameters::Parameters()
	{
		map_imag_param_.clear();
		map_model_param_.clear();
	};

	Parameters::~Parameters()
	{
		map_imag_param_.clear();
		map_model_param_.clear();
	};
}