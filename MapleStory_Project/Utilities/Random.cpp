#include "stdafx.h"
#include "Random.h"
#include <random>

// 랜덤 생성기는 파일 내부에서만 사용하도록 익명 네임스페이스에 정의
namespace
{
std::random_device rd; // 랜덤 seed 생성을 위한 장치
std::mt19937 gen; // Mersenne Twister 기반 난수 생성기
}

namespace Random
{
// 랜덤 생성기 초기화
// random_device를 이용해 seed를 설정
void Init()
{
	gen.seed(rd());
}

// 0.0f ~ 1.0f 범위의 float 난수 반환
// 범위가 고정되어 있으므로 distribution을 static으로 유지
float Random::GetFloat()
{
	static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	return dis(gen);
}

// 지정된 범위(min ~ max)의 float 난수 반환
// 호출마다 범위가 달라질 수 있으므로 distribution을 매번 생성
float Random::Range(float min, float max)
{
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

// 지정된 범위(min ~ max)의 int 난수 반환
// 호출마다 범위가 달라질 수 있으므로 distribution을 매번 생성
int Random::Range(int min, int max)
{
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

// 시작 인덱스(start)부터 count 개수만큼의 범위에서 랜덤 인덱스 반환
// 예: Index(9, 5) -> 9 ~ 13 사이 랜덤 값 반환
int Index(int start, int count)
{
	return Range(start, start + count - 1);
}

// 지정된 범위(min ~ max)의 Vector2 난수 반환
// x, y 각각 Range 함수를 사용하여 난수 생성
DirectX::SimpleMath::Vector2 Random::GetVector2(float min, float max)
{
	return DirectX::SimpleMath::Vector2(Range(min, max), Range(min, max));
}
}
