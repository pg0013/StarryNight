/**
 * @file	AMG_Summer_Co_Production_2020\script\WAVEReader.h.
 *
 * @brief WAVEファイル読み込みクラス
 * @author Takuya Fujisawa
 */

#include<xaudio2.h>
#include<iostream>

namespace sound
{
	/**
	 * @class	WAVEReader
	 *
	 * @brief	WAVEファイル読み込みクラス
	 *
	 */
	class WAVEReader
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

		WAVEFORMATEX m_WFX{};   //!< WAVEFORMATEX構造体

		char* m_pBuffer;	//!< 波形データ部の先頭アドレス

		Chunk m_data;   //!< データチャンク

	public:
		WAVEReader();
		~WAVEReader();

		WAVEReader& operator=(const WAVEReader& rhs);

		/**
		 * @fn	void WAVEReader::LoadWAVE(const char* _fileName);
		 *
		 * @brief	WAVファイルの読み込み
		 *
		 * @param 	_fileName	Filename of the file.
		 */
		void LoadWAVE(const char* _fileName);

		/**
		 * @fn	WAVEFORMATEX WAVEReader::Getwfx()
		 *
		 * @brief	WFX構造体を返す
		 *
		 * @returns	A WAVEFORMATEX.
		 */
		WAVEFORMATEX Getwfx() { return m_WFX; }

		/**
		 * @fn	char* WAVEReader::GetpBuffer()
		 *
		 * @brief	波形データの先頭アドレスを返す
		 *
		 * @returns	波形データの先頭アドレス
		 */
		char* GetpBuffer() { return m_pBuffer; }

		/**
		 * @fn	Chunk WAVEReader::GetDataChunk()
		 *
		 * @brief	波形データを返す
		 *
		 * @returns	The data chunk.
		 */
		Chunk GetDataChunk() { return m_data; }
	};
}