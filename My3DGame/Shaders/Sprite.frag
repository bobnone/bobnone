// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;
in vec3 fragVertexColor;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	vec4 tex = texture(uTexture, fragTexCoord);
	float R = clamp(tex.x + fragVertexColor.x, 0.0, 1.0);
	float G = clamp(tex.y + fragVertexColor.y, 0.0, 1.0);
	float B = clamp(tex.z + fragVertexColor.z, 0.0, 1.0);
	float A = clamp(tex.w, 0.0, 1.0);
	// Sample color from texture
	outColor = vec4(R, G, B, A);
}
