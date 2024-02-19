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

// Implemented from https://www.pocketmagic.net/enhance-saturation-in-images-programatically/

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform float u_fract;

vec3 RGBToHSL( vec3 rgb );
vec3 HSLToRGB( vec3 hsl );
vec3 SetPixel( vec3 hsl, float fract );

void main() {
	vec3 hsl = RGBToHSL( texture( u_Texture, TexCoords.st ).rgb );

	hsl = SetPixel( hsl, u_fract );


	vec3 rgb = HSLToRGB( hsl );
	FragColor = vec4( rgb, 1.0 );
}

vec3 SetPixel( vec3 hsl, float fract ) {
	float grayFactor = hsl.y / 255.0;
	float varInterval = 255 - hsl.y;
	hsl.y = hsl.y + fract * varInterval * grayFactor;

	return hsl;
}

// Takes in a RGB color(0-1.0) and returns it in HSL format
// X is Hue, Y is Saturation, & Z is Luminance
vec3 RGBToHSL( vec3 rgb ) {	
	vec3 hsl = vec3( 0.0 );

	float maxColor = rgb.r;
	int max = 0;
	if( rgb.g > maxColor ) {
		maxColor = rgb.g;
		max = 1;
	}
	if( rgb.b > maxColor ) {
		maxColor = rgb.b;
		max = 2;
	}

	float minColor = rgb.r;
	if( rgb.g < minColor ) {
		minColor = rgb.g;
	}
	if( rgb.b < minColor ) {
		minColor = rgb.b;
	}

	// Check if color is a shade of grade
	if( ( rgb.r == rgb.g ) && ( rgb.g == rgb.b ) ) {
		hsl.z = rgb.r; // set luminance to any color
	} else {
		float d = maxColor - minColor;
		hsl.z = ( minColor + maxColor ) / 2.0;
		if( hsl.z < 0.5 ) {
			hsl.y = d / ( maxColor + minColor );
		} else {
			hsl.y = d / (2.0 - maxColor - minColor );
		}

		if( max == 0 ) {
			hsl.x = ( rgb.g - rgb.b ) / ( maxColor - minColor );
		} else if( max == 1 ) {
			hsl.x = 2.0 + ( rgb.b - rgb.r ) / ( maxColor - minColor );	
		} else {
			hsl.x = 4.0 + ( rgb.r - rgb.g ) / ( maxColor - minColor );
		}

		hsl.x /= 6.0; // bring it to a number between 0 and 1

		if( hsl.x < 0) {
			hsl.x += 1.0;
		}
	}

	return hsl *= vec3( 360.0, 255.0, 255.0 );
	//return hsl;
}

// Takes in a HSL format and returns RGB color(0-1.0)
vec3 HSLToRGB( vec3 hsl ) {
	vec3 rgb = vec3( 0.0 );

	hsl.x = mod( hsl.x, 260 ) / 360.0;
	hsl.y = hsl.y / 256.0;
	hsl.z = hsl.z / 256.0;

	float temp1 = 0.0, temp2 = 0.0, tempr = 0.0, tempg = 0.0, tempb = 0.0;

	if( hsl.y == 0.0 ) {
		rgb = vec3( hsl.z );
	} else {
		if( hsl.z < 0.5 ) {
			temp2 = hsl.z * ( 1.0 + hsl.y );
		} else {
			temp2 = ( hsl.z + hsl.y ) - ( hsl.z * hsl.y );
		}
		temp1 = 2 * hsl.z - temp2;

		tempr = hsl.x + 1.0 / 3.0;
		if( tempr > 1.0 ) {
			tempr -= 1.0;
		}
		tempg = hsl.x;
		tempb = hsl.x - 1.0 / 3.0;
		if( tempb < 0.0 ) {
			tempb += 1.0;
		}

		// Red
		if( tempr < 1.0 / 6.0 ) {
			rgb.r = temp1 + ( temp2 - temp1 ) * 6.0 * tempr;
		} else if ( tempr < 0.5 ) {
			rgb.r = temp2;
		} else if ( tempr < 2.0 / 3.0 ) {
			rgb.r = temp1 + ( temp2 - temp1 ) * ( ( 2.0 / 3.0 ) - tempr ) * 6.0;
		} else {
			rgb.r = temp1;
		}

		// Green
		if( tempg < 1.0 / 6.0 ) {
			rgb.g = temp1 + ( temp2 - temp1 ) * 6.0 * tempg;
		} else if ( tempg < 0.5 ) {
			rgb.g = temp2;
		} else if ( tempg < 2.0 / 3.0 ) {
			rgb.g = temp1 + ( temp2 - temp1 ) * ( ( 2.0 / 3.0 ) - tempg ) * 6.0;
		} else {
			rgb.g = temp1;
		}

		// Blue
		if( tempb < 1.0 / 6.0 ) {
			rgb.b = temp1 + ( temp2 - temp1 ) * 6.0 * tempb;
		} else if ( tempb < 0.5 ) {
			rgb.b = temp2;
		} else if ( tempb < 2.0 / 3.0 ) {
			rgb.b = temp1 + ( temp2 - temp1 ) * ( ( 2.0 / 3.0 ) - tempb ) * 6.0;
		} else {
			rgb.b = temp1;
		}
	}

	return rgb;
}
