#pragma once
#include "framework.h"

struct DiffusedVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct ModelVertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

	bool operator==(const ModelVertex& other) const
	{
		return position.x == other.position.x &&
			position.y == other.position.y &&
			position.z == other.position.z &&
			normal.x == other.normal.x &&
			normal.y == other.normal.y &&
			normal.z == other.normal.z &&
			uv.x == other.uv.x &&
			uv.y == other.uv.y;
	}
};

template <typename TVertex>
class Mesh
{
public:
	Mesh() = default;
	Mesh(ComPtr<ID3D11Device>& device, const vector<TVertex>& vertices, const vector<UINT>& indices)
	{
		CreateVertexBuffer(device, vertices);
		CreateIndexBuffer(device, indices);
		indexCount = indices.size();
	}
	~Mesh() = default;

protected:
	void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<TVertex>& vertices)
	{
		D3D11_BUFFER_DESC bufferdesc = {};
		bufferdesc.Usage = D3D11_USAGE_DEFAULT;
		bufferdesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(TVertex));
		bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferdesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initdata = {};
		initdata.pSysMem = vertices.data();

		HR(device->CreateBuffer(&bufferdesc, &initdata, vertexBuffer.GetAddressOf()));
	}
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices)
	{
		D3D11_BUFFER_DESC bufferdesc = {};
		bufferdesc.Usage = D3D11_USAGE_DEFAULT;
		bufferdesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
		bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA initdata = {};
		initdata.pSysMem = indices.data();

		HR(device->CreateBuffer(&bufferdesc, &initdata, indexBuffer.GetAddressOf()));
	}

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	size_t indexCount = 0;

public:

	void Render(ComPtr<ID3D11DeviceContext>& devcon)
	{
		UINT stride = sizeof(TVertex);
		UINT offset = 0;
		devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devcon->DrawIndexed(indexCount, 0, 0);
	}
};