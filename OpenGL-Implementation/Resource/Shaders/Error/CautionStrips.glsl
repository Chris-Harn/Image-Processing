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

uniform float iTime;

float interval = 360.0; // Width of Bands

void main() {
	// Caution Yellow
	vec4 yellow = vec4( 0.972656, 0.835938, 0.238281, 1.0 );

	// Calculate diagonal that is black depending on built in x/y screen coordinates
	float a = step( mod(gl_FragCoord.x + gl_FragCoord.y, interval) / (interval - 1.0), 0.5 );
	
	FragColor = vec4( yellow.rgb * a, 1.0 );
}