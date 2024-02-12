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
vec2 twoPixelSize = 2.0 / imageSize;
float c_x0 = -1.0;
float c_x1 =  0.0;
float c_x2 =  1.0;
float c_x3 =  2.0;

vec3 CubicLagrange (vec3 A, vec3 B, vec3 C, vec3 D, float t);

void main() {
	vec2 pixel = vec2( FragCoord.st * imageSize + 0.5 );

	vec2 frac = fract( pixel );
	pixel = ( floor( pixel ) / imageSize ) - vec2( pixelSize / 2.0 );

	vec3 C00 = vec3( texture( u_Texture, pixel + vec2( -pixelSize.x, -pixelSize.y ) ).rgb );
	vec3 C10 = vec3( texture( u_Texture, pixel + vec2( 0.0, -pixelSize.y ) ).rgb );
	vec3 C20 = vec3( texture( u_Texture, pixel + vec2( pixelSize.x, -pixelSize.y ) ).rgb );
	vec3 C30 = vec3( texture( u_Texture, pixel + vec2( twoPixelSize.x, -pixelSize.y ) ).rgb );

	vec3 C01 = vec3( texture( u_Texture, pixel + vec2( -pixelSize.x, 0.0) ).rgb );
	vec3 C11 = vec3( texture( u_Texture, pixel + vec2( 0.0, 0.0 ) ).rgb );
	vec3 C21 = vec3( texture( u_Texture, pixel + vec2( pixelSize.x, 0.0 ) ).rgb );
	vec3 C31 = vec3( texture( u_Texture, pixel + vec2( twoPixelSize.x, 0.0 ) ).rgb );

	vec3 C02 = vec3( texture( u_Texture, pixel + vec2( -pixelSize.x, pixelSize.y) ).rgb );
	vec3 C12 = vec3( texture( u_Texture, pixel + vec2( 0.0, pixelSize.y ) ).rgb );
	vec3 C22 = vec3( texture( u_Texture, pixel + vec2( pixelSize.x, pixelSize.y ) ).rgb );
	vec3 C32 = vec3( texture( u_Texture, pixel + vec2( twoPixelSize.x, pixelSize.y ) ).rgb );

	vec3 C03 = vec3( texture( u_Texture, pixel + vec2( -pixelSize.x, twoPixelSize.y ) ).rgb );
	vec3 C13 = vec3( texture( u_Texture, pixel + vec2( 0.0, twoPixelSize.y ) ).rgb );
	vec3 C23 = vec3( texture( u_Texture, pixel + vec2( pixelSize.x, twoPixelSize.y ) ).rgb );
	vec3 C33 = vec3( texture( u_Texture, pixel + vec2( twoPixelSize.x, twoPixelSize.y ) ).rgb );

	vec3 CP0X = CubicLagrange( C00, C10, C20, C30, frac.x );
	vec3 CP1X = CubicLagrange( C01, C11, C21, C31, frac.x );
	vec3 CP2X = CubicLagrange( C02, C12, C22, C32, frac.x );
	vec3 CP3X = CubicLagrange( C03, C13, C23, C33, frac.x );

	vec3 finalColor = CubicLagrange( CP0X, CP1X, CP2X, CP3X, frac.y );

	FragColor = vec4( finalColor, 1.0 );
}

vec3 CubicLagrange (vec3 A, vec3 B, vec3 C, vec3 D, float t) {
    return
        A * 
        (
            (t - c_x1) / (c_x0 - c_x1) * 
            (t - c_x2) / (c_x0 - c_x2) *
            (t - c_x3) / (c_x0 - c_x3)
        ) +
        B * 
        (
            (t - c_x0) / (c_x1 - c_x0) * 
            (t - c_x2) / (c_x1 - c_x2) *
            (t - c_x3) / (c_x1 - c_x3)
        ) +
        C * 
        (
            (t - c_x0) / (c_x2 - c_x0) * 
            (t - c_x1) / (c_x2 - c_x1) *
            (t - c_x3) / (c_x2 - c_x3)
        ) +       
        D * 
        (
            (t - c_x0) / (c_x3 - c_x0) * 
            (t - c_x1) / (c_x3 - c_x1) *
            (t - c_x2) / (c_x3 - c_x2)
        );
}