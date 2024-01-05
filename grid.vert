#version 330 compatibility

layout(location = 0) in vec3 aVertex;

uniform int uSelected;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 vVertex;
flat out int vSelected;

void main()
{
    vec4 vertex = vec4(aVertex, 1.0);

    vVertex = aVertex;
    vSelected = uSelected;

    mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = PVM * vertex;
}