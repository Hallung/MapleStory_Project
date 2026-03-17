// 카메라 View / Projection 행렬
// 모든 오브젝트의 월드 좌표를 화면 좌표로 변환할 때 사용
cbuffer ViewProjection : register(b1)
{
    matrix _view;
    matrix _projection;
}

// Sprite Atlas 정보
// 하나의 텍스처(Atlas) 안에서 개별 스프라이트의 UV를 계산하기 위해 사용
cbuffer SpriteAtlasBuffer : register(b10)
{
    float2 _textureSize; // 전체 텍스처 크기 (예: 1024 x 1024)
    float2 _spriteSize;  // Atlas 안의 개별 스프라이트 크기 (예: 32 x 32)
}

// Vertex Shader 입력 구조
struct VertexInput
{
    float4 position : POSITION0; // 정점 로컬 위치
    float2 uv : TEXCOORD0;       // 기본 UV (0~1 범위)
	
    // 인스턴싱 데이터
    matrix instWorld : INST_WORLD;   // 인스턴스별 월드 변환 행렬
    int instIndex : INST_INDEX;      // Atlas 내에서 사용할 스프라이트 인덱스
    uint instanceID : SV_InstanceID; // GPU가 제공하는 인스턴스 ID
};

// Vertex -> Pixel Shader 전달 데이터
struct PixelInput
{
    float4 position : SV_Position; // 최종 화면 좌표
    float2 uv : TEXCOORD0;         // Atlas 계산 후 UV
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    // 로컬 정점을 월드 -> 뷰 -> 프로젝션 순으로 변환
    output.position = mul(input.position, input.instWorld);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    // Atlas에서 한 줄에 몇 개의 스프라이트가 들어가는지 계산
    int maxColumns = (int) (_textureSize.x / _spriteSize.x);

    // instIndex를 이용해 Atlas 내의 row / column 계산
    int row = input.instIndex / maxColumns;
    int col = input.instIndex % maxColumns;
	
    // Atlas 전체 대비 스프라이트 하나의 UV 크기
    float2 uvStep = _spriteSize / _textureSize;
	
    // 해당 스프라이트의 시작 UV
    float2 startUV = float2(col, row) * uvStep;

    // 기본 UV(0~1)를 스프라이트 영역으로 변환
    output.uv = startUV + (input.uv * uvStep);
	
    return output;
}

// 사용할 텍스처 (Atlas)
Texture2D _srcTex : register(t0);

// 텍스처 샘플링 방식
SamplerState _samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    // Atlas에서 계산된 UV로 텍스처 샘플링
    float4 color = _srcTex.Sample(_samp, input.uv);
	
    return color;
}