#include "stdafx.h"
#include "Texture.h"
#include <algorithm>

//========================================================
// 텍스처 로드 및 GPU SRV 생성
// 파일 확장자 기반 로드 선택 (DDS / TGA / WIC)
// 필요 시 mipmap 생성
// ShaderResourceView 생성으로 셰이더 바인딩 가능 상태 준비
//========================================================
Texture::Texture(std::wstring filePath)
	:path(std::move(filePath))	// lvalue/rvalue 모두 효율적으로 이동 저장
{
	//=====================================
	// 파일 확장자 추출
	// 마지막 "." 이후 문자열을 확장자로 사용
	//=====================================
	std::wstring extension = path.substr(path.find_last_of(L".") + 1);

	//=======================================================
	// ::toupper로 확장자 대문자 통일 (사용자 입력 케이스 대응)
	// begin부터 end까지 적용하여 begin부터 넣습니다
	//=======================================================
	std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);

	HRESULT hr;

	//==============================================
	// DDS 로드 
	// GPU 친화적 포맷 (압축/큐브맵/mipmap 내장 기능)
	// 런타임 변환 없이 빠른 로딩/업로드
	// 실시간 렌더링용 텍스처에 최적
	//==============================================
	if (extension == L"DDS")
	{
		hr = DirectX::LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image);
		CHECK(hr);
	}
	//==========================================
	// TGA 로드
	// 무압축/단순 구조 이미지 포맷
	// 알파 채널 지원으로 스프라이트/마스크에 사용
	// DirectXTex 전용 로더 사용
	//==========================================
	else if (extension == L"TGA")
	{
		hr = DirectX::LoadFromTGAFile(path.c_str(), &metaData, image);
		CHECK(hr);
	}
	//===================================
	// WIC 기반 포맷 (PNG/JPG/BMP 등)
	// 범용 이미지 포맷 지원
	// mipmap이 없으므로 런타임 생성 필요
	// 일반 아트 리소스 입력용
	//===================================
	else
	{
		DirectX::ScratchImage tempImage;

		// 원본 이미지 로드
		hr = DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, tempImage);
		CHECK(hr);

		// mipmap 생성 (원본(tempImage)->mipmap 포함 image 생성)
		hr = DirectX::GenerateMipMaps(tempImage.GetImages(), tempImage.GetImageCount(), tempImage.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, image);
		CHECK(hr);

		// mipmap 생성된 메타데이터로 갱신
		metaData = image.GetMetadata();
	}

	//==========================================
	// GPU ShaderResourceView 생성
	// 셰이더에서 샘플링 가능한 텍스처 리소스 생성
	//==========================================
	hr = DirectX::CreateShaderResourceView(
		Graphics::GetInstance().GetDevice().Get(),
		image.GetImages(),
		image.GetImageCount(),
		metaData,
		&shaderResourceView
	);
	CHECK(hr);
}

//=============================
// 텍스처 바인딩
// PixelShader 슬롯에 SRV 설정
// 이후 Draw 시 해당 텍스처 사용
//=============================
void Texture::Bind(UINT slot)
{
	Graphics::GetInstance().GetDeviceContext()->PSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
}
