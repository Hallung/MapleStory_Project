#pragma once

class Material
{
public:
	Material(std::wstring shaderPath, std::span<const D3D11_INPUT_ELEMENT_DESC> layoutDesc);

	void Bind();

private:

};