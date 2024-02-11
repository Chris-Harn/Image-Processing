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
vec2 offsets[25] = vec2[]
(
	vec2( -offset_x * 2,  offset_y * 2 ),  vec2( -offset_x, offset_y * 2 ),  vec2( 0.0f, offset_y * 2 ),  vec2( offset_x, offset_y * 2 ),  vec2( offset_x * 2, offset_y * 2 ),
	vec2( -offset_x * 2,  offset_y ),      vec2( -offset_x, offset_y ),      vec2( 0.0f, offset_y ),      vec2( offset_x, offset_y ),      vec2( offset_x * 2, offset_y ),
	vec2( -offset_x * 2,  0.0f ),          vec2( -offset_x, 0.0f ),          vec2( 0.0f, 0.0f ),          vec2( offset_x, 0.0f ),          vec2( offset_x * 2, 0.0f ),
	vec2( -offset_x * 2,  -offset_y ),     vec2( -offset_x, -offset_y ),     vec2( 0.0f, -offset_y ),     vec2( offset_x, -offset_y ),     vec2( offset_x * 2, -offset_y ),
	vec2( -offset_x * 2,  -offset_y * 2 ), vec2( -offset_x, -offset_y * 2 ), vec2( 0.0f, -offset_y * 2 ), vec2( offset_x, -offset_y * 2 ), vec2( offset_x * 2, -offset_y * 2 )
);

// Low pass filter
float kernel[25] = float[]
(
	 1,  4,  6,  4,  1,
	 4, 16, 24, 16,  4,
	 6, 24, 36, 24,  6,
	 4, 16, 24, 16,  4,
	 1,  4,  6,  4,  1
);

void main() {
	for( int i = 0; i < 25; i++ )
		kernel[i] = (1.0f / 256.0f) * kernel[i];

	vec3 color = vec3( 0.0f );
	for( int i = 0; i < 25; i++ )
		color += vec3( texture( u_Texture, FragCoord.st + offsets[i] ) ) * kernel[i];

	FragColor = vec4( color, 1.0 );
}