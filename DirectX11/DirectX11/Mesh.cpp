#include "Mesh.h"

Mesh::~Mesh()
{
	SAFE_RESET(vertexBuffer);
	SAFE_RESET(indexBuffer);
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& devcon)
{
	UINT stride = sizeof(DiffusedVertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	devcon->DrawIndexed(indexCount, 0, 0);
}


DiffusedMesh::DiffusedMesh(ComPtr<ID3D11Device>& device, const vector<DiffusedVertex>& vertices, const vector<UINT>& indices)
{
	CreateVertexBuffer(device, vertices);
	CreateIndexBuffer(device, indices);
	indexCount = indices.size();
}

void DiffusedMesh::CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<DiffusedVertex>& vertices)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(DiffusedVertex));
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = vertices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, vertexBuffer.GetAddressOf()));
}

void DiffusedMesh::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
	bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = indices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, indexBuffer.GetAddressOf()));
}

ModelMesh::ModelMesh(ComPtr<ID3D11Device>& device, const vector<ModelVertex>& vertices, const vector<UINT>& indices)
{
	CreateVertexBuffer(device, vertices);
	CreateIndexBuffer(device, indices);
	indexCount = indices.size();
}

void ModelMesh::Render(ComPtr<ID3D11DeviceContext>& devcon)
{
	UINT stride = sizeof(ModelVertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(indexCount, 0, 0);
}

void ModelMesh::CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<ModelVertex>& vertices)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(ModelVertex));
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = vertices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, vertexBuffer.GetAddressOf()));
}

void ModelMesh::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
	bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = indices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, indexBuffer.GetAddressOf()));
}
