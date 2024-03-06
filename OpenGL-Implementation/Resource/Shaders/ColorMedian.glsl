#shader vertex
#version 450 core

layout ( location = 0 ) in vec2 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

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

const float offset_x = 1.0f / 640.0f;
const float offset_y = 1.0f / 480.0f;

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
}

