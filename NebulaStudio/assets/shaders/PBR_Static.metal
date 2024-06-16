// -----------------------------
// -- Nebula Engine PBR shader --
// -----------------------------

// Note: this shader is still very much in progress. There are likely many bugs and future additions that will go in.
//       Currently heavily updated. 
//

#include <metal_stdlib>
using namespace metal;

/**
	Vertex Shader
*/

struct VertexInput {
	float3 Position 	[[attribute(0)]];
	float3 Normal 		[[attribute(1)]];
	float3 Tangent 		[[attribute(2)]];
	float3 Binormal 	[[attribute(3)]];
	float2 TexCoord 	[[attribute(4)]];
};

// layout (std140, binding = 0) uniform Camera {
// 	float4x4 u_ViewProjectionMatrix;
// };

// layout (std140, binding = 1) uniform Transform {
// 	float4x4 u_Transform;
// };

// layout (std140, binding = 2) uniform Entity {
// 	int u_EntityID;
// };

struct VertexOutput
{
	float4 Position 		[[position]];
	float3 WorldPosition 	[[shared]];
    float3 Normal			[[shared]];
	float2 TexCoord			[[shared]];
	float3 WorldNormals_x		[[shared]];
	float3 WorldNormals_y		[[shared]];
	float3 WorldNormals_z		[[shared]];
	float3 WorldTransform_x		[[shared]];
	float3 WorldTransform_y		[[shared]];
	float3 WorldTransform_z		[[shared]];
	float3 Binormal				[[shared]];
    int EntityID        		[[shared]];
};

// layout (location = 0) out VertexOutput Output;
// layout (location = 10) out flat int f_EntityID;

float3x3 convertTo3x3(float4x4 mat)
{
	return float3x3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
}

vertex VertexOutput vertexMain(
	VertexInput input [[stage_in]], 
	constant float4x4& Camera [[buffer(1)]], 
	constant float4x4& Transform [[buffer(2)]], 
	constant int& Entity [[buffer(3)]]
) {
	VertexOutput Output = {0};

	Output.Position = float4(input.Position, 1.0) * Transform * Camera;
	Output.WorldPosition = float3(Transform * float4(input.Position, 1.0));
    Output.Normal = convertTo3x3(Transform) * input.Normal;
	Output.TexCoord = input.TexCoord;	//float2(TexCoord.x, 1.0 - TexCoord.y);

	float3x3 intNormals = convertTo3x3(Transform) * float3x3(input.Tangent, input.Binormal, input.Normal);
	Output.WorldNormals_x 	= intNormals[0];
	Output.WorldNormals_y	= intNormals[1];
	Output.WorldNormals_z	= intNormals[2];

	float3x3 intTransform = convertTo3x3(Transform);
	Output.WorldTransform_x = intTransform[0];
	Output.WorldTransform_y = intTransform[1];
	Output.WorldTransform_z = intTransform[2];

	Output.Binormal = input.Binormal;
	Output.EntityID = Entity;

	return Output;
}


/**
	---------------------------------------------------------------------------
								Fragment Shader
	---------------------------------------------------------------------------
*/

constant float PI = 3.141592;
constant float Epsilon = 0.00001;

constant int LightCount = 1;

// Constant normal incidence Fresnel factor for all dielectrics.
constant float3 Fdielectric = float3(0.04);

struct Light {
	float3 Direction;
	float3 Radiance;
	float Multiplier;
};

struct EnvironmentData {
	Light lights;
	float3 u_CameraPosition;
};

struct Material
{
	// layout (offset = 64) 
	float3 AlbedoColor;
	float Metalness;
	float Roughness;

	float EnvMapRotation;

	// Toggles
	float RadiancePrefilter;
	float AlbedoTexToggle;
	float NormalTexToggle;
	float MetalnessTexToggle;
	float RoughnessTexToggle;
};

