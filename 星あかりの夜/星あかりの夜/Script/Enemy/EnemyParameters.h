/**
 * @file    EnemyParameters.h
 * @brief  エネミーパラメータ読み込みクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/20
 */
#pragma once
#include"appframe.h"
#include<unordered_map>
#include<vector>

namespace starrynight
{
    namespace enemy
    {
        /**
         * @brief  エネミーパラメータ読み込みクラス
         */
        class EnemyParameters :
            public parameters::Parameters
        {
        public:
            EnemyParameters();
            ~EnemyParameters();

            /**
             * @brief　 エネミーパラメータをjsonファイルから読み込む
             *
             * @param  _stagename   読み込むステージ名
             * @param  _async_flag     非同期読み込みを行うフラグ
             */
            void LoadStageEnemys(std::string _stagename, bool _async_flag);

            /**
             * @brief　 エネミーパラメータをjsonファイルから読み込む
             *
             * @param  _param_name　パラメータ名を格納したコンテナ
             */
            void LoadEnemyParameters(const std::vector<std::string>& _param_name);

            /**
             * @brief　 mapからエネミーパラメータを取得する
             *
             * @param  _param_name　パラメータ名（jsonに記載）
             * @return   パラメータ(float)
             */
            float GetEnemyParam(const std::string _param_name);

        private:
            //エネミーパラメータ記録用変数
            std::unordered_map<std::string, float> map_enemy_param_;
        };
    }
}