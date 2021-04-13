/**
 * @file    LoadImageParamFromJson.cpp
 * @brief  画像情報をjsonから読み出す処理
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include"Parameters.h"
#include"../Picojson/picojson.h"
#include"../Resource/ResourceServer.h"
#include"../Utility/DefineMacro.h"
#include<fstream>

namespace parameters
{
	void Parameters::LoadImagParameters(const std::string& _file_name, const bool& _async_flag)
	{
		std::string json_name = "Resource/json/image/" + _file_name + ".json";
		std::ifstream ifs(json_name);

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
		auto imag_array = json_object["image"].get<picojson::array>();

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

			resource::ResourceServer::RegisterDivGraph(_file_name, param.filename_, param.xnum_ * param.ynum_,
				param.xnum_, param.ynum_, param.xsize_, param.ysize_, _async_flag);

			map_imag_param_.emplace(param.filename_, param);
		}
	}
}