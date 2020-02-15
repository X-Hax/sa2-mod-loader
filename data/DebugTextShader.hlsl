struct VS_IN
{
	float3 position : POSITION;
	float4 color    : COLOR0;
	float2 tex      : TEXCOORD0;
};

struct PS_IN
{
	float4 position    : POSITION;
	float4 color       : COLOR;
	float2 uv          : TEXCOORD;
};

float4 gDeviceInfo : register(c104);

PS_IN main(VS_IN input)
{
	PS_IN output;
	float2 r0;
	float resX = gDeviceInfo.x / 2;
	float resY = gDeviceInfo.y / 2;
	r0.x = -input.position.x - 0.5f;
	r0.y = -input.position.y - 0.5f;
	r0.x = (r0.x / resX) - 1;
	r0.y  = (r0.y / resY) - 1;
	output.position.x = r0.x;
	output.position.y = -r0.y;
	output.position.z = (1 + input.position.z) * r0.x;
	output.position.w = 1;
	output.color = input.color;
	output.uv = input.tex;
	return output;
}