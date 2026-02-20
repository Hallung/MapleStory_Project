#pragma once

// VertexShader, PixelShader의 공통 기능을 제공하는 Shader 기본 클래스
// Shader 컴파일과 에러 처리를 담당
class Shader
{
public:
	virtual void Create(const std::wstring& path, const std::string& entryName) = 0; // Shader 생성 (각 Shader 타입에서 구현)
	virtual void Clear() = 0; // Shader 해제
	virtual void Bind() = 0; // Shader를 GPU 파이프라인에 바인딩

protected:
	// HLSL 파일을 컴파일하여 Bytecode 생성
	void CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob);

private:
	// Shader 컴파일 에러 발생 시 메시지 출력
	void CheckShaderError(HRESULT hr, const ComPtr<ID3DBlob>& error);

protected:
	std::wstring path = L""; // Shader 파일 경로
	std::string entryName = ""; // Shader Entry 함수 이름
};