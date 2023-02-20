#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// TODO(student): Declare various other uniforms
uniform int mixBool;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = texture2D(texture_1, texcoord);
	vec4 color1 = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);

	if (mixBool == 1)
		color = mix(color1, color2, .5f);


	float alpha = color.a;
	if (alpha < 0.5f){
		discard;
	}

    out_color = color;
}
