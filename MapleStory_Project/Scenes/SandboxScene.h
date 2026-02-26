#pragma once
#include "Scene.h"

class Player;

//=================================================================
// 엔진 기능 검증, Object 배치, 동작 실험 등을 위한 개발용 테스트 Scene
// 필요에 따라 Object 생성 및 테스트 로직 추가 예정
//=================================================================
class SandboxScene : public Scene
{
public:
	void Init() override; // Scene 초기화, 테스트할 Object 생성 및 초기 상태 설정
	void Destroy() override; // Scene 종료 처리, Scene에서 생성한 리소스 및 Object 정리

	void Update() override; // Scene 업데이트, 기본 Scene Update 호출 후 추가 테스트 로직 확장 가능
	void Render() override; // Scene 렌더링, 기본 Scene Render 호출 후 디버그 렌더링 등 확장 가능

private:
};