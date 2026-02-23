#pragma once

class Scene;

// 게임 전체 흐름을 관리하는 최상위 클래스
class Game
{
public:
	Game();
	~Game();

	void Init(); // 게임 초기화, Scene 생성 및 초기 Scene 설정
	void Update(); // 현재 Scene 업데이트
	void Render(); // 현재 Scene 렌더링

private:
	//=======================================
	// Scene 전환 함수
	// 기존 Scene Destroy 후 새로운 Scene Init 수행
	// index : sceneList에 등록된 Scene 인덱스
	//=======================================
	void SwitchScene(size_t index);

	std::vector<std::shared_ptr<Scene>> sceneList; // Scene 목록, Game이 소유하며 Scene 전환에 사용
	std::shared_ptr<Scene> currentScene; //현재 실행 중인 Scene
};