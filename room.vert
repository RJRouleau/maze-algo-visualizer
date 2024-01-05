#version 330 compatibility

layout(location = 0) in vec3 aVertex;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec4 vColor;

void main()
{
    vec4 vertex = vec4(aVertex, 1.0);

    vColor = gl_Color;

    mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = PVM * vertex;
}