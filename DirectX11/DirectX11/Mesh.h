#pragma once
#include "framework.h"



class Mesh
{
public:
	Mesh() = default;
	~Mesh();

protected:

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	UINT indexCount = 0;
public:

	virtual void Render(ComPtr<ID3D11DeviceContext>& devcon);
};

struct DiffusedVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class DiffusedMesh : public Mesh
{
public:

	DiffusedMesh() = default;
	DiffusedMesh(ComPtr<ID3D11Device>& device, const vector<DiffusedVertex>& vertices, const vector<UINT>& indices);
	~DiffusedMesh() = default;

private:

	void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<DiffusedVertex>& vertices);
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices);
};

struct ModelVertex 
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

	bool operator==(const ModelVertex& other) const {
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

class ModelMesh : public Mesh
{
public:

	ModelMesh() = default;
	ModelMesh(ComPtr<ID3D11Device>& device, const vector<ModelVertex>& vertices, const vector<UINT>& indices);
	~ModelMesh() = default;

	void Render(ComPtr<ID3D11DeviceContext>& devcon) override;

private:

	void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<ModelVertex>& vertices);
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<UINT>& indices);
};