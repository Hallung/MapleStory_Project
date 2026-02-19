#pragma once
#include "Shader.h"

// Pixel Shader를 생성하고 GPU 파이프라인의 Pixel Shader 단계에 바인딩하는 클래스
// Shader Base 클래스를 상속받아 컴파일 기능을 사용
class PixelShader : public Shader
{
public:
	void Create(const std::wstring& path, const std::string& entryName) override; // HLSL 파일을 컴파일하여 PixelShader 생성
	void Clear() override; // PixelShader 리소스 해제
	void Bind() override; // 생성된 PixelShader를 GPU 파이프라인에 바인딩

	ID3DBlob* GetBlob() const { return blob.Get(); }
	ID3D11PixelShader* GetResource() const { return shader.Get(); }

private:
	ComPtr<ID3DBlob> blob; // 컴파일된 Shader Bytecode, PixelShader 생성 시 사용
	ComPtr<ID3D11PixelShader> shader; // GPU에 생성된 PixelShader 리소스
};