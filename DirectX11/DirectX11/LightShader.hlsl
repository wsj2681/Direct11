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
	float3 lightDirection; // ���� ����
	float3 lightColor; // ���� ����
	float3 ambientColor; // ȯ�汤 ����
	int lightenable; // ���� on/off
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

float3 CalculateLighting(float3 normal, float3 viewDir)
{
    // ���� ����
	float3 lightDir = normalize(-lightDirection);

    // ��ǻ�� ����
	float diffuse = max(dot(normal, lightDir), 0.0f);
	float3 diffuseLight = lightColor * diffuse;

    // ȯ�汤
	float3 ambientLight = ambientColor;

    // ����ŧ�� ����
	float3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 1.0f); // ���̴ϴϽ� ��
	float3 specularLight = lightColor * specular;

    // ���� ����
	return ambientLight + diffuseLight + specularLight;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    // ���� ����ȭ
	float3 normal = normalize(input.normal);
	float3 lighting = 1;
	
	// ������ ���� ��� (�� ����)
	float3 viewDir = normalize(-input.position.xyz);
	
	// ���� ���
	lighting = CalculateLighting(normal, viewDir);


    // �ؽ�ó ���� ���ø�
	float4 texColor = myTexture.Sample(mySampler, input.texcoord);

    // ����� �ؽ�ó�� ������ ���� ���
	return float4(texColor.rgb * lighting, texColor.a);
}