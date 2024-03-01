#shader fragment
#version 450 core

in vec2 FragCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;

const float offset_x = 1.0f / 3840.0f;
const float offset_y = 1.0f / 2160.0f;

const mat3 RGB2CIE = mat3( 0.4124564, 0.3575761, 0.1804375,
						   0.2126729, 0.7151522, 0.0721750,
						   0.0193339, 0.1191920, 0.9503041 );

const mat3 CIE2RGB = mat3( 3.2404542, -1.5371385, -0.4985314,
						   -0.9692660, 1.8760108, 0.0415560,
						   0.0556434, -0.2040259, 1.0572252 );

vec3 RGBToCIEXYZ( vec3 rgb );
vec3 CIEXYZToRGB( vec3 Yxy );
float averageValues( float arr[9] );
float insertionSort( float arr[9] );

// Sample values around main pixel
vec2 offsets[9] = vec2[]
(
	vec2( -offset_x,  offset_y ), vec2( 0.0f,  offset_y ), vec2( offset_x,  offset_y ),
	vec2( -offset_x,  0.0f ),     vec2( 0.0f,  0.0f ),     vec2( offset_x,  0.0f ), 
	vec2( -offset_x, -offset_y ), vec2( 0.0f, -offset_y ), vec2( offset_x, -offset_y )
);

void main() {
	vec3 colors[9];

	for( int i = 0; i < 9; i++ ) {
		colors[i] = texture( u_Texture, FragCoord.st + offsets[i] ).rgb;
	}

	// Sort Red Color
	int i, j;
	float key;
	for( i = 1; i < 9; i++ ) {
		key = colors[i].r;
		j = i - 1;

		while( j >= 0 && colors[j].r > key ) {
			colors[j + 1].r = colors[j].r;
			j = j - 1;
		}
		colors[j + 1].r = key;
	}

	// Sort Green Color
	for( i = 1; i < 9; i++ ) {
		key = colors[i].g;
		j = i - 1;

		while( j >= 0 && colors[j].g > key ) {
			colors[j + 1].g = colors[j].g;
			j = j - 1;
		}
		colors[j + 1].g = key;
	}

	// Sort Blue Color
	for( i = 1; i < 9; i++ ) {
		key = colors[i].b;
		j = i - 1;

		while( j >= 0 && colors[j].b > key ) {
			colors[j + 1].b = colors[j].b;
			j = j - 1;
		}
		colors[j + 1].b = key;
	}

	// Place Median of all three colors in new pixel
	FragColor = vec4( colors[4], 1.0 );

/*
	float color[8];
	float luminance[8];
	vec3 center = RGBToCIEXYZ( texture( u_Texture, FragCoord.st ).rgb );

	vec3 xyz = vec3( 0.0 );
	for( int i = 0; i < 8; i++ ) {
		xyz = RGBToCIEXYZ( texture( u_Texture, FragCoord.st + offsets[i] ).rgb );
		color[i] = xyz.r;
		luminance[i] = xyz.g;
	}

	// Sort color
	int i, j;
	float key;
	for( i = 1; i < 8; i++ ) {
		key = color[i];
		j = i - 1;

		while( j >= 0 && color[j] > key ) {
			color[j + 1] = color[j];
			j = j - 1;
		}
		color[j + 1] = key;
	}

	// Sort luminance
	for( i = 1; i < 8; i++ ) {
		key = luminance[i];
		j = i - 1;

		while( j >= 0 && luminance[j] > key ) {
			luminance[j + 1] = luminance[j];
			j = j - 1;
		}
		luminance[j + 1] = key;
	}

	// Test to see if needs color and luminance needs replacment
	if( center.r < color[0] ) {
		center.r = color[0];
	} else if ( center.r > color[7] ) {
		center.r = color[7];
	}
	if( center.g < luminance[0] ) {
		center.g = luminance[0];
	} else if ( center.g > luminance[7] ) {
		center.g = luminance[7];
	}

	xyz = CIEXYZToRGB( center );
	FragColor = vec4( xyz, 1.0 );

	*/
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

#shader vertex
#version 450 core

layout ( location = 0 ) in vec2 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

out vec2 FragCoord;

void main() {
   gl_Position = vec4( aPos, 0.0, 1.0 );
   FragCoord = aTexCoords;
}