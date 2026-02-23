#pragma once

//====================================================
// 프로젝트에서 사용하는 모든 Scene 헤더를 모아둔 파일
// 목적:
// - Game 클래스에서 Scene 생성 시 include 관리 단순화
// - Scene 추가 시 이 파일에 include만 추가하면 사용 가능
// - Scene 관련 include 의존성을 한 곳에서 관리
//
// 사용 예:
// sceneList.push_back(std::make_shared<SandboxScene>());
//====================================================

// 테스트 / 실험용 Scene
#include "SandboxScene.h"

// 실제 게임 Scene
