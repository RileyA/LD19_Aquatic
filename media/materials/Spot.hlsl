

void amb_vp(float4 position	: POSITION,
			 float2 uv			: TEXCOORD0,
			 out float4 oPosition    : POSITION,
			 out float2 oUv			 : TEXCOORD0,
			 out float4 oPos		 : TEXCOORD1,
			 uniform float4x4 worldViewProj)
{
	// calculate output position
	oPosition = mul(worldViewProj, position);
	oPos = position;
	// pass the main uvs straight through unchanged
	oUv = uv;
}

// point light frag shader

void amb_fp( float2 uv			: TEXCOORD0,
			float4 pos			: TEXCOORD1,

			  out float4 colour	: COLOR,
			  
			  uniform float4 camPos,
			  
			  uniform sampler2D   diffuseMap : register(s0),
			  uniform sampler2D   casuticMap : register(s1))
{
	float distance = length(pos-camPos);
	float ler = saturate((distance-5.0)/15.0);
	float3 caustic = tex2D(casuticMap,uv.xy*5).xyz;
	colour = (1-ler)*0.38*float4((tex2D(diffuseMap,uv.xy*5).xyz) + caustic,1)+ler*float4(0.09,0.1978,0.204,1);
	//colour = (1-ler)*float4((tex2D(diffuseMap,uv.xy).xyz*caustic) +caustic*0.2,1)+ler*float4(1,0.1978,0.204,1);
}



//point lights

void main_vp(float4 position	: POSITION,
			 float3 normal		: NORMAL,
			 float2 uv			: TEXCOORD0,

			 // outputs
			 out float4 oPosition    : POSITION,
			 out float2 oUv			 : TEXCOORD0,
			 out float3 lightDir	 : TEXCOORD1,
			 out float3 onormal	 : TEXCOORD2,
			 out float4 oPos	 : TEXCOORD3,
			 // parameters
			 uniform float4 lightPosition, // object space
			 uniform float4x4 worldViewProj)
{
	// calculate output position
	oPosition = mul(worldViewProj, position);
	
	oPos = position;

	// pass the main uvs straight through unchanged
	oUv = uv;
	
	// object space normal
	onormal = normal;
	
	// object space light dir
	lightDir = lightPosition.xyz - position;
	
	
}

// point light frag shader

void main_fp( float2 uv			: TEXCOORD0,
			  float3 lightDir : TEXCOORD1,
			  float3 normal : TEXCOORD2,
			  float4 pos : TEXCOORD3,

			  out float4 colour	: COLOR,

			  uniform float4 lightDiffuse,
			  uniform float4 lightAttenuation,
			  uniform float4 camPos,
			  uniform sampler2D   diffuseMap : register(s0),
			  uniform sampler2D   casuticMap : register(s1))
{
	float ler = saturate((length(pos-camPos)-5.0)/45.0);
	float rng = length(lightDir);
	if(rng>lightAttenuation.x)
	{
		discard;
		return;
	}
	float attenuation = saturate((lightAttenuation.x-rng)/lightAttenuation.x);
	float3 lightVec = lightDir/rng;
	float3 lightValue = 0;
	colour = (1-ler)*float4(lightDiffuse.xyz * attenuation * (tex2D(diffuseMap,uv.xy*5).xyz+tex2D(casuticMap,uv.xy*5).xyz*2) * dot(normal, lightVec),1);
	//colour = colour*(1-ler);
	//colour = float4(1,0,0,1);
}

// spot lights

void spot_vp(float4 position	: POSITION,
			 float3 normal		: NORMAL,
			 float2 uv			: TEXCOORD0,

			 // outputs
			 out float4 oPosition    : POSITION,
			 out float2 oUv			 : TEXCOORD0,
			 out float3 lightDir	 : TEXCOORD1,
			 out float3 onormal	 : TEXCOORD2,
			 out float4 oPos	 : TEXCOORD3,
			 // parameters
			 uniform float4 lightPosition, // object space
			 uniform float4x4 worldViewProj)
{
	// calculate output position
	oPosition = mul(worldViewProj, position);
	
	oPos = position;

	// pass the main uvs straight through unchanged
	oUv = uv;
	
	// object space normal
	onormal = normal;
	
	// object space light dir
	lightDir = lightPosition.xyz - position;
	
	
}

