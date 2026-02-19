#include "stdafx.h"
#include "PixelShader.h"

// HLSL 파일을 컴파일하여 PixelShader 생성
void PixelShader::Create(const std::wstring& path, const std::string& entryName)
{
	// Shader 경로 및 Entry 이름 저장
	this->path = path;
	this->entryName = entryName;

	// Shader 컴파일 (PixelShader profile 사용)
	CompileShader(this->path, this->entryName, "ps_5_0", &blob);

	// 컴파일된 Bytecode를 기반으로 GPU에 PixelShader 생성
	HRESULT hr = Graphics::GetInstance().GetDevice()->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	CHECK(hr);
}

// PixelShader 리소스 해제
void PixelShader::Clear()
{
	shader.Reset();
	blob.Reset();
}

// PixelShader를 GPU 파이프라인에 바인딩
void PixelShader::Bind()
{
	Graphics::GetInstance().GetDeviceContext()->PSSetShader(shader.Get(), nullptr, 0);
}
