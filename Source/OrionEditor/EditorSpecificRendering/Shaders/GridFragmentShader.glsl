
#version 330 core

in vec2 v_NDC;
out vec4 o_Color;

// Camera uniforms
uniform mat4 u_InverseViewProjection;
uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPosition;

// Grid settings
uniform float u_GridSpacing;
uniform float u_FadeStart;
uniform float u_FadeEnd;

// --------------------------------------------------
// Background colors
// --------------------------------------------------

const vec3 SKY_TOP = vec3(0.24, 0.38, 0.56);
const vec3 SKY_HORIZON = vec3(0.48, 0.55, 0.63);

const vec3 VOID_COLOR = vec3(0.008, 0.010, 0.015);

// --------------------------------------------------
// Grid colors
// --------------------------------------------------

const vec3 MINOR_COLOR = vec3(0.24, 0.26, 0.29);
const vec3 MAJOR_COLOR = vec3(0.36, 0.38, 0.41);

// World axes
const vec3 X_AXIS_COLOR = vec3(0.90, 0.20, 0.18);
const vec3 Y_AXIS_COLOR = vec3(0.20, 0.80, 0.35);
const vec3 Z_AXIS_COLOR = vec3(0.20, 0.42, 0.95);

// Grid settings
const float MAJOR_GRID_INTERVAL = 10.0;
const float AXIS_WIDTH = 1.5;

// --------------------------------------------------
// Background
// --------------------------------------------------

vec3 CalculateBackground()
{
    float skyBlend = smoothstep(
        -0.15,
        0.65,
        v_NDC.y
    );

    vec3 sky = mix(
        SKY_HORIZON,
        SKY_TOP,
        smoothstep(0.0, 1.0, v_NDC.y)
    );

    return mix(
        VOID_COLOR,
        sky,
        skyBlend
    );
}

// --------------------------------------------------
// Antialiased grid
// --------------------------------------------------

float CalculateGridLine(vec2 coordinates)
{
    vec2 derivative = max(
        fwidth(coordinates),
        vec2(0.00001)
    );

    vec2 distanceToLine = abs(
        fract(coordinates - 0.5) - 0.5
    );

    vec2 lineDistance = distanceToLine / derivative;

    return 1.0 - min(
        min(lineDistance.x, lineDistance.y),
        1.0
    );
}

// --------------------------------------------------
// World axis lines
// --------------------------------------------------

float CalculateAxisLine(float coordinate)
{
    float derivative = max(
        fwidth(coordinate),
        0.00001
    );

    return 1.0 - smoothstep(
        derivative * 0.5,
        derivative * AXIS_WIDTH,
        abs(coordinate)
    );
}

// --------------------------------------------------
// Main
// --------------------------------------------------

void main()
{
    // Background
    o_Color = vec4(CalculateBackground(), 1.0);

    gl_FragDepth = 0.999999;

    // --------------------------------------------------
    // World-space ray reconstruction
    // --------------------------------------------------

    vec4 nearPoint = u_InverseViewProjection *
        vec4(v_NDC, -1.0, 1.0);

    vec4 farPoint = u_InverseViewProjection *
        vec4(v_NDC, 1.0, 1.0);

    if (abs(nearPoint.w) < 0.000001 ||
        abs(farPoint.w) < 0.000001)
    {
        return;
    }

    nearPoint /= nearPoint.w;
    farPoint /= farPoint.w;

    vec3 rayOrigin = nearPoint.xyz;

    vec3 rayDirection = normalize(
        farPoint.xyz - nearPoint.xyz
    );

    if (abs(rayDirection.y) < 0.0001)
    {
        return;
    }

    // --------------------------------------------------
    // Ground plane intersection
    // --------------------------------------------------

    float intersectionDistance =
        -rayOrigin.y / rayDirection.y;

    if (intersectionDistance <= 0.0)
    {
        return;
    }

    vec3 worldPosition =
        rayOrigin + rayDirection * intersectionDistance;

    // --------------------------------------------------
    // Depth reconstruction
    // --------------------------------------------------

    vec4 clipPosition = u_ViewProjection *
        vec4(worldPosition, 1.0);

    if (clipPosition.w <= 0.000001)
    {
        return;
    }

    float ndcDepth = clipPosition.z / clipPosition.w;

    if (ndcDepth < -1.0 || ndcDepth > 1.0)
    {
        return;
    }

    float depth = ndcDepth * 0.5 + 0.5;

    // --------------------------------------------------
    // Distance fading
    // --------------------------------------------------

    float cameraDistance = length(
        worldPosition.xz - u_CameraPosition.xz
    );

    float fade = 1.0 - smoothstep(
        u_FadeStart,
        u_FadeEnd,
        cameraDistance
    );

    if (fade <= 0.001)
    {
        return;
    }

    // --------------------------------------------------
    // Grid coordinates
    // --------------------------------------------------

    float spacing = max(u_GridSpacing, 0.0001);

    vec2 gridCoordinates =
        worldPosition.xz / spacing;

    float minorGrid = CalculateGridLine(
        gridCoordinates
    );

    float majorGrid = CalculateGridLine(
        gridCoordinates / MAJOR_GRID_INTERVAL
    );

    // --------------------------------------------------
    // World axes
    // --------------------------------------------------

    float xAxis = CalculateAxisLine(worldPosition.z);
    float zAxis = CalculateAxisLine(worldPosition.x);

    // --------------------------------------------------
    // Grid composition
    // --------------------------------------------------

    vec3 gridColor = mix(
        MINOR_COLOR,
        MAJOR_COLOR,
        majorGrid * 0.75
    );

    gridColor = mix(
        gridColor,
        X_AXIS_COLOR,
        xAxis
    );

    gridColor = mix(
        gridColor,
        Z_AXIS_COLOR,
        zAxis
    );

    float minorAlpha = minorGrid * 0.24;
    float majorAlpha = majorGrid * 0.42;

    float gridAlpha = max(
        minorAlpha,
        majorAlpha
    );

    gridAlpha = max(gridAlpha, xAxis * 0.90);
    gridAlpha = max(gridAlpha, zAxis * 0.90);

    gridAlpha *= fade;

    // Composite grid over background
    o_Color.rgb = mix(
        o_Color.rgb,
        gridColor,
        gridAlpha
    );

    // Ground intersection depth
    gl_FragDepth = depth;
}
