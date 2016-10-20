/*
 * Sample dummy shader to check the highlighter plugin.
 */

#version 430 core


layout (binding = 0) uniform sampler2D ground;

uniform float windowLength = 800;
uniform float windowHeigth = 600;
uniform bool renderAgeMap = false;

vec3 getTexValue(sampler2D tex, float x, float y)
{
    vec2 div = textureSize(tex,0)/vec2(windowLength,windowHeigth);
    return texture(tex, div*(gl_FragCoord.xy)/(textureSize(tex,0))).xyz;
}

out vec3 color;
const float COLOR_STEP=	1.5f;

vec3 renderHeightMap(float c)
{
    vec3 color = vec3(0.0);
    if (c < 0.5)
        color = vec3(0.0, 0.0, 0.25 + 1.5 * c);
    else if (c < 1.0)
        color = vec3(0.0, 2 * (c - 0.5), 1.0);
    else
    {
      c -= 1.0;
      if (c < 1.0 * COLOR_STEP)
           color = vec3(0.0, 0.5 + 0.5 * c / COLOR_STEP, 0.0);
      else if (c < 1.5 * COLOR_STEP)
           color = vec3(2 * (c - 1.0 * COLOR_STEP) / COLOR_STEP, 1.0, 0.0);
      else if (c < 2.0 * COLOR_STEP)
           color = vec3(1.0, 1.0 -(c - 1.5 * COLOR_STEP)/COLOR_STEP, 0);
      else if (c < 3.0 * COLOR_STEP)
           color = vec3(1.0 - 0.5 * (c - 2.0 * COLOR_STEP) / COLOR_STEP, 
                        0.5 - 0.25 * (c - 2.0 * COLOR_STEP) / COLOR_STEP, 
                        0);
      else if (c < 5.0 * COLOR_STEP)
           color = vec3(0.5 - 0.125 * (c - 3.0 *  COLOR_STEP) / (2*COLOR_STEP),
                    0.25 + 0.125 * (c - 3.0 * COLOR_STEP) / (2*COLOR_STEP),
                    0.375 * (c - 3.0 *  COLOR_STEP) / (2*COLOR_STEP));
      else if (c < 8.0 * COLOR_STEP)
           color = vec3(0.375 + 0.625 * (c - 5.0 * COLOR_STEP) / (3*COLOR_STEP),
                    0.375 + 0.625 * (c - 5.0 * COLOR_STEP) / (3*COLOR_STEP),
                    0.375 + 0.625 * (c - 5.0 * COLOR_STEP) / (3*COLOR_STEP));
      else
      {
            c -= 8.0 * COLOR_STEP;
            while (c > 2.0 * COLOR_STEP)
                    c -= 2.0 * COLOR_STEP;

            color = vec3(1, 1 , 1);
        }
    }
    return color;
}
void main() 
{
    if(renderAgeMap)
        color = vec3(getTexValue(ground,0.0,0.0).x);
    else
        color = renderHeightMap(getTexValue(ground,0.0,0.0).x );

}
