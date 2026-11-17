#version 330 core

out vec2 o_BRDF;

in vec2 v_TexCoords;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;

float RadicalInverseVdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

    return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint index, uint count)
{
    return vec2(
        float(index) / float(count),
        RadicalInverseVdC(index)
    );
}

vec3 ImportanceSampleGGX(vec2 xi, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;

    float phi = 2.0 * PI * xi.x;

    float cosTheta = sqrt(
        (1.0 - xi.y) /
        (1.0 + (alphaSquared - 1.0) * xi.y)
    );

    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    return vec3(
        cos(phi) * sinTheta,
        sin(phi) * sinTheta,
        cosTheta
    );
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float k = roughness * roughness * 0.5;

    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 normal, vec3 viewDirection,
                    vec3 lightDirection, float roughness)
{
    float NdotV = max(dot(normal, viewDirection), 0.0);
    float NdotL = max(dot(normal, lightDirection), 0.0);

    float geometryView = GeometrySchlickGGX(NdotV, roughness);
    float geometryLight = GeometrySchlickGGX(NdotL, roughness);

    return geometryView * geometryLight;
}

vec2 IntegrateBRDF(float NdotV, float roughness)
{
    vec3 viewDirection = vec3(
        sqrt(1.0 - NdotV * NdotV),
        0.0,
        NdotV
    );

    vec3 normal = vec3(0.0, 0.0, 1.0);

    float A = 0.0;
    float B = 0.0;

    for (uint index = 0u; index < SAMPLE_COUNT; index++)
    {
        vec2 xi = Hammersley(index, SAMPLE_COUNT);

        vec3 halfVector = ImportanceSampleGGX(xi, roughness);

        vec3 lightDirection =
            normalize(2.0 * dot(viewDirection, halfVector) * halfVector
                      - viewDirection);

        float NdotL = max(lightDirection.z, 0.0);
        float NdotH = max(halfVector.z, 0.0);
        float VdotH = max(dot(viewDirection, halfVector), 0.0);

        if (NdotL > 0.0)
        {
            float geometry = GeometrySmith(
                normal,
                viewDirection,
                lightDirection,
                roughness
            );

            float geometryVisibility =
                (geometry * VdotH) /
                max(NdotH * NdotV, 0.000001);

            float fresnel = pow(1.0 - VdotH, 5.0);

            A += (1.0 - fresnel) * geometryVisibility;
            B += fresnel * geometryVisibility;
        }
    }

    return vec2(A, B) / float(SAMPLE_COUNT);
}

void main()
{
    float NdotV = v_TexCoords.x;
    float roughness = v_TexCoords.y;

    o_BRDF = IntegrateBRDF(NdotV, roughness);
}
