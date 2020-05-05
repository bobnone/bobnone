// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
layout(location = 0) out vec4 outColor;

// Different textures from G-buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGSpecular;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// Create a struct for directional light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecularColor;
};

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Specular power for this surface
uniform float uSpecularPower;
// Ambient light level
uniform vec3 uAmbientLight;
// Directional Light
uniform DirectionalLight uDirLight;

void main()
{
	// Sample from G-buffer
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferSpecular = texture(uGSpecular, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	// Surface normal
	vec3 N = normalize(gbufferNorm);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos - gbufferWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecularColor * pow(max(0.0, dot(R, V)), uSpecularPower);
		Phong += Diffuse + Specular;
	}
	// Clamp light between 0-1 RGB values
	Phong = clamp(Phong, 0.0, 1.0);

	// Final color is texture color times phong light (alpha = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}