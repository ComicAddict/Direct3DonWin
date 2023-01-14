cbuffer CBuf {
	matrix transform;
};

struct VS_INPUT {
	float3 pos : Position;
	float4 col : Color;
};

struct VS_OUTPUT {
	float4 vPos : SV_Position;
	float4 fCol : Color;
};

VS_OUTPUT main(VS_INPUT v) 
{
	VS_OUTPUT output;
	output.vPos =mul( float4(v.pos, 1.0f), transform);
	output.fCol = v.col;
	return output;
}