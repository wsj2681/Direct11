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

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection; // 광원 방향
	float3 lightColor; // 광원 색상
	float3 ambientColor; // 환경광 색상
	int lightenable; // 조명 on/off
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

float3 CalculateLighting(float3 normal, float3 viewDir)
{
    // 광원 방향
	float3 lightDir = normalize(-lightDirection);

    // 디퓨즈 조명
	float diffuse = max(dot(normal, lightDir), 0.0f);
	float3 diffuseLight = lightColor * diffuse;

    // 환경광
	float3 ambientLight = ambientColor;

    // 스펙큘러 조명
	float3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 1.0f); // 샤이니니스 값
	float3 specularLight = lightColor * specular;

    // 최종 조명
	return ambientLight + diffuseLight + specularLight;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    // 법선 정규화
	float3 normal = normalize(input.normal);
	float3 lighting = 1;
	
	// 관찰자 방향 계산 (뷰 방향)
	float3 viewDir = normalize(-input.position.xyz);
	
	// 조명 계산
	lighting = CalculateLighting(normal, viewDir);


    // 텍스처 색상 샘플링
	float4 texColor = myTexture.Sample(mySampler, input.texcoord);

    // 조명과 텍스처를 조합한 최종 출력
	return float4(texColor.rgb * lighting, texColor.a);
}