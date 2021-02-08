/**
 * @file    UIManager.h
 * @brief  ƒQ[ƒ€“àUIŠÇ—ƒNƒ‰ƒX
 *
 * @author Takuya Fujisawa
 * @date    202102/08
 */

#pragma once
#include"appframe.h"
#include"Shoot_UI.h"

namespace starrynight
{
	namespace ui
	{
		class UIManager
		{
		public:
			UIManager();
			~UIManager();

			/**
			 * @brief   ‰Šú‰»ˆ—
			 *
			 * @return ‰Šú‰»¬Œ÷‚Ì‰Â”Û
			 */
			void Initialize();

			/**
			 * @brief   I—¹ˆ—
			 *
			 * @return I—¹ˆ—¬Œ÷‚Ì‰Â”Û
			 */
			void Terminate();

			/**
			 * @brief   ‰‰Zˆ—
			 *
			 * @return ‰‰Zˆ—¬Œ÷‚Ì‰Â”Û
			 */
			void Process();

			/**
			 * @brief   •`‰æˆ—
			 *
			 * @return •`‰æˆ—¬Œ÷‚Ì‰Â”Û
			 */
			void Render();

		public:
			Shoot_UI shoot_ui_;

		private:
			parameters::Parameters param_;
		};
	}
}
