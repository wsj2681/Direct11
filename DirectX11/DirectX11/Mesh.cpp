#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device>& device, const vector<Vertex>& vertices, const vector<UINT>& indices)
{
	CreateVertexBuffer(device, vertices);
	CreateIndexBuffer(device, indices);
	indexCount = indices.size();
}

void Mesh::CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<Vertex>& vertices)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = vertices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, vertexBuffer.GetAddressOf()));
}

void Mesh::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));;
	bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = indices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, indexBuffer.GetAddressOf()));
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& devcon)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(indexCount, 0, 0);
}
