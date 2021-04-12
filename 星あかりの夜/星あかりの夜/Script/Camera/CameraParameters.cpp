/**
 * @file    CameraParameters.cpp
 * @brief  カメラパラメータ読み込み処理
 *
 * @author Takuya Fujisawa
 * @date   2020/12/24
 */

#include"CameraParameters.h"
#include<fstream>
using namespace starrynight::camera;

CameraParameters::CameraParameters()
{
	map_camera_param_.clear();
}

CameraParameters::~CameraParameters()
{

}

void CameraParameters::LoadCameraParameters(const std::vector<std::string>& _param_name)
{
	std::string json_name = "Resource/json/parameters/camera.json";
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

	//playerオブジェクトからパラメータを読み込む
	auto player_param = json_object["camera"].get<picojson::object>();

	for (auto&& iter : _param_name)
	{
		if (player_param[iter.c_str()].is<double>())
		{
			map_camera_param_.emplace(iter, static_cast<float>(player_param[iter.c_str()].get<double>()));
		}
	}
}

float CameraParameters::GetCameraParam(const std::string _param_name)
{
	auto iter = map_camera_param_.find(_param_name);

	//パラメータがあったら返す
	if (iter != map_camera_param_.end())
	{
		return iter->second;
	}

	clsDx();
	printfDx("パラメータ : %s が見つかりません\n", _param_name.c_str());
	return 0;
}