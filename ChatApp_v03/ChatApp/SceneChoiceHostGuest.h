#pragma once

/*
 * ホストかゲストかを選ぶ画面
 */

#include "Scene.h"

class SceneChoiceHostGuest : public Scene
{
public:
	SceneChoiceHostGuest();

public:
	Scene* Update() override;
	void Draw() override;

private:
	// コールバック関数
	void BeHost();
	void BeGuest();
	void End();

private:
	Scene* m_Next;
};

