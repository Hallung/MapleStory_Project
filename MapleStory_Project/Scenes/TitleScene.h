#pragma once
#include "Scene.h"

class TitleScene : public Scene
{
public:
	void Init() override;
	void Destroy() override;
	void Update() override;
	void Render() override;

private:
	std::shared_ptr<Object> pressEnterText;
	std::shared_ptr<class Material> pressEnterTextMaterial;

	float alpha;
};