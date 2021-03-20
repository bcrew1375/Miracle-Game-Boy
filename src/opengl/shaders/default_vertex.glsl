#version 110

attribute vec2 vertexCoordinates;
varying vec2 textureCoordinates;
uniform float scaleX;
uniform float scaleY;

void main()
{
    vec2 vertexVerticalFlip = vec2(vertexCoordinates.x, vertexCoordinates.y * -1.0);
    vec2 vertexScaleCoordinates = vec2(vertexVerticalFlip.x * scaleX, vertexVerticalFlip.y * scaleY);
    gl_Position = vec4(vertexScaleCoordinates, 0.0, 1.0);
    textureCoordinates = vertexCoordinates * vec2(0.5, 0.5) + vec2(0.5, 0.5);
}
