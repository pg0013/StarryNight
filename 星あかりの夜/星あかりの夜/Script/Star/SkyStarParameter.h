/**
 * @file    SkyStarParameter.h
 * @brief  星座のパラメータを読み込むクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/27
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
    namespace star
    {
        class SkyStarParameter :
            public parameters::Parameters
        {
        public:
            SkyStarParameter();
            ~SkyStarParameter();

            /**
             * @brief ステージの星座情報を読み込む.
             *
             * @param _stagename　ステージ名
             * @param _async_flag　非同期読み込みフラグ
             */
            void LoadSkyStar(std::string _stagename, bool _async_flag = false);

            /**
             * @brief　 星座のパラメータを読み込む
             *
             * @param  _name    星座名
             * @param  _param_name　パラメータ名を格納したコンテナ
             * @param  _async_flag  非同期読み込みフラグ
             */
            void LoadSkyStarParameter(std::string _name, const std::vector<std::string>& _param_name,bool _async_flag = false);

        private:
            //星座パラメータ記録用変数
            std::unordered_map<std::string, float> map_skystar_param_;
        };
    }
}