// point light frag shader

void spot_fp( float2 uv			: TEXCOORD0,
			  float3 lightDir : TEXCOORD1,
			  float3 normal : TEXCOORD2,
			  float4 pos : TEXCOORD3,

			  out float4 colour	: COLOR,

			  uniform float4 lightDiffuse,
			  uniform float4 lightAttenuation,
			  uniform float4 camPos,
			  uniform float4 spotParams,
			  uniform float4 spotDir,
			  uniform sampler2D   diffuseMap : register(s0),
			  uniform sampler2D   casuticMap : register(s1))
{
	float ler = saturate((length(pos-camPos)-5.0)/(30.0+lightAttenuation.x-42));
	float rng = length(lightDir);
	if(rng>lightAttenuation.x)
	{
		discard;
		return;
	}
	
	float3 lightVec = lightDir/rng;
	float3 lightValue = 0;
	
	float spotFactor = 0;
    float rho = saturate(dot((-spotDir.xyz),lightVec.xyz ));
    spotFactor = pow(saturate(rho - spotParams.y) / (spotParams.x - spotParams.y), spotParams.z);
    spotFactor = saturate(spotFactor);
	
	
	float attenuation = saturate((lightAttenuation.x-rng)/lightAttenuation.x);
	colour = (1-ler)*float4(lightDiffuse.xyz * spotFactor * attenuation * (tex2D(diffuseMap,uv.xy*5).xyz+tex2D(casuticMap,uv.xy*5).xyz*2) * dot(normal, lightVec),1);
	//colour = colour*(1-ler);
	//colour = float4(1,0,0,1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void amb_vp2(float4 position	: POSITION,
			 float2 uv			: TEXCOORD0,
			 float2 uv2			: TEXCOORD1,
			 out float4 oPosition    : POSITION,
			 out float4 oUv			 : TEXCOORD0,
			 out float4 oPos		 : TEXCOORD1,
			 uniform float4x4 worldViewProj)
{
	// calculate output position
	oPosition = mul(worldViewProj, position);
	oPos = position;
	// pass the main uvs straight through unchanged
	oUv.xy = uv;
	oUv.zw = uv2;
}

// point light frag shader

void amb_fp2( float4 uv			: TEXCOORD0,
			float4 pos			: TEXCOORD1,

			  out float4 colour	: COLOR,
			  
			  uniform float4 camPos,
			  
			  uniform sampler2D   diffuseMap : register(s0),
			  uniform sampler2D   casuticMap : register(s1))
{
	float distance = length(pos-camPos);
	float ler = saturate((distance-5.0)/15.0);
	float3 caustic = tex2D(casuticMap,uv.zw*5).xyz;
	colour = (1-ler)*0.38*float4((tex2D(diffuseMap,uv.xy).xyz) + caustic,1)+ler*float4(0.09,0.1978,0.204,1);
	//colour = (1-ler)*float4((tex2D(diffuseMap,uv.xy).xyz*caustic) +caustic*0.2,1)+ler*float4(1,0.1978,0.204,1);
}



//point lights

void main_vp2(float4 position	: POSITION,
			 float3 normal		: NORMAL,
			 float2 uv			: TEXCOORD0,
			 float2 uv2			: TEXCOORD1,

			 // outputs
			 out float4 oPosition    : POSITION,
			 out float4 oUv			 : TEXCOORD0,
			 out float3 lightDir	 : TEXCOORD1,
			 out float3 onormal	 : TEXCOORD2,
			 out float4 oPos	 : TEXCOORD3,
			 // parameters
			 uniform float4 lightPosition, // object space
			 uniform float4x4 worldViewProj)
{
	// calculate output position
	oPosition = mul(worldViewProj, position);
	
	oPos = position;

	// pass the main uvs straight through unchanged
	oUv.xy = uv;
	oUv.zw = uv2;
	
	// object space normal
	onormal = normal;
	
	// object space light dir
	lightDir = lightPosition.xyz - position;
	
	
}

