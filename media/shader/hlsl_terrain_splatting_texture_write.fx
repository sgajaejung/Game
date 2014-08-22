
// -------------------------------------------------------------
// ��������
// -------------------------------------------------------------
float4x4 mWorld;
float4x4 mVP;		// ���ÿ��� �������������� ��ǥ��ȯ
float4x4 mWIT;
float4x4 mWVPT;
float3 vLightDir = {0, -1, 0};
float3 vEyePos;
float4 vFog;
float4 fogColor = {0.58823f, 0.58823f, 0.58823f, 1}; // RGB(150,150,150)
float alphaUVFactor = 8.f;

// ���� ���.
float4 I_a = {0.3f, 0.3f, 0.3f, 0.0f}; // ambient
float4 I_d = {1.f, 1.f, 1.f, 0.0f}; // diffuse
float4 I_s = {1.f, 1.f, 1.f, 0.0f}; // specular

// �ݻ���
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


// ------------------------------------------------------------
// �ؽ�ó
// ------------------------------------------------------------
texture Tex;
sampler Samp = sampler_state
{
    Texture = <Tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};


// ------------------------------------------------------------
// ���÷��� ���� �ؽ���
// ------------------------------------------------------------
texture SplattingAlphaMap;
sampler SplattingMapSamp = sampler_state
{
    Texture = <SplattingAlphaMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};


// ------------------------------------------------------------
// ���÷��� �ؽ�ó ���̾�1
// ------------------------------------------------------------
texture Tex1;
sampler Samp1 = sampler_state
{
    Texture = <Tex1>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
};

// ------------------------------------------------------------
// ���÷��� �ؽ�ó ���̾�2
// ------------------------------------------------------------
texture Tex2;
sampler Samp2 = sampler_state
{
    Texture = <Tex2>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
};

// ------------------------------------------------------------
// ���÷��� �ؽ�ó ���̾�3
// ------------------------------------------------------------
texture Tex3;
sampler Samp3 = sampler_state
{
    Texture = <Tex3>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
};


// ------------------------------------------------------------
// ���÷��� �ؽ�ó ���̾�4
// ------------------------------------------------------------
texture Tex4;
sampler Samp4 = sampler_state
{
    Texture = <Tex4>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
};



// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float3 Eye : TEXCOORD1;
	float3 N : TEXCOORD2;
};


// -------------------------------------------------------------
// 1�н�:�������̴�
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// ���� ���� ���.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // ���� ��ǥ�迡���� ����.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 1�н�:�ȼ����̴�
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	float4 Out;
	Out = tex2D(Samp, In.Tex);
    return Out;
}


// -------------------------------------------------------------
// 2�н�:�ȼ����̴�, ���÷���  (����X)
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	float4 Out;

	Out = tex2D(Samp, In.Tex);

	float4 alpha = tex2D(SplattingMapSamp, (In.Tex / alphaUVFactor));
	Out = (alpha.a * (tex2D(Samp1, In.Tex))) + ((1 - alpha.a) * Out);
	Out = (alpha.r * (tex2D(Samp2, In.Tex))) + ((1 - alpha.r) * Out);
	Out = (alpha.g * (tex2D(Samp3, In.Tex))) + ((1 - alpha.g) * Out);
	Out = (alpha.b * (tex2D(Samp4, In.Tex))) + ((1 - alpha.b) * Out);

    return Out;
}


	
// -------------------------------------------------------------
// ��ũ��
// -------------------------------------------------------------
technique TShader
{
	// �⺻ �ؽ��� ��� (���÷��� X)
	pass P0
    {
        VertexShader = compile vs_3_0 VS_pass0();
		PixelShader  = compile ps_3_0 PS_pass0();
    }


	// ���÷��� 
    pass P1
    {
        VertexShader = compile vs_3_0 VS_pass0();
		PixelShader  = compile ps_3_0 PS_pass1();
    }

}