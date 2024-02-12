#shader vertex
#version 450 core

layout( location = 0 ) in vec2 aPos;
layout( location = 1 ) in vec2 aTexCoords;

out vec2 FragCoord;

void main() {
	gl_Position = vec4( aPos, 0.0, 1.0 );
	FragCoord = aTexCoords;
}

#shader fragment
#version 450 core

in vec2 FragCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;

float pi = 3.141592;

vec3 RGBToHSL(vec3 rgb);
vec3 HSLToRGB(vec3 hsl);

void main() {
	vec3 color = vec3( texture( u_Texture, FragCoord.st ) );

	FragColor = vec4( color, 1.0 );
}

vec3 RGBToHSL(vec3 rgb) {
	float maxValue = max( rgb.r, rgb.g );
	maxValue = max( maxValue, rgb.b );

	float minValue = min( rgb.r, rgb.g );
	minValue = min( minValue, rgb.b );
 
	float chroma = maxValue - minValue;
	float luminosity = ( maxValue + minValue ) / 2.0;
	float hue, saturation;

	if( maxValue == minValue ) {
		// Will likely never hit here.
		hue = 0.0;
		saturation = 0.0;
	} else {
		saturation = maxValue - minValue;
		luminosity = maxValue;

		switch ( maxValue )  {
		case rgb.r:
			hue = ( rgb.g - rgb.b ) * ( pi / 3 ) / chroma;
			break;
		case rgb.g:
			hue = ( 2 + ( ( rgb.b - rgb.r )  / chroma ) ) * ( pi / 3 )
			break;
		case rgb.b:
			hue = ( 4 + ( ( rgb.r - rgb.g )  / chroma ) ) * ( pi / 3 )
			break;
		default:
			break;
		}
	{

	return vec3( hue, saturation, luminosity );
}

vec3 HSLToRGB(vec3 hsl) {

}


