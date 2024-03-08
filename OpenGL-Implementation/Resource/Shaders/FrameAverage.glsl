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

uniform sampler2D u_FrameOne;
uniform sampler2D u_FrameTwo;
uniform sampler2D u_FrameThree;

void main() {
	vec3 frameOne = texture( u_FrameOne, FragCoord ).rgb;
	vec3 frameTwo = texture( u_FrameTwo, FragCoord ).rgb;
	vec3 frameThree = texture( u_FrameThree, FragCoord ).rgb;

	vec3 rgb = ( frameOne + frameTwo + frameThree ) / 3.0;
	//vec3 rgb = ( frameOne + frameTwo ) / 2.0;

   FragColor = vec4( rgb, 1.0 );
}