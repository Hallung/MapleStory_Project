// ==============================
// Vertex Shader 입력 구조체
// ==============================
struct VertexInput
{
    float4 position : POSITION0; // 정점 위치
    float2 uv : TEXCOORD0; // 텍스처 UV 좌표
};

// ==============================
// Pixel Shader로 전달되는 구조체
// ==============================
struct PixelInput
{
    float4 position : SV_POSITION0; // 변환된 정점 위치
    float2 uv : TEXCOORD0; // Pixel Shader에 전달할 UV
};

// ==============================
// World 변환 ConstantBuffer
// register(b0)
// ==============================
cbuffer World : register(b0)
{
    matrix _world;
}

// ==============================
// View / Projection ConstantBuffer
// register(b1)
// ==============================
cbuffer ViewProjection : register(b1)
{
    matrix _view;
    matrix _projection;
}

// ==============================
// Sprite Frame ConstantBuffer
// SpriteSheet에서 사용할 UV 영역 설정
// register(b10)
// ==============================
cbuffer FrameBuffer : register(b2)
{
    float2 _startUV; // 시작 UV
    float2 _sizeUV; // UV 크기
}

// ==============================
// Vertex Shader
// 위치 변환 및 UV 좌표 계산
// ==============================
PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    // 위치 변환
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    
    // SpriteSheet Frame UV 계산
    output.uv = _startUV + (input.uv * _sizeUV);
    
    return output;
}

// ==============================
// Color ConstantBuffer
// Sprite 색상 Tint 처리
// register(b0)
// ==============================
cbuffer ColorData : register(b0)
{
    float4 _color;
}

// ==============================
// Texture Sampler
// ==============================
SamplerState samp : register(s0);

// ==============================
// Texture Resource
// ==============================
Texture2D srcTex : register(t0);

// ==============================
// Pixel Shader
// Texture 샘플링 후 색상 적용
// ==============================
float4 PS(PixelInput input) : SV_Target0
{
    float4 color = srcTex.Sample(samp, input.uv); // Texture 색상 샘플링
    
    // Texture 색상에 Tint 색상 적용
    return color * _color;
}