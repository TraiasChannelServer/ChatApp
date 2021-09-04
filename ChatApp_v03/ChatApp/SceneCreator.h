#pragma once

/*
 * シーンの生成を行うクラス
 * 生成方法が複雑なシーンがあっても
 * このクラスを通すと簡単に生成できる
 */

#include "Scene.h"

class SceneCreator
{
public:
	enum class Name
	{
		CHOICE_HOST_GUEST,
		HOST,
		GUEST
	};

public:
	static Scene* Create(Name name);

private:
	SceneCreator() = delete;
};