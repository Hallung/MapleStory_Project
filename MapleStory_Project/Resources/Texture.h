#pragma once

//==========================================
// 텍스처 리소스 클래스
// 이미지 파일 로드 및 GPU 텍스처 생성
// ShaderResourceView(SRV)로 셰이더에 바인딩
// 텍스처 경로/크기 메타데이터 제공
//==========================================
class Texture
{
public:
	//===============================
	// 텍스처 로드 및 GPU 리소스 생성
	// 파일 경로 기반 이미지 로드
	// ShaderResourceView 생성
	//===============================
	Texture(std::wstring filePath);

	// 픽셀 셰이더 슬롯에 텍스처 바인딩(slot : PS 텍스처 슬롯 인덱스)
	void Bind(UINT slot = 0);

	// 텍스처 파일 경로 반환
	std::wstring GetPath() const { return path; }
	// 텍스처 크기 반환 (width, height)
	DirectX::SimpleMath::Vector2 GetSize() const 
	{ 
		return DirectX::SimpleMath::Vector2((float)metaData.width, (float)metaData.height);
	}
	// ShaderResourceView 반환
	ComPtr<ID3D11ShaderResourceView> GetSRV() const { return shaderResourceView; }

private:
	std::wstring path;				// 텍스처 파일 경로
	DirectX::TexMetadata metaData;	// 텍스처 메타데이터(크기, 포맷 등)
	DirectX::ScratchImage image;	// 로드된 이미지 데이터
	// GPU 텍스처 리소스 뷰(셰이더에서 샘플링 가능한 SRV)
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
};