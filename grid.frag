#version 330 compatibility

in vec3 vVertex;
flat in int vSelected;

uniform vec3 uColor;
uniform float uTime;

out vec4 color;

void main()
{
	if (vVertex.x > (-0.8) && vVertex.x < (0.8) && vVertex.y > -0.8 && vVertex.y < 0.8)
		color = vec4(uColor, 1.0);
	else {
		if (vSelected == 1) {
			// use time to creating a flashing effect around a selected tile
			if (int(uTime * 10.0) % 2 == 0)
				color = vec4(0.0, 1.0, 0.325, 1.0);
		}			
		else
			color = vec4(0.0, 0.0, 0.0, 1.0);
	}		
}