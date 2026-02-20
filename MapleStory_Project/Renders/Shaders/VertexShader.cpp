#include "stdafx.h"
#include "VertexShader.h"

// HLSL 파일을 컴파일하여 VertexShader 생성
void VertexShader::Create(const std::wstring& path, const std::string& entryName)
{
	// Shader 경로 및 Entry 이름 저장
	this->path = path;
	this->entryName = entryName;

	// Shader 컴파일 (VertexShader profile 사용)
	CompileShader(path, entryName, "vs_5_0", &blob);

	// 컴파일된 Bytecode를 기반으로 GPU에 VertexShader 생성
	HRESULT hr = Graphics::GetInstance().GetDevice()->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	CHECK(hr);
}

// VertexShader 리소스 해제
void VertexShader::Clear()
{
	shader.Reset();
	blob.Reset();
}

// VertexShader를 GPU 파이프라인에 바인딩
void VertexShader::Bind()
{
	Graphics::GetInstance().GetDeviceContext()->VSSetShader(shader.Get(), nullptr, 0);
}
