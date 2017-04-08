#version 120

uniform vec2 vFollow;
uniform float circleRadius;

uniform vec4 colorIn;
uniform vec4 colorOut;

uniform vec4 colorHanabi;
uniform float ratioHanabi;
void main()
{

    float distance = sqrt((vFollow.x-gl_FragCoord.x)*(vFollow.x-gl_FragCoord.x)
                          + (vFollow.y-gl_FragCoord.y)*(vFollow.y-gl_FragCoord.y));
    float colorRatio = distance / (circleRadius*2.);

    gl_FragColor = colorRatio * colorOut + (1. - colorRatio) * ((1.- ratioHanabi)*(colorIn)
                    + ratioHanabi * colorHanabi);
}
