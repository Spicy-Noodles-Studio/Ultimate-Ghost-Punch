#version 330 core

in vec4 position
in vec3 normal 
in vec2 uv
in vec4 col

out vec4 oPosition
out vec2 oUv
out vec4 oCol 

uniform mat4 worldViewProj
uniform float curlSize
uniform vec3 gravity
uniform float ttl
uniform vec3 seed
uniform float amount

void main()
{
    vec4 mypos = position;

    vec3 deviation = position.xyz;

    float age = (1 - col.a) * ttl;

    mypos.xyz += gravity * age;

    // Calculate some simple turbulence
    deviation = sin ((deviation + seed) / curlSize) * age * amount;
    mypos.xyz += deviation;

    oPosition = mul(worldViewProj, mypos);

    oUv = uv;

    oCol = col;
}