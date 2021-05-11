/**
 * @file    LoadWavStream.cpp
 * @brief  wavファイルのストリーミング再生用バッファ準備
 *
 * @author Takuya Fujisawa
 * @date   2021/05/11
 */

#include"SoundReader.h"
#include"../Utility/Utility.h"

using namespace sound;

FILE* SoundReader::LoadWavStream(const char* _filename)
{
	FILE* fp = nullptr;

	//WAVEファイルを開く
	if (fopen_s(&fp, _filename, "rb") != 0)
	{
		utility::DrawDebugError("can not open wave file");
		return nullptr;
	}

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, fp);

	//Formatチャンクの読み込み
	FmtChunk format;
	fread(&format, sizeof(format), 1, fp);

	//Dataチャンクの読み込み
	fread(&data_, sizeof(data_), 1, fp);

	//波形フォーマットの設定
	memcpy(&wfx_, &format.fmt, sizeof(format.fmt));

	//WAVEFORMATにはないビット深度を計算
	wfx_.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	return fp;
}