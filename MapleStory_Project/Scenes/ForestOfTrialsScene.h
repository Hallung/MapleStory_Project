#pragma once
#include "Scene.h"

class ForestOfTrialsScene : public Scene
{
public:
	void Init() override;
	void Destroy() override;
	void Update() override;
	void Render() override;

	void DrawBackGroundImage();
	void DrawGuideImage();
	void SpawnMonsters();

private:
	std::shared_ptr<Object> worldPlayer;
	std::shared_ptr<Object> backGroundImage1;
	std::shared_ptr<Object> backGroundImage2;
	std::shared_ptr<Object> backGroundImage3;
	std::shared_ptr<class TileMap> tileMap;
	std::shared_ptr<class PlayEffect> playEffect;

	bool isCleared = false;
	double clearTime = 0.0;
};