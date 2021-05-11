/**
 * @file    SoundReader.cpp
 * @brief  サウンドファイル読み込み処理クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/22
 */

#include "SoundReader.h"
#include"../Utility/Utility.h"

using namespace sound;

SoundReader::SoundReader(){}

SoundReader::~SoundReader(){}

void SoundReader::Load(const char* _filename)
{
	//拡張子を取得
	std::string extension = utility::GetExtension(_filename);

	if (extension == ".wav")
	{
		LoadWav(_filename);
	}
	else if (extension == ".ogg")
	{
		LoadOgg(_filename);
	}
}

//void SoundReader::LoadStream(const char* _filename)
//{
//	//拡張子を取得
//	std::string extension = utility::GetExtension(_filename);
//
//	if (extension == ".wav")
//	{
//		LoadWavStream(_filename);
//	}
//}
