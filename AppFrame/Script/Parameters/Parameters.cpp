/**
 * @file    Parameters.cpp
 * @brief  ゲーム内定数管理クラス
 *
 * @author Takuya Fujisawa
 * @date   2020/11/02
 */

#include"Parameters.h"
#include "DxLib.h"
#include"../Picojson/picojson.h"
#include"../Resource/ResourceServer.h"
#include<fstream>

namespace parameters
{
	Parameters::Parameters() {};
	Parameters::~Parameters() {};

	void Parameters::LoadImagParameters(std::string _mode, bool _async_flag)
	{
		std::ifstream ifs("Resource/json/imag.json");

		if (ifs.fail())
		{
			printfDx("json file can not open\n");
			return;
		}

		picojson::value json_value;

		ifs >> json_value;

		ifs.close();

		const auto error = picojson::get_last_error();

		if (!error.empty())
		{
			printfDx("json file can not read\n");
			return;
		}

		//指定モード名のオブジェクトからimag配列を読み込む
		auto json_object = json_value.get<picojson::object>();
		auto mode_object = json_object[_mode].get<picojson::object>();
		auto imag_array = mode_object["imag"].get<picojson::array>();

		for (auto iter = imag_array.begin(); iter != imag_array.end(); iter++)
		{
			auto imag = (*iter).get<picojson::object>();
			imag_param param = {};

			if (imag["filename"].is<std::string>())
			{
				param.filename_ = imag["filename"].get<std::string>();
			}

			if (imag["xnum"].is<double>())
			{
				param.xnum_ = static_cast<int>(imag["xnum"].get<double>());
			}

			if (imag["ynum"].is<double>())
			{
				param.ynum_ = static_cast<int>(imag["ynum"].get<double>());
			}

			if (imag["xsize"].is<double>())
			{
				param.xsize_ = static_cast<int>(imag["xsize"].get<double>());
			}

			if (imag["ysize"].is<double>())
			{
				param.ysize_ = static_cast<int>(imag["ysize"].get<double>());
			}

			resource::ResourceServer::RegisterDivGraph(_mode, param.filename_.c_str(), param.xnum_ * param.ynum_,
				param.xnum_, param.ynum_, param.xsize_, param.ysize_, _async_flag);
		}
	}
}