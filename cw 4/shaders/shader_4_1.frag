#version 430 core

uniform vec3 color;
uniform float near;
uniform float far;

out vec4 out_color;

float linearizeDepth(float zPrime, float n, float f) {
    float z = (-2.0 * n * f) / (zPrime * (n - f) + n + f);
    return z / f;
}

void main()
{
    float zPrime = gl_FragCoord.z * 2.0 - 1.0;
    zPrime = linearizeDepth(zPrime, near, far);

    out_color = vec4(zPrime, zPrime, zPrime, 1.0);
}
