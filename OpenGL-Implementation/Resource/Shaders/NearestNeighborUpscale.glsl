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

void main() {
	vec2 pixel = vec2( FragCoord.st * imageSize );
	pixel = ( floor( pixel ) / imageSize ) + vec2( ( 1.0 / imageSize ) / 2 );
	vec3 color = vec3( texture( u_Texture, pixel ).rgb );

	FragColor = vec4( color, 1.0 );
}