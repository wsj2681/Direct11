#include "TextureShader.h"

TextureShader::TextureShader(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, const string& vsPath, const string& psPath, const string& textureFile)
{
	// DiffusedVertex Shader
	ComPtr<ID3DBlob> vsBlob;
	HR(D3DCompileFromFile(TOWSTRING(vsPath), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), nullptr));
	HR(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// Pixel Shader
	ComPtr<ID3DBlob> psBlob;
	HR(D3DCompileFromFile(TOWSTRING(psPath), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, psBlob.GetAddressOf(), nullptr));
	HR(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));

	LoadTexture(device, textureFile);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HR(device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inputLayout.GetAddressOf()));


	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HR(device->CreateSamplerState(&samplerDesc, samplerstate.GetAddressOf()));
	devcon->PSSetSamplers(0, 1, samplerstate.GetAddressOf());
}

TextureShader::~TextureShader()
{
	SAFE_RESET(textureView);
	SAFE_RESET(samplerstate);
}

void TextureShader::SetConstantBuffer(ComPtr<ID3D11Device>& device)
{
	Shader::SetConstantBuffer(device);
}

void TextureShader::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection)
{
	Shader::UpdateConstantBuffer(devcon, world, view, projection);
}

void TextureShader::Bind(ComPtr<ID3D11DeviceContext>& devcon)
{
	Shader::Bind(devcon);
	devcon->PSSetShaderResources(0, 1, textureView.GetAddressOf());
	devcon->PSSetSamplers(0, 1, samplerstate.GetAddressOf());
}

void TextureShader::LoadTexture(ComPtr<ID3D11Device>& device, const string& textureFile)
{
	// WIC 팩토리 생성
	ComPtr<IWICImagingFactory> wicFactory;
	HR(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)));

	// 이미지 디코더 생성
	ComPtr<IWICBitmapDecoder> decoder;
	HR(wicFactory->CreateDecoderFromFilename(TOWSTRING(textureFile), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder));

	// 첫 번째 프레임 로드
	ComPtr<IWICBitmapFrameDecode> frame;
	HR(decoder->GetFrame(0, &frame));

	// 비트맵 형식 변환기 생성
	ComPtr<IWICFormatConverter> formatConverter;
	HR(wicFactory->CreateFormatConverter(&formatConverter));

	// 픽셀 포맷을 32비트 RGBA로 변환
	HR(formatConverter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr, 0.0,
		WICBitmapPaletteTypeCustom));

	// 이미지 크기 가져오기
	UINT width, height;
	HR(formatConverter->GetSize(&width, &height));

	// 텍스처 데이터 생성
	vector<BYTE> textureData(width * height * 4);
	HR(formatConverter->CopyPixels(
		nullptr,
		width * 4,                 // 한 행의 바이트 수
		static_cast<UINT>(textureData.size()),
		textureData.data()));

	// Direct3D 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = textureData.data();
	initData.SysMemPitch = width * 4;

	ComPtr<ID3D11Texture2D> texture;
	HR(device->CreateTexture2D(&textureDesc, &initData, &texture));

	// Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	HR(device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf()));
}
