/**
 * @file    ObjectBase.cpp
 * @brief  オブジェクトの基底クラスの処理
 * 
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include "ObjectBase.h"
using namespace starrynight;

ObjectBase::ObjectBase()
{

}

ObjectBase::~ObjectBase()
{

}

void ObjectBase::Initialize()
{

}

void ObjectBase::Process()
{

}

void ObjectBase::Render()
{
	MV1DrawModel(handle_);
}