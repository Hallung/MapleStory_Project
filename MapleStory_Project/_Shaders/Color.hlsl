// ==============================
// Vertex Shader 입력 구조체
// VertexBuffer에서 전달받는 데이터 구조
// ==============================
struct VertexInput
{
    float4 position : POSITION0; // 정점 위치
    float4 color : COLOR0; // 정점 색상
};

// ==============================
// Pixel Shader로 전달되는 구조체
// Vertex Shader 출력 → Pixel Shader 입력
// ==============================
struct PixelInput
{
    float4 position : SV_POSITION0; // 변환된 최종 정점 위치 (Clip Space)
    float4 color : COLOR0; // Pixel Shader에 전달할 색상
};

// ==============================
// World 변환 ConstantBuffer
// Object의 위치, 회전, 크기 변환
// register(b0) : ConstantBuffer Slot 0
// ==============================
cbuffer World : register(b0)
{
    matrix _world;
}

// ==============================
// View / Projection 변환 ConstantBuffer
// Camera 변환 및 Projection 변환
// register(b1) : ConstantBuffer Slot 1
// ==============================
cbuffer ViewProjection : register(b1)
{
    matrix _view;
    matrix _projection;
}

// ==============================
// Vertex Shader
// 정점 위치를 World → View → Projection 순서로 변환
// ==============================
PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    output.position = mul(input.position, _world); // World 변환
    output.position = mul(output.position, _view); // View 변환
    output.position = mul(output.position, _projection); // Projection 변환
	
    output.color = input.color; // 색상 전달
	
    return output;
}

// ==============================
// Pixel Shader
// 전달받은 색상을 그대로 출력
// ==============================
float4 PS(PixelInput input) : SV_Target0
{
    return input.color;
}