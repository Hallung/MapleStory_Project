#include "stdafx.h"
#include "Shader.h"

// HLSL 파일을 컴파일하여 Shader Bytecode 생성
void Shader::CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob)
{
	ComPtr<ID3DBlob> error;

	// HLSL 파일 컴파일
	HRESULT hr = D3DCompileFromFile
	(
		path.c_str(), // Shader 파일 경로
		nullptr,
		nullptr,
		entryName.c_str(), // Entry 함수 이름
		profile.c_str(), // Shader 타입 (vs_5_0, ps_5_0)
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		blob, // 컴파일된 Bytecode
		&error // 에러 메시지
	);
	CheckShaderError(hr, error);
}

// Shader 컴파일 에러 확인 및 출력
void Shader::CheckShaderError(HRESULT hr, const ComPtr<ID3DBlob>& error)
{
	if (FAILED(hr))
	{
		if (error.Get()) // 에러 메시지가 존재하면 출력
		{
			const std::string& str = (const char*)error->GetBufferPointer();
			MessageBoxA(nullptr, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
}
