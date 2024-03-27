#shader fragment
#version 450 core

in vec2 FragCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_BackProjection;

vec3 RGBToHSI( vec3 rgb );
vec3 HSIToRGB( vec3 hsi );

float shift = 1.0 / 512.0f;
float PI = 3.14159265;

void main() {
	vec3 hsi = RGBToHSI( texture( u_Texture, FragCoord ).rgb );

	hsi.z = texture( u_BackProjection, vec2( hsi.z ) ).r;

	vec3 rgb = HSIToRGB( hsi );
	
	FragColor = clamp( vec4 ( rgb, 1.0 ), 0.0, 1.0 );
}

vec3 RGBToHSI( vec3 rgb ) {
	vec3 hsi = vec3( 0.0 );

	// Intensity Calculation: [0.0:1.0]
	hsi.b = ( rgb.r + rgb.g + rgb.b ) / 3.0;

	// Saturation Calculation: [0.0:1.0]
	hsi.g = 1.0 -  min( rgb.r, min( rgb.g, rgb.b ) ) / hsi.b;	

	// Hue Calculation: [0.0:2 * PI]
	hsi.r = acos( ( 0.5 * ( ( rgb.r - rgb.g ) + ( rgb.r - rgb.b ) ) ) / sqrt( ( rgb.r - rgb.g ) * ( rgb.r - rgb.g ) + ( rgb.r - rgb.b ) * ( rgb.g - rgb.b ) ) );

	if( hsi.r > 1.0 ) hsi.r = 1;
	if( hsi.r < -1.0 ) hsi.r = -1;

	if( rgb.b > rgb.g ) {
		hsi.r = 2.0 * PI - hsi.r;
	}

	return hsi;
}

vec3 HSIToRGB( vec3 hsi ) {
	vec3 rgb = vec3( 0.0 );

	if( hsi.r < 2.0 * PI / 3.0 ) {
		rgb.b = hsi.b * ( 1.0 - hsi.g );
		rgb.r = hsi.b * ( 1.0 + hsi.g * cos( hsi.r ) / cos( PI / 3.0 - hsi.r ) );
		rgb.g = 3.0 * hsi.b - ( rgb.r + rgb.b );
	} else if ( hsi.r < 4.0 * PI / 3.0 ) {
		hsi.r = hsi.r - 2.0 * PI / 3.0;
		rgb.r = hsi.b * ( 1.0 - hsi.g );
		rgb.g = hsi.b * ( 1.0 + hsi.g * cos( hsi.r ) / cos( PI / 3.0 - hsi.r ) );
		rgb.b = 3.0 * hsi.b - ( rgb.r + rgb.g );
	} else {
		hsi.r = hsi.r - 4.0 * PI / 3.0;
		rgb.g = hsi.b * ( 1.0 - hsi.g );
		rgb.b = hsi.b * ( 1.0 + hsi.g * cos( hsi.r ) / cos( PI / 3.0 - hsi.r ) );
		rgb.r = 3.0 * hsi.b - ( rgb.g + rgb.b );	
	} 

	return rgb;
}


#shader vertex
#version 450 core

layout( location = 0 ) in vec2 Pos;
layout( location = 1 ) in vec2 TexCoords;

out vec2 FragCoord;

void main() {
	gl_Position = vec4( Pos, 0.0, 1.0 );
	FragCoord = TexCoords;
}

