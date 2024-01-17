#version 330 compatibility

layout(location = 0) in vec3 aVertex;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

flat out vec3 vNormal;
out vec3 vLight;
out vec3 vEye;
out vec3 vPosition;

const vec3 LightPosition = vec3(0., 0., 0.);

void main()
{
    vPosition = aVertex;
    vec4 vertex = vec4(aVertex, 1.0);

    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    vec4 ECposition = modelViewMatrix * vertex;

    mat3 normalMatrix = mat3(modelViewMatrix);
    vNormal = normalize(normalMatrix * aNormal);
    vLight = LightPosition - ECposition.xyz;
    vEye = vec3(0., 0., 0.) - ECposition.xyz;

    mat4 PVM = projectionMatrix * modelViewMatrix;
    gl_Position = PVM * vertex;
}