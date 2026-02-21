#pragma once

//==================================================================
// 셰이더 구성 모음
// 하나의 렌더링에 필요한 InputLayout, VertexShader, PixelShader 묶음
// shared_ptr로 한 이유 :
// 셰이더는 여러 객체가 공유하는 리소스
// 캐시로 수명 관리 자동화
//==================================================================
struct ShaderSet
{
	// 입력 레이아웃 (정점 구조 정의)
	std::shared_ptr<InputLayout> inputLayout;
	//버텍스 셰이더
	std::shared_ptr<VertexShader> vertexShader;
	// 픽셀 셰이더
	std::shared_ptr<PixelShader> pixelShader;
};

class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager() = default;

public:
	// 싱글턴 복사/이동 금지
	ShaderManager(const ShaderManager& other) = delete;
	ShaderManager& operator=(const ShaderManager& other) = delete;
	ShaderManager(ShaderManager&& other) = delete;
	ShaderManager& operator=(ShaderManager&& other) = delete;

	// 싱글턴 인스턴스 반환
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}

public:
	//===========================
	// 셰이더 요청
	// path 기반 셰이더 생성/조회
	// 캐시에 존재하면 재사용
	//===========================
	ShaderSet GetShader(const std::wstring& path, std::span<const D3D11_INPUT_ELEMENT_DESC> descs);

private:
	//======================================================
	// 셰이더 캐시
	// key : 셰이더 경로 , value : 생성된 ShaderSet
	// 캐싱 이유 :
	// 셰이더 컴파일 및 생성은 비용이 큼
	// 동일 셰이더를 공유할 때 중복 생성 방지 및 로딩 시간 절감
	// GPU 리소스 재사용으로 메모리 절약
	//======================================================
	std::unordered_map<std::wstring, ShaderSet> shaderCache;
};