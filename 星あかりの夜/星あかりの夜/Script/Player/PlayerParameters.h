/**
 * @file    PlayerParameters.h
 * @brief  プレイヤーパラメータ読み込み処理
 *
 * @author Takuya Fujisawa
 * @date   2020/12/23
 */
#pragma once
#include"appframe.h"
#include<unordered_map>
#include<vector>

namespace starrynight
{
    namespace player
    {
        class PlayerParameters :
            public parameters::Parameters
        {
        public:
            PlayerParameters();
            ~PlayerParameters();

            /**
             * @brief　 プレイヤーパラメータをjsonファイルから読み込む
             *
             */
            void LoadPlayerParameters(const std::vector<std::string>& _param_name);

            /**
             * @brief　 mapからプレイヤーパラメータを取得する
             *
             * @param  _param_name　パラメータ名（jsonに記載）
             * @return   パラメータ(float)
             */
            float GetPlayerParam(const std::string _param_name);

        private:
            //プレイヤーパラメータ記録用変数
            std::unordered_map<std::string, float> map_player_param_;
        };
    }
}