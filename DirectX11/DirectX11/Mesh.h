#pragma once
#include "framework.h"

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class Mesh
{
public:

	Mesh() = default;
	Mesh(ComPtr<ID3D11Device>& device, const vector<Vertex>& vertices, const vector<UINT>& indices);
	~Mesh() = default;

private:

	void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<Vertex>& vertices);
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices);

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	UINT indexCount = 0;

public:

	void Render(ComPtr<ID3D11DeviceContext>& devcon);

};

