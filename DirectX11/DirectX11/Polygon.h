#pragma once

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

//shader input


class CPolygon
{
public:
	CPolygon() = default;
	CPolygon(ComPtr<ID3D11Device>& device, UINT vertices = 3);
	~CPolygon() = default;

private:

	void CreatePolygonVertex(UINT vertexCount);
	void CreateVertexBuffer(ComPtr<ID3D11Device>& device);
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device);
	void CreateConstantBuffer(ComPtr<ID3D11Device>& device);
	void CreateShader(ComPtr<ID3D11Device>& device);

	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};


	vector<Vertex> vertices;
	vector<UINT> indices;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	UINT indexCount = 0;

	XMMATRIX worldMatrix;
public:

	void Render(ComPtr<ID3D11DeviceContext>& devcon, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
};

