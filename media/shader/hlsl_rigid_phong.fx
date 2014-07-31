
// -------------------------------------------------------------
// 전역변수
// -------------------------------------------------------------
float4x4 mWorld;
float4x4 mVP;		// 로컬에서 투영공간으로의 좌표변환
float4x4 mWIT;
float4x4 mWVPT;
float3 vLightDir = {0, -1, 0};
float3 vEyePos;
float4 vFog;
float4 fogColor = {0.58823f, 0.58823f, 0.58823f, 1}; // RGB(150,150,150)

// 광원 밝기.
float4 I_a = {0.3f, 0.3f, 0.3f, 0.0f}; // ambient
float4 I_d = {1.f, 1.f, 1.f, 0.0f}; // diffuse
float4 I_s = {1.f, 1.f, 1.f, 0.0f}; // specular

// 반사율
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


// ------------------------------------------------------------
// 텍스처
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
// 그림자맵
// ------------------------------------------------------------
texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};



// -------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
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
// 1패스:정점셰이더
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
    // 좌표변환
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// 법선 벡터 계산.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 1패스:픽셀셰이더
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	Out = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	Out = Out * tex2D(Samp, In.Tex);
	//Out = tex2D(Samp, In.Tex);
    return Out;
}


// -------------------------------------------------------------
// 1패스:정점셰이더, 포그 출력
// -------------------------------------------------------------
VS_OUTPUT VS_pass1(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
    // 좌표변환
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// 법선 벡터 계산.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
    
    return Out;
}

// -------------------------------------------------------------
// 1패스:픽셀셰이더, 포그 출력.
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	Out = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	Out = Out * tex2D(Samp, In.Tex);

	float distance = length(In.Eye);
	float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	Out = lerp(Out, fogColor, l);

    return Out;
}



// -------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
// 모델 + 그림자.
// -------------------------------------------------------------
struct VS_OUTPUT_SHADOW
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float4 TexShadow : TEXCOORD1;
	float3 Eye : TEXCOORD2;
	float3 N : TEXCOORD3;
};


// -------------------------------------------------------------
// 2패스:정점셰이더, 포그 출력, 그림자 출력.
// -------------------------------------------------------------
VS_OUTPUT_SHADOW VS_pass2(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT_SHADOW Out = (VS_OUTPUT_SHADOW)0;        // 출력데이터
    
    // 좌표변환
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// 법선 벡터 계산.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
	Out.TexShadow = mul( Pos, mWVPT );
    
    return Out;
}

// -------------------------------------------------------------
// 2패스:픽셀셰이더, 포그 출력.
// -------------------------------------------------------------
float4 PS_pass2(VS_OUTPUT_SHADOW In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	float4 Color = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	float4 decale = tex2D(Samp, In.Tex);
	Out = Color * decale;

	float4 shadow = tex2Dproj( ShadowMapSamp, In.TexShadow );
	Out = Out * saturate(Color - (0.8f*shadow));

	float distance = length(In.Eye);
	float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	Out = lerp(Out, fogColor, l);

    return Out;
}


	
// -------------------------------------------------------------
// 테크닉
// -------------------------------------------------------------
technique TShader
{

	// 퐁 셰이딩
    pass P0
    {
        VertexShader = compile vs_3_0 VS_pass0();
		PixelShader  = compile ps_3_0 PS_pass0();
    }


	// 포그 셰이딩.
    pass P1
    {
        VertexShader = compile vs_3_0 VS_pass1();
		PixelShader  = compile ps_3_0 PS_pass1();
    }


    // 모델 + 그림자 출력 셰이더
    pass P2
    {
        VertexShader = compile vs_3_0 VS_pass2();
		PixelShader  = compile ps_3_0 PS_pass2();
    }


}
