
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

// Background
const vec3 SKY_TOP      = vec3(0.105, 0.165, 0.255);
const vec3 SKY_HORIZON  = vec3(0.045, 0.075, 0.120);
const vec3 VOID_COLOR   = vec3(0.008, 0.012, 0.020);

// Grid colors
const vec3 MINOR_COLOR  = vec3(0.20, 0.25, 0.32);
const vec3 MAJOR_COLOR  = vec3(0.32, 0.39, 0.49);

// World axes
const vec3 X_AXIS_COLOR = vec3(0.85, 0.24, 0.24);
const vec3 Z_AXIS_COLOR = vec3(0.25, 0.43, 0.95);

const float MAJOR_GRID_INTERVAL = 10.0;
const float AXIS_WIDTH = 1.5;

// --------------------------------------------------
// Background
// --------------------------------------------------

vec3 CalculateBackground()
{
    float horizonBlend = smoothstep(-0.12, 0.18, v_NDC.y);

    vec3 sky = mix(
        SKY_HORIZON,
        SKY_TOP,
        smoothstep(0.0, 1.0, v_NDC.y)
    );

    return mix(VOID_COLOR, sky, horizonBlend);
}

// --------------------------------------------------
// Antialiased grid
// --------------------------------------------------

float CalculateGridLine(vec2 coordinates)
{
    vec2 derivative = max(fwidth(coordinates), vec2(0.00001));

    vec2 distanceToLine = abs(fract(coordinates - 0.5) - 0.5);

    vec2 lineDistance = distanceToLine / derivative;

    return 1.0 - min(min(lineDistance.x, lineDistance.y), 1.0);
}

float CalculateAxisLine(float coordinate)
{
    float derivative = max(fwidth(coordinate), 0.00001);

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
    // Background is drawn at the far end of the depth range.
    o_Color = vec4(CalculateBackground(), 1.0);
    gl_FragDepth = 0.999999;

    // Reconstruct near and far points.
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
    vec3 rayDirection = normalize(farPoint.xyz - nearPoint.xyz);

    // Avoid unstable intersections with nearly horizontal rays.
    if (abs(rayDirection.y) < 0.0001)
    {
        return;
    }

    // Intersect the ray with the XZ ground plane.
    float intersectionDistance = -rayOrigin.y / rayDirection.y;

    if (intersectionDistance <= 0.0)
    {
        return;
    }

    vec3 worldPosition = rayOrigin + rayDirection * intersectionDistance;

    // Project the intersection back into clip space.
    vec4 clipPosition = u_ViewProjection * vec4(worldPosition, 1.0);

    if (clipPosition.w <= 0.000001)
    {
        return;
    }

    float ndcDepth = clipPosition.z / clipPosition.w;

    // Respect the camera's near and far clipping planes.
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

    vec2 gridCoordinates = worldPosition.xz / spacing;

    float minorGrid = CalculateGridLine(gridCoordinates);

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

    vec3 gridColor = MINOR_COLOR;

    gridColor = mix(
        gridColor,
        MAJOR_COLOR,
        majorGrid * 0.65
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

    float gridAlpha = max(minorGrid * 0.45, majorGrid * 0.75);

    gridAlpha = max(gridAlpha, xAxis);
    gridAlpha = max(gridAlpha, zAxis);

    gridAlpha *= fade;

    // Composite over the background.
    o_Color.rgb = mix(

        o_Color.rgb,
        gridColor,
        gridAlpha
    );

    // Write the actual ground intersection depth.
    gl_FragDepth = depth;
}
