/**
 * @file    StageParameters.cpp
 * @brief	ステージのハンドルや座標を管理するクラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */
#include "StageParameters.h"
#include<fstream>

using namespace starrynight::stage;

StageParameters::StageParameters()
{
}

StageParameters::~StageParameters()
{
}

void StageParameters::LoadStage(std::string _stagename, bool _async_flag)
{
	std::ifstream ifs("Resource/json/stage.json");

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
	auto mode_object = json_object["Stage"].get<picojson::object>();
	auto model_array = mode_object[_stagename].get<picojson::array>();

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

		//ステージ名のファイルパスを追加
		std::string stage_name = _stagename + "/" + param.filename_;

		//ResourceServerにハンドルを保存
		resource::ResourceServer::RegisterModel("Stage", stage_name, param.handlename_, _async_flag);

		//モデルの初期パラメータをそれぞれセット
		handle model_handle = resource::ResourceServer::GetModelHandle(param.handlename_);
		MV1SetPosition(model_handle, param.position_);
		MV1SetRotationXYZ(model_handle, param.rotation_);
		MV1SetScale(model_handle, param.scale_);

		map_model_param_.emplace(param.handlename_, param);
	}
}