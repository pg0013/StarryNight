/**
 * @file    SoundReader.h
 * @brief  音声ファイル読み込みクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/22
 */

#pragma once
#include<xaudio2.h>
#include<iostream>

namespace
{
	constexpr size_t BUFFER_SIZE = 4;
}

namespace sound
{
	class SoundReader
	{
	private:
		/**
		 * @struct	Chunk
		 *
		 * @brief	chunkデータの基本構造.
		 * @note	chunk→データの塊
		 *
		 */
		struct Chunk
		{
			char id[4];					//!< chunk毎のID
			int32_t chunk_size;	//!< チャンクサイズ
		};

		/**
		 * @struct	RiffHeader
		 *
		 * @brief	RIFFヘッダー
		 *
		 */
		struct RiffHeader
		{
			Chunk chunk;	//!< "RIFF"
			char type[4];	//!< "WAVE"
		};

		/**
		 * @struct	FmtChunk
		 *
		 * @brief	fmtチャンク
		 *
		 */
		struct FmtChunk
		{
			Chunk chunk;
			WAVEFORMAT fmt;
		};

	public:
		SoundReader();
		~SoundReader();

		SoundReader& operator=(const SoundReader& rhs)
		{
			//自己代入処理
			if (this != &rhs)
			{
				this->buffer_ = rhs.buffer_;
				this->wfx_ = rhs.wfx_;
				this->data_ = rhs.data_;
			}
			return*this;
		}

		/**
		 * @brief サウンドファイルの読み込み
		 *
		 * @param _filename サウンドファイル名（拡張子込み）
		 */
		void Load(const char* _filename);

		/**
		 * @brief wavファイルのストリーミング再生読み込み
		 *
		 * @param _filename ファイル名
		 */
		FILE* LoadWavStream(const char* _filename);

		/**
		 * @brief WFX構造体を返す
		 *
		 * @returns	A WAVEFORMATEX.
		 */
		WAVEFORMATEX Getwfx() const { return wfx_; }

		/**
		 * @brief 波形データの先頭アドレスを返す
		 *
		 * @returns	波形データの先頭アドレス
		 */
		char* GetpBuffer() const { return buffer_; }

		/**
		 * @brief 波形データを返す
		 *
		 * @returns	The data chunk.
		 */
		Chunk GetDataChunk() const { return data_; }


		char* stream_buffer_[4]; //ストリーミング再生のプライマリバッファ
	private:
		/**
		 * @brief wavファイルの読み込み
		 *
		 * @param _filename ファイル名
		 */
		void LoadWav(const char* _filename);

		/**
		 * @brief oggファイルの読み込み
		 *
		 * @param _filename ファイル名
		 */
		void LoadOgg(const char* _filename);

		WAVEFORMATEX wfx_{};   //!< WAVEFORMATEX構造体

		char* buffer_;	//!< 波形データ部の先頭アドレス

		Chunk data_;   //!< データチャンク
	};

}

