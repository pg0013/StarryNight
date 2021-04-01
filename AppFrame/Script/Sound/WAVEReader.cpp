/**
 * @file	AMG_Summer_Co_Production_2020\script\WAVEReader.cpp.
 *
 * @brief	WAVEファイル読み込みクラス
 */

#include<xaudio2.h>
#include<iostream>
#include<DxLib.h>
#include"WAVEReader.h"

namespace sound
{
	WAVEReader::WAVEReader() {}
	WAVEReader::~WAVEReader() {}

	WAVEReader& WAVEReader::operator=(const WAVEReader& rhs)
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

	void WAVEReader::LoadWAVE(const char* _filename)
	{
		int filehandle;

		//DXライブラリのアーカイブ機能を使うため、DXライブラリのfileopen関数を使う
		filehandle = FileRead_open(_filename);

		//RIFFヘッダーの読み込み
		RiffHeader riff;
		FileRead_read(&riff, sizeof(riff), filehandle);

		//Formatチャンクの読み込み
		FmtChunk format;
		FileRead_read(&format, sizeof(format), filehandle);

		//Dataチャンクの読み込み
		FileRead_read(&data_, sizeof(data_), filehandle);

		//Dataチャンクのデータ部(波形データ)の読み込み
		buffer_ = (char*)malloc(data_.chunk_size);
		FileRead_read(buffer_, data_.chunk_size, filehandle);

		//波形フォーマットの設定
		memcpy(&wfx_, &format.fmt, sizeof(format.fmt));

		//WAVEFORMATにはないビット深度を計算
		wfx_.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
	}
}