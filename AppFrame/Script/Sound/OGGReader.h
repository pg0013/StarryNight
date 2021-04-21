/**
 * @file    OGGReader.h
 * @brief  oggファイル読み込みクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/21
 */

#pragma once
#include<xaudio2.h>

namespace sound
{
	/**
	 * @brief oggファイル読み込みクラス
	 */
	class OGGReader
	{
	public:
		OGGReader();
		~OGGReader();

		/**
		 * @brief oggファイルの読み込み
		 *
		 * @param _filename
		 */
		void LoadOGG(const char* _filename);

		/**
		 * @brief	WFX構造体を返す
		 *
		 * @returns	A WAVEFORMATEX.
		 */
		WAVEFORMATEX Getwfx() const { return wfx_; }

		/**
		 * @brief	波形データの先頭アドレスを返す
		 *
		 * @returns	波形データの先頭アドレス
		 */
		char* GetpBuffer() const { return buffer_; }

	private:
		WAVEFORMATEX wfx_{};   //!< WAVEFORMATEX構造体

		char* buffer_;	//!< 波形データ部の先頭アドレス
	};
}