// point light frag shader

void main_fp2( float4 uv			: TEXCOORD0,
			  float3 lightDir : TEXCOORD1,
			  float3 normal : TEXCOORD2,
			  float4 pos : TEXCOORD3,

			  out float4 colour	: COLOR,

			  uniform float4 lightDiffuse,
			  uniform float4 lightAttenuation,
			  uniform float4 camPos,
			  uniform sampler2D   diffuseMap : register(s0),
			  uniform sampler2D   casuticMap : register(s1))
{
	float ler = saturate((length(pos-camPos)-5.0)/45.0);
	float rng = length(lightDir);
	if(rng>lightAttenuation.x)
	{
		discard;
		return;
	}
	float attenuation = saturate((lightAttenuation.x-rng)/lightAttenuation.x);
	float3 lightVec = lightDir/rng;
	float3 lightValue = 0;
	colour = (1-ler)*float4(lightDiffuse.xyz * attenuation * (tex2D(diffuseMap,uv.xy).xyz+tex2D(casuticMap,uv.zw*5).xyz*0.4) * dot(normal, lightVec),1);
	//colour = colour*(1-ler);
	//colour = float4(1,0,0,1);
}

// spot lights

void spot_vp2(float4 position	: POSITION,
			 float3 normal		: NORMAL,
			 float2 uv			: TEXCOORD0,
			 float2 uv2			: TEXCOORD1,

			 // outputs
			 out float4 oPosition    : POSITION,
			 out float4 oUv			 : TEXCOORD0,
			 out float3 lightDir	 : TEXCOORD1,
			 out float3 onormal	 : TEXCOORD2,
			 out float4 oPos	 : TEXCOORD3,
			 // parameters
			 uniform float4 lightPosition, // object space
			 uniform float4x4 worldViewProj)
{
	// calculate output position
	oPosition = mul(worldViewProj, position);
	
	oPos = position;

	// pass the main uvs straight through unchanged
	oUv.xy = uv;
	oUv.zw = uv2;
	
	// object space normal
	onormal = normal;
	
	// object space light dir
	lightDir = lightPosition.xyz - position;
	
	
}

// point light frag shader

void spot_fp2( float4 uv			: TEXCOORD0,
			  float3 lightDir : TEXCOORD1,
			  float3 normal : TEXCOORD2,
			  float4 pos : TEXCOORD3,

			  out float4 colour	: COLOR,

			  uniform float4 lightDiffuse,
			  uniform float4 lightAttenuation,
			  uniform float4 camPos,
			  uniform float4 spotParams,
			  uniform float4 spotDir,
			  uniform sampler2D   diffuseMap : register(s0),
			  uniform sampler2D   casuticMap : register(s1))
{
	float ler = saturate((length(pos-camPos)-5.0)/(30.0+lightAttenuation.x-42));
	float rng = length(lightDir);
	if(rng>lightAttenuation.x)
	{
		discard;
		return;
	}
	
	float3 lightVec = lightDir/rng;
	float3 lightValue = 0;
	
	float spotFactor = 0;
    float rho = saturate(dot((-spotDir.xyz),lightVec.xyz ));
    spotFactor = pow(saturate(rho - spotParams.y) / (spotParams.x - spotParams.y), spotParams.z);
    spotFactor = saturate(spotFactor);
	
	
	float attenuation = saturate((lightAttenuation.x-rng)/lightAttenuation.x);
	colour = (1-ler)*float4(lightDiffuse.xyz * spotFactor * attenuation * (tex2D(diffuseMap,uv.xy).xyz+tex2D(casuticMap,uv.zw*5).xyz*0.4) * dot(normal, lightVec),1);
	//colour = colour*(1-ler);
	//colour = float4(1,0,0,1);
}