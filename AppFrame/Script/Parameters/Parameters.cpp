/**
 * @file    Parameters.cpp
 * @brief  ゲーム内定数管理クラス
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

	void Parameters::LoadImagParameters(const std::string _mode, bool _async_flag)
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

			resource::ResourceServer::RegisterDivGraph(_mode, param.filename_, param.xnum_ * param.ynum_,
				param.xnum_, param.ynum_, param.xsize_, param.ysize_, _async_flag);

			map_imag_param_.emplace(param.filename_, param);
		}
	}

	void Parameters::LoadModel(std::string _mode, bool _async_flag)
	{
		std::ifstream ifs("Resource/json/model.json");

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
		auto model_array = mode_object["model"].get<picojson::array>();

		//ファイル重複チェック用変数
		std::string old_file_name = "";
		int name_count = 0;

		for (auto iter = model_array.begin(); iter != model_array.end(); iter++)
		{
			auto model = (*iter).get<picojson::object>();
			model_param param = {};

			if (model["filename"].is<std::string>())
			{
				param.filename_ = model["filename"].get<std::string>();
				param.handlename_ = model["filename"].get<std::string>();
			}

			//前回読み込んだファイル名と比較し、同じであれば数字付きのハンドル名にする
			if (param.handlename_ == old_file_name)
			{
				name_count++;
				param.handlename_ = param.filename_ + "_" + std::to_string(name_count);
			}
			else
			{
				name_count = 0;
			}
			old_file_name = param.filename_;

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
				param.rotation_.x = RAD2DEG(param.rotation_.x);
			}
			if (model["ry"].is<double>())
			{
				param.rotation_.y = static_cast<float>(model["ry"].get<double>());
				param.rotation_.y = RAD2DEG(param.rotation_.y);
			}
			if (model["rz"].is<double>())
			{
				param.rotation_.z = static_cast<float>(model["rz"].get<double>());
				param.rotation_.z = RAD2DEG(param.rotation_.z);
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
			resource::ResourceServer::RegisterModel(_mode, param.filename_, param.handlename_, _async_flag);

			//モデルの初期パラメータをそれぞれセット
			handle model_handle = resource::ResourceServer::GetModelHandle(param.handlename_);
			MV1SetPosition(model_handle, param.position_);
			MV1SetRotationXYZ(model_handle, param.rotation_);
			MV1SetScale(model_handle, param.scale_);

			map_model_param_.emplace(param.handlename_, param);
		}
	}

	void Parameters::LoadStage(std::string _stagename, bool _async_flag)
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

		//ファイル重複チェック用変数
		std::string old_file_name = "";
		int name_count = 0;

		for (auto iter = model_array.begin(); iter != model_array.end(); iter++)
		{
			auto model = (*iter).get<picojson::object>();
			model_param param = {};

			if (model["filename"].is<std::string>())
			{
				param.filename_ = model["filename"].get<std::string>();
			}

			if (model["handlename"].is<std::string>())
			{
				param.handlename_ = model["handlename"].get<std::string>();
			}

			////前回読み込んだファイル名と比較し、同じであれば数字付きのハンドル名にする
			//if (param.handlename_ == old_file_name)
			//{
			//	name_count++;
			//	param.handlename_ = param.filename_ + "_" + std::to_string(name_count);
			//}
			//else
			//{
			//	name_count = 0;
			//}
			//old_file_name = param.filename_;

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
}