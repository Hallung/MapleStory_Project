#pragma once

// 싱글턴 기반 시간 관리 클래스
// 프레임 시간 (deltaTime) 계산
// 누적 시간 (worldTime) 관리
// FPS 계산 및 제한 기능

class TimeManager
{
private:
	TimeManager();
	~TimeManager() = default;

public:
	// 싱글턴 복사/이동 금지
	TimeManager(const TimeManager& other) = delete;
	TimeManager& operator=(const TimeManager& other) = delete;
	TimeManager(TimeManager&& other) = delete;
	TimeManager& operator=(TimeManager&& other) = delete;

	// 싱글턴 인스턴스 반환
	static TimeManager& GetInstance()
	{
		static TimeManager instance;
		return instance;
	}

public:
	// 시간 측정용 기준 시계
	using Clock = std::chrono::steady_clock;
	// 시계 기준 시점 (타임스탬프)
	using TimePoint = std::chrono::time_point<Clock>;
	// 시간 간격 (초 단위, double로 정밀도 상승)
	using Duration = std::chrono::duration<double>;

public:
	// 프레임 시간 및 FPS 계산 갱신 (매 프레임 호출)
	void Update();

	// 이전 프레임에 소요된 시간 (초)
	float GetDeltation() const { return static_cast<float>(deltaTime); }
	// 게임 시작 이후 누적 시간 (초)
	double GetWorldTime() const { return worldTime; }
	// 1초 동안의 프레임 수
	UINT GetFPS() const { return fps; }
	
	// 목표 FPS 설정 (0이면 제한 없음)
	void SetTargetFPS(double fps);
	// 목표 프레임 시간까지 대기하여 FPS 제한
	void WaitToTargetFrameRate();

private:
	TimePoint currentTime;	// 현재 프레임 시간
	TimePoint lastTime;		// 이전 프레임 시간

	double deltaTime = 0.0;	// 프레임 간 시간 간격(초)
	double worldTime = 0.0;	// 시작 이후 누적 시간(초)

	UINT fps = 0;				// 계산된 FPS
	UINT frameCount = 0;		// FPS 계산용 프레임 카운트
	double fpsTimeElapsed = 0.0;// FPS 계산 누적 시간(초)

	Duration targetFrameTime = Duration::zero();	// 목표 프레임 시간(초)
};