cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
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
	
	output.normal = normalize(mul(input.normal, (float3x3) world));
	
	output.texcoord = float2(input.texcoord.x, 1.0 - input.texcoord.y);
	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float normalmap = myTexture.Sample(mySampler, input.texcoord).rgb;
	float3 normal = normalize(normalmap * 2.0f - 1.0f);
	float lightDir = normalize(float3(0.0f, 0.0f, -1.0f));
	float diffuse = max(dot(normal, lightDir), 0.0f);
	
	//return float4(diffuse, diffuse, diffuse, 1.0f);
	return myTexture.Sample(mySampler, input.texcoord);
	//return float4(input.texcoord.x, input.texcoord.y, 0.0f, 1.0f);
}