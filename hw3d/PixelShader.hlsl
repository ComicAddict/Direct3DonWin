struct PS_INPUT {
	float4 vPos : SV_Position;
	float4 fCol : Color;
};

float4 main(PS_INPUT input) : SV_Target
{
	return input.fCol;
}