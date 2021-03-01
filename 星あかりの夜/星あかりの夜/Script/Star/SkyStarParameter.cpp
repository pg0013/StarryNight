/**
 * @file    SkyStarParameter.h
 * @brief  星座のパラメータを読み込むクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/27
 */

#include "SkyStarParameter.h"
#include<fstream>

using namespace starrynight::star;

SkyStarParameter::SkyStarParameter()
{
}

SkyStarParameter::~SkyStarParameter()
{
}

void SkyStarParameter::LoadSkyStar(std::string _stagename, bool _async_flag)
{
	std::string json_name = "Resource/json/stage/" + _stagename + "/skystar.json";
	std::ifstream ifs(json_name.c_str());

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
	auto model_array = json_object["skystar"].get<picojson::array>();

	for (auto iter = model_array.begin(); iter != model_array.end(); iter++)
	{
		auto model = (*iter).get<picojson::object>();
		parameters::model_param param = {};

		if (model["filename"].is<std::string>())
		{
			param.filename_ = model["filename"].get<std::string>();
		}

		if (model["handlename"].is<std::string>())
		{
			param.handlename_ = model["handlename"].get<std::string>();
		}

		//positionを読み込み
		if (model["tx"].is<double>())
		{
			param.position_.x = static_cast<float>(model["tx"].get<double>());
		}
		if (model["ty"].is<double>())
		{
			param.position_.y = static_cast<float>(model["ty"].get<double>());
		}
		if (model["tz"].is<double>())
		{
			param.position_.z = static_cast<float>(model["tz"].get<double>());
		}

		//rotationを読み込み
		if (model["rx"].is<double>())
		{
			param.rotation_.x = static_cast<float>(model["rx"].get<double>());
			param.rotation_.x = DEG2RAD(param.rotation_.x);
		}
		if (model["ry"].is<double>())
		{
			param.rotation_.y = static_cast<float>(model["ry"].get<double>());
			param.rotation_.y = DEG2RAD(param.rotation_.y);
		}
		if (model["rz"].is<double>())
		{
			param.rotation_.z = static_cast<float>(model["rz"].get<double>());
			param.rotation_.z = DEG2RAD(param.rotation_.z);
		}

		//scaleを読み込み
		if (model["sx"].is<double>())
		{
			param.scale_.x = static_cast<float>(model["sx"].get<double>());
		}
		if (model["sy"].is<double>())
		{
			param.scale_.y = static_cast<float>(model["sy"].get<double>());
		}
		if (model["sz"].is<double>())
		{
			param.scale_.z = static_cast<float>(model["sz"].get<double>());
		}

		//ResourceServerにハンドルを保存
		resource::ResourceServer::RegisterModel("SkyStar", param.filename_, param.handlename_, _async_flag);

		//モデルの初期パラメータをそれぞれセット
		handle model_handle = resource::ResourceServer::GetModelHandle(param.handlename_);
		MV1SetPosition(model_handle, param.position_);
		MV1SetRotationXYZ(model_handle, param.rotation_);
		MV1SetScale(model_handle, param.scale_);

		map_model_param_.emplace(param.handlename_, param);
	}
}

void SkyStarParameter::LoadSkyStarParameter(std::string _name, const std::vector<std::string>& _param_name, bool _async_flag)
{
	std::string json_name = "Resource/json/parameters/skystar.json";
	std::ifstream ifs(json_name.c_str());

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

	auto json_array = json_value.get<picojson::array>();
	auto json_object = json_array[0].get<picojson::object>();

	//オブジェクトからパラメータを読み込む
	auto skystar_param = json_object[_name.c_str()].get<picojson::object>();

	for (auto iter = _param_name.begin(); iter != _param_name.end(); iter++)
	{
		if (skystar_param[iter->c_str()].is<double>())
		{
			map_skystar_param_.emplace((*iter), static_cast<float>(skystar_param[iter->c_str()].get<double>()));
		}
	}
}