struct PBRParameters
{
	float3 Albedo;
	float Roughness;
	float Metalness;

	float3 Normal;
	float3 View;
	float NdotV;
};

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2
float ndfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(NdotV, k);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 fresnelSchlickRoughness(float3 F0, float cosTheta, float roughness)
{
    return F0 + (max(float3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
} 

// ---------------------------------------------------------------------------------------------------
// The following code (from Unreal Engine 4's paper) shows how to filter the environment map
// for different roughnesses. This is mean to be computed offline and stored in cube map mips,
// so turning this on online will cause poor performance
float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i)/float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 Xi, float Roughness, float3 N)
{
	float a = Roughness * Roughness;
	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt( (1 - Xi.y) / ( 1 + (a*a - 1) * Xi.y ) );
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );
	float3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;
	float3 UpVector = abs(N.z) < 0.999 ? float3(0,0,1) : float3(1,0,0);
	float3 TangentX = normalize( cross( UpVector, N ) );
	float3 TangentY = cross( N, TangentX );
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

float3 PrefilterEnvMap(float Roughness, float3 R, device float& TotalWeight)
{
	float3 N = R;
	float3 V = R;
	float3 PrefilteredColor = float3(0.0);
	int NumSamples = 1024;
	for(int i = 0; i < NumSamples; i++)
	{
		float2 Xi = Hammersley(i, NumSamples);
		float3 H = ImportanceSampleGGX(Xi, Roughness, N);
		float3 L = 2 * dot(V, H) * H - V;
		float NoL = clamp(dot(N, L), 0.0, 1.0);
		if (NoL > 0)
		{
			//PrefilteredColor += texture(u_EnvRadianceTex, L).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / TotalWeight;
}

// ---------------------------------------------------------------------------------------------------

float radians(float degrees) {
	return degrees * PI / 180.0;
}

float3 RotateVectorAboutY(float angle, float3 vec)
{
    angle = radians(angle);
    float3x3 rotationMatrix = float3x3(
		float3(cos(angle),0.0,sin(angle)),
		float3(0.0,1.0,0.0),
		float3(-sin(angle),0.0,cos(angle))
	);

    return rotationMatrix * vec;
}

float3 Lighting(float3 F0, EnvironmentData env, PBRParameters m_Params)
{
	float3 result = float3(0.0);
	for(int i = 0; i < LightCount; i++)
	{
		float3 Li = -env.lights.Direction;
		float3 Lradiance = env.lights.Radiance * env.lights.Multiplier;
		float3 Lh = normalize(Li + m_Params.View);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(m_Params.Normal, Li));
		float cosLh = max(0.0, dot(m_Params.Normal, Lh));

		float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, m_Params.View)));
		float D = ndfGGX(cosLh, m_Params.Roughness);
		float G = gaSchlickGGX(cosLi, m_Params.NdotV, m_Params.Roughness);

		float3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
		float3 diffuseBRDF = kd * m_Params.Albedo;

		// Cook-Torrance
		float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * m_Params.NdotV);

		result += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	}
	return result;
}

float3 IBL(float3 F0, float3 Lr)
{
	//float3 irradiance = texture(u_EnvIrradianceTex, m_Params.Normal).rgb;
	//float3 F = fresnelSchlickRoughness(F0, m_Params.NdotV, m_Params.Roughness);
	//float3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
	//float3 diffuseIBL = m_Params.Albedo * irradiance;
	//
	//int envRadianceTexLevels = textureQueryLevels(u_EnvRadianceTex);
	//float NoV = clamp(m_Params.NdotV, 0.0, 1.0);
	//float3 R = 2.0 * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;
	//float3 specularIrradiance = textureLod(u_EnvRadianceTex, RotateVectorAboutY(u_MaterialUniforms.EnvMapRotation, Lr), (m_Params.Roughness) * envRadianceTexLevels).rgb;
	//
	//// Sample BRDF Lut, 1.0 - roughness for y-coord because texture was generated (in Sparky) for gloss model
	//float2 specularBRDF = texture(u_BRDFLUTTexture, float2(m_Params.NdotV, 1.0 - m_Params.Roughness)).rg;
	//float3 specularIBL = specularIrradiance * (F * specularBRDF.x + specularBRDF.y);
	//
	//return kd * diffuseIBL + specularIBL;
	return float3(0.0);
}


