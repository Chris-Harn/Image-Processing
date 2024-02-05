#shader vertex
#version 460 core
layout ( location = 0 ) in vec2 vertex; 

out VS_OUT{
	vec2 TexCoords;
	flat int index;
} vs_out;

uniform mat4 transforms[200];
uniform mat4 projection;

void main() {
   gl_Position = projection * transforms[gl_InstanceID] * vec4( vertex.xy, 0.0, 1.0 );
   vs_out.index = gl_InstanceID;
   vs_out.TexCoords = vertex.xy;
   vs_out.TexCoords.y = 1.0f - vs_out.TexCoords.y;
}

#shader fragment
#version 460 core

in VS_OUT{
	vec2 TexCoords;
	flat int index;
} fs_in;

out vec4 FragColor;

uniform sampler2DArray text;
uniform int letterMap[400];
uniform vec3 textColor;

void main() {
   vec4 sampled = vec4( 1.0, 1.0, 1.0, texture(text, vec3(fs_in.TexCoords.xy, letterMap[fs_in.index])).r );
   FragColor = vec4( textColor, 1.0 ) * sampled;
}