#include "stdafx.h"
#include "TileMap.h"

#include "tinyxml2.h"
#include <filesystem>

#include "Renders/ConstantBuffers/GlobalBuffers.h"
#include "Renders/IA/InstanceBuffer.h"
#include "Utilities/ObjectFactory.h"
#include "Resources/VertexType.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"

TileMap::TileMap(UINT width, UINT height, float tileSize, const std::wstring& tileSetPath, UINT tileCols, UINT tileRows)
	: width(width), height(height), tileSize(tileSize)
{
	// width * height 만큼 타일 공간을 미리 확보
	// 모든 타일은 기본값(textureIndex = -1) 상태로 초기화됨
	tiles.resize(width * height);

	//==============================
	// Instance Rendering 준비
	//==============================

	// 최대 타일 수만큼 InstanceBuffer 생성
	instanceBuffer = std::make_shared<InstanceBuffer>();
	instanceBuffer->Create(width * height, sizeof(VertexInstancing));

	// CPU에서 사용할 인스턴스 데이터 공간 확보
	instanceData.reserve(width * height);

	//==============================
	// 타일셋 텍스처 로드
	//==============================

	std::shared_ptr<Texture> tileTexture = TextureManager::GetInstance().LoadTexture(tileSetPath);
	// 실제 텍스처 해상도
	DirectX::SimpleMath::Vector2 actualTexSize = tileTexture->GetSize();

	//==============================
	// Atlas에서 개별 Sprite 크기 계산
	//==============================

	DirectX::SimpleMath::Vector2 sliceSize;

	// 타일셋을 tileCols x tileRows 형태로 분할
	sliceSize.x = actualTexSize.x / (float)tileCols;
	sliceSize.y = actualTexSize.y / (float)tileRows;

	//==============================
	// Sprite Atlas ConstantBuffer 설정
	//==============================

	spriteAtlasBuffer = std::make_shared<SpriteAtlasBuffer>();

	// Atlas 전체 크기와 Sprite 단위 크기를 Shader에 전달
	spriteAtlasBuffer->SetData(actualTexSize, sliceSize);
	spriteAtlasBuffer->Update();

	//==============================
	// 타일 렌더링용 Quad Mesh 생성
	//==============================

	quadMesh = GeometryHelper::CreateTexturedQuad();

	//==============================
	// InputLayout 구성
	// Vertex + Instance 데이터 결합
	//==============================

	std::vector<D3D11_INPUT_ELEMENT_DESC> combinedDescs = VertexTexture::descs;

	// Instancing 데이터(InputLayout) 추가
	combinedDescs.insert(combinedDescs.end(), VertexInstancing::descs.begin(), VertexInstancing::descs.end());

	//==============================
	// Sprite Instancing Material 생성
	//==============================

	material = make_shared<Material>(L"_Shaders/SpriteInstancing.hlsl", combinedDescs);

	// 타일셋 텍스처 설정
	material->SetTexture(tileTexture);
}

void TileMap::Render()
{
	// 타일 데이터가 변경되었으면 인스턴스 데이터 재생성
	UpdateInstances();

	// 그릴 타일이 없으면 렌더링 생략
	if (instanceData.empty()) return;

	quadMesh->Bind(); // Quad Mesh 바인딩
	instanceBuffer->Bind(); // InstanceBuffer 바인딩
	material->Bind(); // Material (Shader + Texture) 바인딩
	spriteAtlasBuffer->BindVS(10); // Sprite Atlas ConstantBuffer 바인딩

	// 삼각형 리스트 토폴로지 설정
	Graphics::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Instanced Draw 호출
	Graphics::GetInstance().GetDeviceContext()->DrawIndexedInstanced(quadMesh->GetIndexBuffer()->GetCount(), (UINT)instanceData.size(), 0, 0, 0);
}

