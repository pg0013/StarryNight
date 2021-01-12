/**
 * @file    ObjectBase.cpp
 * @brief  オブジェクトの基底クラスの処理
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include "ObjectBase.h"
using namespace starrynight::object;

ObjectBase::ObjectBase()
{
}

ObjectBase::~ObjectBase()
{
	MV1DeleteModel(handle_);
}

ObjectBase& ObjectBase::operator=(const ObjectBase& _object)
{
	handle_ = _object.handle_;
	position_ = _object.position_;
	rotation_ = _object.rotation_;

	return *this;
}

ObjectBase& ObjectBase::operator=(ObjectBase&& _object) noexcept
{
	//自分自身へmoveしない
	if (this != &_object)
	{
		handle_ = _object.handle_;
		position_ = _object.position_;
		rotation_ = _object.rotation_;
	}
	return *this;
}

void ObjectBase::Initialize()
{
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);
}

void ObjectBase::Process()
{
}

void ObjectBase::Render()
{
	MV1DrawModel(handle_);
}