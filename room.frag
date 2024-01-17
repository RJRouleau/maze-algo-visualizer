#version 330 compatibility

const float uKa = 0.1;
const float uKd = 0.5;
const float uKs = 0.4;
const vec3 uSpecularColor = vec3(1., 1., 1.);
const float uShininess = 12.0;

uniform vec3 uColor;

flat in vec3 vNormal;
in vec3 vLight;
in vec3 vEye;
in vec3 vPosition;

out vec4 color;

void main()
{	
	vec3 myColor = uColor;

	// Setup for lighting
	vec3 Normal = normalize(vNormal);
	vec3 Light = normalize(vLight);
	vec3 Eye = normalize(vEye);

	// Do Ambient lighting
	vec3 ambient = uKa * myColor;

	// Do Diffuse lighting
	float dd = max(dot(Normal, Light), 0.);
	vec3 diffuse = uKd * dd * myColor;

	// Do Specular lighting
	float ss = 0.;
	if (dot(Normal, Light) > 0.)
	{
		vec3 ref = normalize(reflect(-Light, Normal));
		ss = pow(max(dot(Eye, ref), 0.), uShininess);
	}
	vec3 specular = uKs * ss * uSpecularColor;

	// Combine Ambient + Diffuse + Specular
	color = vec4(ambient + diffuse + specular, 1.);
}