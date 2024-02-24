#shader vertex
#version 450 core

layout ( location = 0 ) in vec2 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
	gl_Position = vec4( aPos, 0.0, 1.0 );
	TexCoords = aTexCoords;
}

#shader fragment
#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform float u_Luminance;
uniform float u_Chromatic_1;
uniform float u_Chromatic_2;

const mat3 RGB2CIE = mat3( 0.4124564, 0.3575761, 0.1804375,
						   0.2126729, 0.7151522, 0.0721750,
						   0.0193339, 0.1191920, 0.9503041 );

const mat3 CIE2RGB = mat3( 3.2404542, -1.5371385, -0.4985314,
						   -0.9692660, 1.8760108, 0.0415560,
						   0.0556434, -0.2040259, 1.0572252 );

vec3 RGBToCIEXYZ( vec3 rgb );
vec3 CIEXYZToRGB( vec3 Yxy );

void main() {
	vec3 xyz = RGBToCIEXYZ( vec3( texture(u_Texture, TexCoords).rgb ) );
	xyz.x = pow( xyz.x, (1.0 / u_Luminance) ); // Lumaniance
	xyz.y = pow( xyz.y, (1.0 / u_Chromatic_1) ); // Chromatic Info
	xyz.z = pow( xyz.z, (1.0 / u_Chromatic_2) ); // Chromatic Info
	vec3 rgb = CIEXYZToRGB(xyz);
	
	FragColor = vec4( rgb, 1.0 );
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

// Takes in a CIE-XYZ format and returns RGB color(0-1.0)
vec3 CIEXYZToRGB(vec3 Yxy) {
	vec3 xyz;
	xyz.r = Yxy.r * Yxy.g / Yxy.b;
	xyz.g = Yxy.r;
	xyz.b = Yxy.r * (1.0 - Yxy.g - Yxy.b) / Yxy.b;

	return CIE2RGB * xyz;
}