// PBR texture inputs
// layout (location = 0) in VertexOutput Input;
// layout (location = 10) flat in int f_EntityID;

// layout (std140, binding = 1) uniform 
// struct EnvironmentData {
// 	Light lights;
// 	float3 u_CameraPosition; // Offset = 32
// };

// layout (binding = 2) uniform sampler2D u_AlbedoTexture;
// layout (binding = 3) uniform sampler2D u_NormalTexture;
// layout (binding = 4) uniform sampler2D u_MetalnessTexture;
// layout (binding = 5) uniform sampler2D u_RoughnessTexture;

// Environment maps
// layout (binding = 6) uniform samplerCube u_EnvRadianceTex;
// layout (binding = 7) uniform samplerCube u_EnvIrradianceTex;

// BRDF LUT
// layout (binding = 8) uniform sampler2D u_BRDFLUTTexture;
// layout (binding = 9) uniform Material u_MaterialUniforms;

struct FragmentOutput {
    float4 Color 	[[color(0)]];
    int    EntityID [[color(1)]];
};

constexpr sampler linearSampler(coord::normalized, min_filter::linear, mag_filter::linear, mip_filter::linear);
fragment FragmentOutput fragmentMain(
	VertexOutput Input [[stage_in]],
	// EnvironmentData Environment [[buffer(1)]],
	texture2d<float> u_AlbedoTexture [[texture(0)]]
){
	PBRParameters m_Params = {0};

	// Standard PBR inputs
	//m_Params.Albedo = u_MaterialUniforms.AlbedoTexToggle > 0.5 ? texture(u_AlbedoTexture, Input.TexCoord).rgb : u_MaterialUniforms.AlbedoColor; 
	//m_Params.Metalness = u_MaterialUniforms.MetalnessTexToggle > 0.5 ? texture(u_MetalnessTexture, Input.TexCoord).r : u_MaterialUniforms.Metalness;
	//m_Params.Roughness = u_MaterialUniforms.RoughnessTexToggle > 0.5 ?  texture(u_RoughnessTexture, Input.TexCoord).r : u_MaterialUniforms.Roughness;
    //m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight

	// Normals (either from vertex or map)
	//m_Params.Normal = normalize(Input.Normal);
	//if (u_MaterialUniforms.NormalTexToggle > 0.5)
	//{
	//	m_Params.Normal = normalize(2.0 * texture(u_NormalTexture, Input.TexCoord).rgb - 1.0);
	//	m_Params.Normal = normalize(Input.WorldNormals * m_Params.Normal);
	//}
	//
	//m_Params.View = normalize(u_CameraPosition - Input.WorldPosition);
	//m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);
	//	
	//// Specular reflection vector
	//float3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
	//
	//// Fresnel reflectance, metals use albedo
	//float3 F0 = mix(Fdielectric, m_Params.Albedo, m_Params.Metalness);
	//
	//float3 lightContribution = Lighting(F0);
	//float3 iblContribution = IBL(F0, Lr);
	//
	//color = float4(lightContribution + iblContribution, 1.0);

	float4 texColor = u_AlbedoTexture.sample(linearSampler, Input.TexCoord);
	texColor.w = 1.0;
	// float3 albedo = texture(u_AlbedoTexture, Input.TexCoord).rgb; 

	FragmentOutput output = {0};
	output.Color = texColor; //float4(texColor.rgb * albedo, 1.0);
	output.EntityID = Input.EntityID;
	return output;
}