#shader vertex
#version 450 core
layout( location = 0 ) in vec2 Pos;
layout( location = 1 ) in vec2 TexCoords;

out vec2 FragCoord;

void main() {
	gl_Position = vec4( Pos, 0.0, 1.0 );
	FragCoord = TexCoords;
}

#shader fragment
#version 450 core

in vec2 FragCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform float u_Gamma;

void main() {
   FragColor = vec4( pow( texture( u_Texture, FragCoord ).rgb, vec3( 1.0 / u_Gamma )), 1.0 );
}