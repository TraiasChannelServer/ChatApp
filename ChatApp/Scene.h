#pragma once

#include "ScreenComponent.h"
#include <vector>
#include <map>

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	virtual Scene* Update() = 0;
	virtual void Draw() = 0;

protected:
	// 基本の画面部品の追加・更新・描画
	void AddBaseComponent(ScreenComponent* component);
	void UpdateBase();
	void DrawBase() const;

	// 子クラス固有の画面部品の更新・描画・破棄
	void UpdateComponent(std::vector<ScreenComponent*>& Component);
	void DrawComponent(const std::vector<ScreenComponent*>& Component) const;
	void DeleteComponent(std::vector<ScreenComponent*>& Component);

private:
	// 基本画面部品
	std::vector<ScreenComponent*> m_BaseComponent;
};

inline void Scene::UpdateBase()
{
	UpdateComponent(m_BaseComponent);
}
inline void Scene::DrawBase() const
{
	DrawComponent(m_BaseComponent);
}
