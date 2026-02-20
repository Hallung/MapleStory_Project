#pragma once
#include "Shader.h"

// Vertex Shader를 생성하고 GPU 파이프라인의 Vertex Shader 단계에 바인딩하는 클래스
// Shader Base 클래스를 상속받아 컴파일 기능을 사용
class VertexShader : public Shader
{
public:
	void Create(const std::wstring& path, const std::string& entryName) override; // HLSL 파일을 컴파일하여 VertexShader 생성
	void Clear() override; // VertexShader 리소스 해제
	void Bind() override; // 생성된 VertexShader를 GPU 파이프라인에 바인딩

	ID3DBlob* GetBlob() const { return blob.Get(); }
	ID3D11VertexShader* GetResource() const { return shader.Get(); } 

private:
	ComPtr<ID3DBlob> blob; // 컴파일된 Shader Bytecode, InputLayout 생성 시 Vertex 구조 정보 해석에 사용
	ComPtr<ID3D11VertexShader> shader; // GPU에 생성된 VertexShader 리소스
};