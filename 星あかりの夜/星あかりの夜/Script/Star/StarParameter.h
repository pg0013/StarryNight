/**
 * @file    StarParameter.h
 * @brief  スターパラメータを読み込み処理
 * 
 * @author Takuya Fujisawa
 * @date    202102/01
 */
#pragma once
#include"appframe.h"

namespace starrynight
{
    namespace star
    {
        /**
         * @brief  スターパラメータを読み込み処理
         */
        class StarParameter :
            public parameters::Parameters
        {
        public:
            StarParameter();
            ~StarParameter();

            /**
             * @brief ステージのスター情報を読み込む.
             * 
             * @param _stagename　ステージ名
             * @param _async_flag　非同期読み込みフラグ
             */
            void LoadStageStar(std::string _stagename, bool _async_flag = false);
        };
    }
}