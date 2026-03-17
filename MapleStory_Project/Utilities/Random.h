#pragma once

// 랜덤 값 생성을 위한 유틸리티 네임스페이스
// 엔진 전역에서 동일한 랜덤 생성기를 사용하기 위해 별도 모듈로 분리
namespace Random
{
// 랜덤 생성기 초기화, 프로그램 시작 시 한 번 호출하여 seed 설정
void Init();

// 0.0f ~ 1.0f 범위의 float 난수 반환
float GetFloat();

// 지정된 범위(min ~ max)의 float 난수 반환
float Range(float min, float max);

// 지정된 범위(min ~ max)의 int 난수 반환
int Range(int min, int max);

// 시작 인덱스(start)부터 count 개수만큼의 범위에서 랜덤 인덱스 반환
int Index(int start, int count);

// 지정된 범위(min ~ max)의 Vector2 난수 반환 (x, y 각각 동일 범위에서 난수 생성)
DirectX::SimpleMath::Vector2 GetVector2(float min, float max);
}