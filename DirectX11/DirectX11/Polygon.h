#pragma once
#include "framework.h"
struct ColorVertex
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

protected:
	virtual void CreatePolygonVertex(UINT vertexCount);

private:

	void CreateVertexBuffer(ComPtr<ID3D11Device>& device);
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device);
	void CreateConstantBuffer(ComPtr<ID3D11Device>& device);
	void CreateShader(ComPtr<ID3D11Device>& device);

	// shader constant buffer
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};


	vector<ColorVertex> vertices;
	vector<UINT> indices;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	UINT indexCount = 0;

	XMMATRIX worldMatrix;
	XMFLOAT3 rotation;

public:

	void Render(ComPtr<ID3D11DeviceContext>& devcon, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
};

