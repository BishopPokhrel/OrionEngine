#version 330 core

out vec4 o_Color;

in vec3 v_LocalPosition;

uniform samplerCube u_EnvironmentMap;
uniform float u_Roughness;
uniform float u_EnvironmentResolution;

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

vec3 ImportanceSampleGGX(vec2 xi, vec3 normal, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;

    float phi = 2.0 * PI * xi.x;

    float cosTheta = sqrt(
        (1.0 - xi.y) /
        (1.0 + (alphaSquared - 1.0) * xi.y)
    );

    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    vec3 halfVector = vec3(
        cos(phi) * sinTheta,
        sin(phi) * sinTheta,
        cosTheta
    );

    vec3 up = abs(normal.z) < 0.999
        ? vec3(0.0, 0.0, 1.0)
        : vec3(1.0, 0.0, 0.0);

    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    return normalize(
        tangent * halfVector.x +
        bitangent * halfVector.y +
        normal * halfVector.z
    );
}

float DistributionGGX(vec3 normal, vec3 halfVector, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;

    float NdotH = max(dot(normal, halfVector), 0.0);

    float denominator =
        NdotH * NdotH * (alphaSquared - 1.0) + 1.0;

    return alphaSquared /
        max(PI * denominator * denominator, 0.000001);
}

void main()
{
    vec3 normal = normalize(v_LocalPosition);
    vec3 reflection = normal;
    vec3 viewDirection = reflection;

    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;

    float texelSolidAngle =
        4.0 * PI /
        (6.0 * u_EnvironmentResolution * u_EnvironmentResolution);

    for (uint index = 0u; index < SAMPLE_COUNT; index++)
    {
        vec2 xi = Hammersley(index, SAMPLE_COUNT);

        vec3 halfVector =
            ImportanceSampleGGX(xi, normal, u_Roughness);

        vec3 lightDirection =
            normalize(2.0 * dot(viewDirection, halfVector) * halfVector
                      - viewDirection);

        float NdotL = max(dot(normal, lightDirection), 0.0);

        if (NdotL > 0.0)
        {
            float NdotH = max(dot(normal, halfVector), 0.0);
            float HdotV = max(dot(halfVector, viewDirection), 0.0);

            float distribution =
                DistributionGGX(normal, halfVector, u_Roughness);

            float pdf = max(
                distribution * NdotH / (4.0 * HdotV),
                0.000001
            );

            float sampleSolidAngle =
                1.0 / (float(SAMPLE_COUNT) * pdf);

            float mipLevel = u_Roughness == 0.0
                ? 0.0
                : 0.5 * log2(sampleSolidAngle / texelSolidAngle);

            prefilteredColor +=
                textureLod(u_EnvironmentMap, lightDirection, mipLevel).rgb
                * NdotL;

            totalWeight += NdotL;
        }
    }

    prefilteredColor /= max(totalWeight, 0.000001);

    o_Color = vec4(prefilteredColor, 1.0);
}