DirectX::SimpleMath::Vector2 TileMap::WorldToGrid(DirectX::SimpleMath::Vector2 worldPos) const
{
	// 월드 좌표를 타일 크기로 나누어 몇 번째 칸인지 계산
	// 음수 좌표에서도 정확한 그리드 인덱스를 얻기 위해 floor 사용
	int x = (int)floor(worldPos.x / tileSize);
	int y = (int)floor(worldPos.y / tileSize);

	return DirectX::SimpleMath::Vector2((float)x, (float)y);
}

DirectX::SimpleMath::Vector2 TileMap::GridToWorld(int gridX, int gridY) const
{
	// 그리드 인덱스를 다시 월드 좌표로 변환
	// tileSize를 곱해서 해당 타일의 시작 위치 계산
	float x = gridX * tileSize;
	float y = gridY * tileSize;

	// 타일 중앙 위치를 반환하기 위해 0.5 * tileSize 만큼 보정
	return DirectX::SimpleMath::Vector2(x + tileSize * 0.5f, y + tileSize * 0.5f);
}

bool TileMap::IsValidGrid(int gridX, int gridY) const
{
	// 가로 범위 검사
	if (gridX < 0 || gridX >= (int)width) return false;
	// 세로 범위 검사
	if (gridY < 0 || gridY >= (int)height) return false;

	return true;
}

void TileMap::SetTile(int gridX, int gridY, int textureIndex)
{
	// 범위를 벗어나면 무시
	if (IsValidGrid(gridX, gridY) == false) return;

	// 2차원 인덱스를 1차원 배열 인덱스로 변환
	int index = gridY * width + gridX;

	// 타일 텍스처 설정
	tiles[index].textureIndex = textureIndex;
	// textureIndex가 0 이상이면 충돌 타일로 간주
	tiles[index].bSolid = (textureIndex >= 0);

	bDirty = true;
}

TileInfo* TileMap::GetTile(int gridX, int gridY)
{
	// 유효하지 않은 위치 접근 방지
	if (IsValidGrid(gridX, gridY) == false) return nullptr;

	// 내부 타일 데이터의 주소 반환
	return &tiles[gridY * width + gridX];
}

const TileInfo* TileMap::GetTile(int gridX, int gridY) const
{
	// 유효하지 않은 위치 접근 방지
	if (IsValidGrid(gridX, gridY) == false) return nullptr;

	// 내부 타일 데이터의 주소 반환
	return &tiles[gridY * width + gridX];
}

// 현재 타일맵 상태를 XML 파일로 저장, 맵 크기 정보와 배치된 타일 정보만 기록
void TileMap::Save(const std::wstring& path)
{
	tinyxml2::XMLDocument doc;

	// 루트 노드 Map 생성
	tinyxml2::XMLElement* root = doc.NewElement("Map");
	doc.InsertFirstChild(root);

	// 맵 기본 정보 저장 (가로, 세로, 타일 크기)
	tinyxml2::XMLElement* sizeNode = doc.NewElement("Size");
	sizeNode->SetAttribute("width", width);
	sizeNode->SetAttribute("height", height);
	sizeNode->SetAttribute("tileSize", tileSize);
	root->InsertEndChild(sizeNode);

	// 전체 타일을 순회하면서 배치된 타일만 저장
	for (UINT y = 0; y < height; ++y)
	{
		for (UINT x = 0; x < width; ++x)
		{
			int index = y * width + x;

			// textureIndex >= 0 인 경우만 저장 (비어있는 타일은 제외)
			if (tiles[index].textureIndex >= 0)
			{
				tinyxml2::XMLElement* tileNode = doc.NewElement("Tile");
				tileNode->SetAttribute("x", x);
				tileNode->SetAttribute("y", y);
				tileNode->SetAttribute("index", tiles[index].textureIndex);
				tileNode->SetAttribute("solid", tiles[index].bSolid);

				root->InsertEndChild(tileNode);
			}
		}
	}

	// 유니코드(한글 등) 경로 대응을 위해 std::filesystem으로 경로 처리
	std::filesystem::path savePath(path);
	doc.SaveFile(savePath.string().c_str());
}

