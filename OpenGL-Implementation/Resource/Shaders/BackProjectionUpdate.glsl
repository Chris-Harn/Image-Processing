#shader vertex
#version 450 core

layout ( location = 0 ) in vec2 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

uniform sampler2D u_Texture;
uniform int u_color;

void main() {
	ivec2 imDims = textureSize( u_Texture, 0 );
	ivec2 imageCoordinates = ivec2( ( gl_VertexID ) % imDims.x, ( gl_VertexID ) / imDims.x );

	float intensity = 0.0;
	if( u_color == 0 ) {
		// Collect Red
		intensity = texelFetch( u_Texture, imageCoordinates, 0 ).r;
	} else if ( u_color == 1 ) {
		// Collect Green
		intensity = texelFetch( u_Texture, imageCoordinates, 0 ).g;
	} else {
		// Collect Blue
		intensity = texelFetch( u_Texture, imageCoordinates, 0 ).b;
	}

	// Cap as some values are sampled outside frame
	intensity = min( max( intensity, 0.0 ), 1.0 );

	// Shift so histogram fits 0 to 511
	intensity *= 0.9998;

	gl_Position = vec4( ( intensity * 2.0 ) - 1.0, 0.0, 0.0, 1.0 );
	gl_PointSize = 1.0;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 FragColor;

void main() {
	FragColor = vec4( 1.0 );
}