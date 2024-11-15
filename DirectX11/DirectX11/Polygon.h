#pragma once

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class CPolygon
{
public:
	CPolygon() = default;
	CPolygon(ComPtr<ID3D11Device>& device, UINT vertices = 3);
	~CPolygon() = default;

private:

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	UINT indexCount = 0;

public:

	void Render(ComPtr<ID3D11DeviceContext>& devcon);
};

