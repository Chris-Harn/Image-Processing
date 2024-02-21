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

// Tells where to sample values around main pixel
vec2 offsets[9] = vec2[]
(
	vec2( -offset_x,  offset_y ), vec2( 0.0f,  offset_y ), vec2( offset_x,  offset_y ),
	vec2( -offset_x,  0.0f ),     vec2( 0.0f,  0.0f ),     vec2( offset_x,  0.0f ),
	vec2( -offset_x, -offset_y ), vec2( 0.0f, -offset_y ), vec2( offset_x, -offset_y )
);

// High pass filter
float kernel[9] = float[]
(
	  0,  -0.5,  0,
	 -0.5,   3, -0.5,
	  0,  -0.5,  0
);

// Sharpened filter (2 * identiy - blur )
float kernel2[9] = float[]
(
	  -1.0,  -1.0,  -1.0,
	  -1.0,    17,  -1.0,
	  -1.0,  -1.0,  -1.0
);

// Sharpened filter (2 * identiy - gaussian blur )
float kernel3[9] = float[]
(
	  -0.0023,  -0.0432,  -0.0023,
	  -0.0432,   0.8180,  -0.0432,
	  -0.0023,  -0.0432,  -0.0023
);

void main() {
	vec3 color = vec3( 0.0f );
	for( int i = 0; i < 9; i++ )
		//color += vec3( texture( u_Texture, FragCoord.st + offsets[i] ) ) * kernel[i];
		color += vec3( texture( u_Texture, FragCoord.st + offsets[i] ) ) * kernel2[i] * 1.0 / 9.0;
		//color += vec3( texture( u_Texture, FragCoord.st + offsets[i] ) ) * kernel3[i];

	FragColor = vec4( color, 1.0 );
}