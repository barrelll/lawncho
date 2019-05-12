#version 450 core
// The data we send back out!
layout (location = 0) out vec4 outColor;
// Interpolated values from the vertex shaders
in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

uniform BlobSettings 
{
	vec4 InnerColor;
	vec4 OuterColor;
	float RadiusInner;
	float RadiusOuter;
};

void main()
{
	float dx = UV.x - 0.5;
	float dy = UV.y - 0.5;
	float dist = sqrt(dx * dx + dy * dy);
	// Output color = color of the texture at the specified UV coord
        //outColor = mix(	texture(myTextureSampler, UV).b * InnerColor, OuterColor, 
	//			smoothstep(RadiusInner, RadiusOuter, dist));
	outColor.rgb = texture(myTextureSampler, UV).rgb;
	outColor.a = 1.0;
}
