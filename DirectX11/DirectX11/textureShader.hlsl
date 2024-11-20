cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VSInput
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

PSInput VSMain(VSInput input)
{
	PSInput output;
	float4 pos = float4(input.position, 1.0f);
	output.position = mul(pos, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.texcoord = float2(input.texcoord.x, 1.0f - input.texcoord.y);
	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return myTexture.Sample(mySampler, input.texcoord);
}