// XML 파일로부터 타일맵 데이터를 불러옴, 기존 타일 데이터는 초기화 후 새로 구성
void TileMap::Load(const std::wstring& path)
{
	tinyxml2::XMLDocument doc;

	// 유니코드(한글 등) 경로 대응을 위해 std::filesystem으로 경로 처리
	std::filesystem::path loadPath(path);

	// 파일 로드 실패 시 더 이상 진행하지 않음 (파일이 없거나 XML 형식이 잘못된 경우)
	if (doc.LoadFile(loadPath.string().c_str()) != tinyxml2::XML_SUCCESS) return;

	// 루트 노드 Map 확인
	// 정상적인 타일맵 파일이 아닐 경우 안전하게 종료
	tinyxml2::XMLElement* root = doc.FirstChildElement("Map");
	if (root == nullptr) return;

	// 맵 기본 정보 읽기
	tinyxml2::XMLElement* sizeNode = root->FirstChildElement("Size");
	if (sizeNode)
	{
		width = sizeNode->UnsignedAttribute("width");
		height = sizeNode->UnsignedAttribute("height");
		tileSize = sizeNode->FloatAttribute("tileSize");

		// 기존 타일 데이터 초기화 후 새 크기로 재할당
		tiles.clear();
		tiles.resize(width * height);

		// InstanceBuffer 크기 재설정
		instanceBuffer->Create(width * height, sizeof(VertexInstancing));
	}

	// 저장된 타일 노드를 순회하며 복원
	tinyxml2::XMLElement* tileNode = root->FirstChildElement("Tile");

	while (tileNode != nullptr)
	{
		int x = tileNode->IntAttribute("x");
		int y = tileNode->IntAttribute("y");
		int texIndex = tileNode->IntAttribute("index");
		bool bSolid = tileNode->BoolAttribute("solid");

		// 유효 범위 검사
		if (IsValidGrid(x, y))
		{
			int index = y * width + x;
			tiles[index].textureIndex = texIndex;
			tiles[index].bSolid = bSolid;
		}

		tileNode = tileNode->NextSiblingElement("Tile");
	}

	bDirty = true;
}

void TileMap::UpdateInstances()
{
	// 타일 데이터가 변경되지 않았다면 업데이트 생략
	if (!bDirty) return;

	// CPU 인스턴스 데이터 초기화
	instanceData.clear();

	// 전체 타일 순회
	for (UINT y = 0; y < height; ++y)
	{
		for (UINT x = 0; x < width; ++x)
		{
			const TileInfo* tile = GetTile(x, y);

			// 비어있는 타일은 렌더링하지 않음
			if (tile != nullptr && tile->textureIndex >= 0)
			{
				VertexInstancing inst;
				// 타일 중심 월드 좌표 계산
				DirectX::SimpleMath::Vector2 worldPos = GridToWorld(x, y);

				// 타일 크기만큼 스케일
				DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScaling(tileSize, tileSize, 1.0f);
				// 타일 위치로 이동
				DirectX::SimpleMath::Matrix T = XMMatrixTranslationFromVector(worldPos);

				// 최종 월드 행렬 계산
				inst.world = XMMatrixTranspose(S * T);
				// Atlas에서 사용할 텍스처 인덱스
				inst.textureIndex = tile->textureIndex;

				// CPU 인스턴스 배열에 추가
				instanceData.push_back(inst);
			}
		}
	}

	// 실제로 렌더링할 인스턴스 개수
	UINT drawCount = (UINT)instanceData.size();

	// CPU에서 생성한 인스턴스 데이터를 GPU InstanceBuffer에 업로드
	if (drawCount > 0)
	{
		instanceBuffer->Update(instanceData.data(), drawCount);
	}

	bDirty = false;
}
