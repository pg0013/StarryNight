/** 
 * @file    ModeBase.cpp
 * @brief  モードの基底クラス
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */
#include "ModeBase.h"

namespace mode
{
	ModeBase::ModeBase()
	{
		modename_ = "";
		uid_ = 1;
		layer_ = 0;

		mode_count_ = 0;
		mode_time_ = 0;
		step_time_ = 0;
		modebase_time_ = 0;
		pause_base_time_ = 0;
		pause_step_time_ = 0;
		old_frame_time_ = 0;

		SetCallPerFrame(1);
		SetCallOfCount(1);
	}

	ModeBase::~ModeBase(){}

	bool ModeBase::Initialize()
	{
		return true;
	}
	
	bool ModeBase::Terminate()
	{
		return true;
	}

	bool ModeBase::Process()
	{
		return true;
	}

	bool ModeBase::Render()
	{
		return true;
	}

	void ModeBase::StepTime(unsigned long _nowtime)
	{
		//時間経過処理
		if (mode_count_ == 0)
		{
			mode_time_ = 0;
			step_time_ = 0;
			modebase_time_ = _nowtime;
			pause_base_time_ = 0;
			pause_step_time_ = 0;
		}
		else
		{
			mode_time_ = _nowtime - modebase_time_ + pause_step_time_;
			step_time_ = _nowtime - old_frame_time_;
		}
		old_frame_time_ = _nowtime;
	}

	void ModeBase::StepCount()
	{
		mode_count_++;
	}
}