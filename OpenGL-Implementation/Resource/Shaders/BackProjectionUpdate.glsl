#shader fragment
#version 450 core

in vec2 FragCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_BackProjection;

float shift = 1.0 / 2048.0f;

vec3 RGBToHSL( vec3 rgb );
vec3 HSLToRGB( vec3 hsl );

void main() {
	vec3 hsl = RGBToHSL( texture( u_Texture, FragCoord ).rgb );
	
	hsl.z += 0.03;
	//hsl.z = texture( u_BackProjection, vec2( clamp( hsl.z - shift, 0.0, 1.0 ), 0 ) ).r;
	//hsl.z = clamp( texture( u_BackProjection, vec2( clamp( hsl.z + shift, 0.0, 1.0 ), 0 ) ).r, 0.0, 1.0 );
	//hsl.z = texture( u_BackProjection, vec2( hsl.z - shift, 0 ) ).r;

	FragColor = clamp( vec4 ( HSLToRGB( hsl ), 1.0 ), 0.0, 1.0);
}

vec3 RGBToHSL( vec3 rgb ) {
	vec3 hsl = vec3( 0.0 );

	float minValue = min( rgb.r, min( rgb.g, rgb.b ) );
	float maxValue = max( rgb.r, max( rgb.g, rgb.b ) );

	// Calculate luminance
	hsl.z = ( maxValue + minValue ) / 2.0;
	if( maxValue > minValue ) {
		float delta = maxValue - minValue;

		// Calculate saturation
		if( hsl.z < 0.0 ) {
			hsl.y = delta / ( 2.0 - maxValue - minValue );
		} else {
			hsl.y = delta / ( maxValue + minValue );
		}

		// Calculate hue
		if( rgb.r + 0.01 > maxValue ) {
			hsl.x = ( rgb.g - rgb.b ) / delta;
		} else if( rgb.g + 0.01 > maxValue ) {
			hsl.x = 2.0 + ( rgb.b - rgb.r ) / delta;
		} else {
			hsl.x = 4.0 + ( rgb.r - rgb.g ) / delta;
		}

		if( hsl.x < 0.0 ) {
			hsl.x += 6.0;
		}
		hsl.x /= 6.0;
	}

	return hsl;
}

vec3 HSLToRGB( vec3 hsl ) {
	vec3 rgb = clamp( abs( mod( hsl.x * 6.0 + vec3( 0.0, 4.0, 2.0 ), 6.0 ) - 3.0 ) - 1.0, 0.0, 1.0 );
	rgb = hsl.z + hsl.y * ( rgb - 0.5 ) * ( 1.0 - abs( 2.0 * hsl.z - 1.0 ) );

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

