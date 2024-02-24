#shader vertex
#version 450 core

layout ( location = 0 ) in vec2 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

uniform sampler2D u_Texture;
uniform int u_color;

const mat3 RGB2CIE = mat3( 0.4124564, 0.3575761, 0.1804375,
						   0.2126729, 0.7151522, 0.0721750,
						   0.0193339, 0.1191920, 0.9503041 );

vec3 RGBToCIEXYZ( vec3 rgb );

void main() {
	ivec2 imDims = textureSize( u_Texture, 0 );
	ivec2 imageCoordinates = ivec2( ( gl_VertexID ) % imDims.x, ( gl_VertexID ) / imDims.x );

	vec3 xyz = RGBToCIEXYZ( texelFetch( u_Texture, imageCoordinates, 0 ).rgb );

	float intensity = clamp( xyz.x, 0.0, 1.0 );

	// Shift so histogram fits 0 to 511
	intensity *= 0.9998;

	gl_Position = vec4( ( intensity * 2.0 ) - 1.0, 0.0, 0.0, 1.0 );
	gl_PointSize = 1.0;
}

// Takes in a RGB color(0-1.0) and returns it in CIE-XYZ format(Yxy).
// Y is luminance, x & y are chromatic info
vec3 RGBToCIEXYZ(vec3 rgb) {	
	vec3 xyz = RGB2CIE * rgb;
	vec3 Yxy;
	Yxy.r = xyz.g;
	float temp = dot(vec3(1.0), xyz);
	Yxy.gb = xyz.rg / temp;

	return Yxy;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 FragColor;

void main() {
	FragColor = vec4( 1.0 );
}