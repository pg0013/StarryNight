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
			this->m_pBuffer = rhs.m_pBuffer;
			this->m_WFX = rhs.m_WFX;
			this->m_data = rhs.m_data;
		}
		return*this;
	}

	void WAVEReader::LoadWAVE(const char* _fileName)
	{
		int filehandle;

		//DXライブラリのアーカイブ機能を使うため、DXライブラリのfileopen関数を使う
		filehandle = FileRead_open(_fileName);

		//RIFFヘッダーの読み込み
		RiffHeader riff;
		FileRead_read(&riff, sizeof(riff), filehandle);

		//Formatチャンクの読み込み
		FmtChunk format;
		FileRead_read(&format, sizeof(format), filehandle);

		//Dataチャンクの読み込み
		FileRead_read(&m_data, sizeof(m_data), filehandle);

		//Dataチャンクのデータ部(波形データ)の読み込み
		m_pBuffer = (char*)malloc(m_data.chunk_size);
		FileRead_read(m_pBuffer, m_data.chunk_size, filehandle);

		//波形フォーマットの設定
		memcpy(&m_WFX, &format.fmt, sizeof(format.fmt));

		//WAVEFORMATにはないビット深度を計算
		m_WFX.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
	}
}