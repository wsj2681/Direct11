#include "framework.h"
#include "Model.h"

Model::Model(ComPtr<ID3D11Device>& device, const vector<Vertex>& vertices, const vector<UINT>& indices, const wchar_t* vsPath, const wchar_t* psPath)
{
	mesh = new Mesh(device, vertices, indices);
	shader = new Shader(device, vsPath, psPath);

	shader->SetConstantBuffer(device);
}

void Model::Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection)
{
	shader->UpdateConstantBuffer(devcon, worldMatrix, view, projection);
	shader->Bind(devcon);
	mesh->Render(devcon);
}
