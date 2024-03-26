#shader vertex
#version 450 core

layout ( location = 0 ) in vec2 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

vec3 RGBToHSL( vec3 rgb );
vec3 HSLToRGB( vec3 hsl );

uniform sampler2D u_Texture;

uniform int u_rgb;

void main() {
	ivec2 imDims = textureSize( u_Texture, 0 );
	ivec2 imageCoordinates = ivec2( ( gl_VertexID ) % imDims.x, ( gl_VertexID ) / imDims.x );

	vec3 hsl = RGBToHSL( texelFetch( u_Texture, imageCoordinates, 0 ).rgb );

	// Scatter Luminance
	float intensity = clamp( hsl.b, 0.0, 1.0 );

	// Shift so histogram fits 0 to 511
	intensity *= 0.9998;

	gl_Position = vec4( ( intensity * 2.0 ) - 1.0, 0.0, 0.0, 1.0 );
	gl_PointSize = 1.0;
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
		if( hsl.z < 0.5 ) {
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


#shader fragment
#version 450 core

layout(location = 0) out vec4 FragColor;

void main() {
	FragColor = vec4( 1.0 );
}