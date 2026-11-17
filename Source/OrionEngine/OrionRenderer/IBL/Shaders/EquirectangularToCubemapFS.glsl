
#version 330 core

out vec4 o_Color;

in vec3 v_LocalPosition;

uniform sampler2D u_EquirectangularMap;

const vec2 INV_ATAN = vec2(0.15915494309, 0.31830988618);

vec2 SampleSphericalMap(vec3 direction)
{
    vec2 uv = vec2(
        atan(direction.z, direction.x),
        asin(direction.y)
    );

    uv *= INV_ATAN;
    uv += 0.5;

    uv.y = 1.0 - uv.y;

    return uv;
}

void main()
{
    vec3 direction = normalize(v_LocalPosition);

    vec2 uv = SampleSphericalMap(direction);

    vec3 color = texture(u_EquirectangularMap, uv).rgb;

    o_Color = vec4(color, 1.0);
}
