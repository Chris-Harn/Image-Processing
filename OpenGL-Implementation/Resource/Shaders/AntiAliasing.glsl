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

vec2 imageSize = vec2( 1280, 960 );
vec2 pixelSize = 1.0 / imageSize;

void main() {
	vec3 color = vec3( texture( u_Texture, FragCoord.st ) );

	vec3 b = vec3 ( texture( u_Texture, FragCoord.st + vec2( pixelSize.x, 0.0 ) ) );
	b += vec3 ( texture( u_Texture, FragCoord.st - vec2( pixelSize.x, 0.0 ) ) );
	b += vec3 ( texture( u_Texture, FragCoord.st + vec2( 0.0, pixelSize.y ) ) );
	b += vec3 ( texture( u_Texture, FragCoord.st - vec2( 0.0, pixelSize.y ) ) );

	b += vec3 ( texture( u_Texture, FragCoord.st + vec2(  pixelSize.x,   pixelSize.x ) ) );
	b += vec3 ( texture( u_Texture, FragCoord.st + vec2(  pixelSize.x,  -pixelSize.x ) ) );
	b += vec3 ( texture( u_Texture, FragCoord.st + vec2( -pixelSize.x,   pixelSize.x ) ) );
	b += vec3 ( texture( u_Texture, FragCoord.st + vec2( -pixelSize.x,  -pixelSize.x ) ) );

	b /= 8.0;
	color += b;
	color /= 2.0;

	FragColor = vec4( color, 1.0 );
}