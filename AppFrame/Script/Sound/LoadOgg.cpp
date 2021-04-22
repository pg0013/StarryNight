/**
 * @file    LoadOgg.cpp
 * @brief  oggファイルの読み込み
 *
 * @author Takuya Fujisawa
 * @date   2021/04/22
 */

#include"SoundReader.h"
#include"vorbis/vorbisfile.h"
#include"../Utility/Utility.h"

using namespace sound;

void SoundReader::LoadOgg(const char* _filename)
{
	OggVorbis_File ov_file;

	//ファイルを開く
	int error = ov_fopen(_filename, &ov_file);

	if (error != 0)
	{
		switch (error)
		{
		case OV_EREAD: break;
		case OV_ENOTVORBIS: break;
		case OV_EVERSION: break;
		case OV_EBADHEADER: break;
		case OV_EFAULT: break;
		default: break;
		}
		utility::DrawDebugError("oggvorbis読み込みエラー"); return;
	}

	//oggファイル情報
	vorbis_info* ov_info = ::ov_info(&ov_file, -1);

	if (ov_info == nullptr)
	{
		utility::DrawDebugError("ogg情報読み込みエラー"); return;
	}

	int tmp_size = 4096;//読み込みサイズ
	int bitstream = 0;//ストリーム再生位置が返る
	char tmp_buffer[4096];

	//データサイズの読み込み
	while (1)
	{
		int read_size = ov_read(&ov_file, tmp_buffer, tmp_size,
			0,	//リトルエンディアン
			2,	//WORDは16bit
			1,	//符号あり
			&bitstream);

		if (read_size == 0) { break; }
		data_.chunk_size += read_size;
	}

	//ファイルの先頭に戻る
	ov_raw_seek(&ov_file, 0);
	//データ分のメモリを確保
	buffer_ = (char*)malloc(data_.chunk_size);
	char* read_buffer = buffer_;

	//波形データの読み込み
	while (1)
	{
		int read_size = ov_read(&ov_file, read_buffer, tmp_size,
			0,	//リトルエンディアン
			2,	//WORDは16bit
			1,	//符号あり
			&bitstream);

		if (read_size == 0) { break; }
		read_buffer += read_size;
	}

	//波形フォーマットの設定
	wfx_.wFormatTag = WAVE_FORMAT_PCM;
	wfx_.nChannels = ov_info->channels;					//チャンネル数
	wfx_.nSamplesPerSec = ov_info->rate;				//サンプリング周波数
	wfx_.wBitsPerSample = 16;								//ビット深度
	wfx_.nAvgBytesPerSec =
		ov_info->channels * ov_info->rate * 16 / 8;	//1秒間に何バイトか
	wfx_.nBlockAlign =
		ov_info->channels * 16 / 8;							//1サンプルは何バイトか

}