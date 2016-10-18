/*
 * Sample dummy shader to check the highlighter plugin.
 */

#version 430 core


layout (binding = 0) uniform sampler2D ground;

uniform ivec2 WindowDimension = ivec2(800,600);

vec3 getTexValue(sampler2D tex, float x, float y)
{
    vec2 div = textureSize(tex,0)/vec2(WindowDimension);
    return texture(tex, (div*gl_FragCoord.xy)/(textureSize(tex,0))).xyz;
}

out vec3 color;

void main() 
{

    color = getTexValue(ground,0,0);
 
}
