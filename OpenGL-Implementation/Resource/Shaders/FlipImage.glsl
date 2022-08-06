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
uniform bool u_FlipHorizontal;
uniform bool u_FlipVeritical;

void main() {
	vec2 NewCoord = FragCoord;
   if( u_FlipHorizontal == true ) {
		NewCoord.x = 1.0 - NewCoord.x;
   }

   if( u_FlipVeritical == true ) {
		NewCoord.y = 1.0 - NewCoord.y;
   }
   //NewCoord.y = 1.0 - NewCoord.y;
   FragColor = vec4( texture(u_Texture, NewCoord).rgb, 1.0 );
}