#pragma once
#include"DxLib.h"
#include"../Utility/Utility.h"

class ObjectBase
{
public:
	//ObjectBase();
	//~ObjectBase();

	///**
	// * @fn	virtual void ObjectBase::Initialize();
	// *
	// * @brief	初期化処理
	// *
	// */
	//virtual void Initialize();

	///**
	// * @fn	virtual void ObjectBase::Process(Game& _game);
	// *
	// * @brief	毎フレーム呼ばれる演算処理
	// *
	// * @param [in,out]	_game	ゲームクラスの参照
	// */
	//virtual void Process();

	///**
	// * @fn	virtual void ObjectBase::Render(Game& _game);
	// *
	// * @brief	描画処理
	// *
	// * @param [in,out]	_game	ゲームクラスの参照
	// */
	//virtual void Render();

private:
	VECTOR positon_;
	handle model_;
};

