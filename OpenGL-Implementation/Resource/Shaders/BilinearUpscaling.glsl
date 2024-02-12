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
	vec2 pixel = vec2( FragCoord.st * imageSize + 0.5 );
	vec2 frac = fract( pixel );
	pixel = ( floor( pixel ) / imageSize ) - vec2( pixelSize / 2.0 );

	vec3 C11 = vec3( texture( u_Texture, pixel + vec2( 0.0, 0.0) ).rgb );
	vec3 C21 = vec3( texture( u_Texture, pixel + vec2( pixelSize.x, 0.0 ) ).rgb );
	vec3 C12 = vec3( texture( u_Texture, pixel + vec2( 0.0 , pixelSize.y ) ).rgb );
	vec3 C22 = vec3( texture( u_Texture, pixel + pixelSize ).rgb );

	vec3 x1 = mix( C11.rgb, C21.rgb, frac.x );
	vec3 x2 = mix( C12.rgb, C22.rgb, frac.x );
	vec3 finalColor = mix( x1.rgb, x2.rgb, frac.y );

	FragColor = vec4( finalColor, 1.0 );
}