#version 330 core
out vec4 fFragColor;

in vec4 vVertex; // en world space
in vec3 vNormal; // en world space
in mat4 vProjMat;

uniform vec3 cameraPos;

void main()
{
	vec3 normal = gl_FrontFacing ? vNormal : -vNormal;

	/* LAYER WEIGHT -> FACING */
	float blend = float(0.5);
	vec3 I = cameraPos.xyz - vVertex.xyz;
	vec3 Iview = (vProjMat[3][3] == 0.0) ? normalize(I) : vec3(0.0, 0.0, -1.0);
	vec3 N = normalize(normal);
	float facing = abs(dot(Iview, N));
	if (blend != 0.5) {
		blend = clamp(blend, 0.0, 0.99999);
		blend = (blend < 0.5) ? 2.0 * blend : 0.5 / (1.0 - blend);
		facing = pow(facing, blend);
	}
	facing = 1.0 - facing;

	/* TRASPARENT BSDF -> BSDF*/
	vec4 bsdf = vec4(1.0, 1.0, 1.0, 0.0);

	/* EMISSION -> EMISSION */
	vec4 color = vec4(0.06, 0.3, 0.8, 1.0);
	float strength = 1.0;
	vec4 emission = color * strength;

	/* MIX SHADER -> SHADER */
	vec4 shader = mix(bsdf, emission, facing);

	/* GEOMETRY -> BACKFACING */
	float backfacing = gl_FrontFacing ? 1.0 : 0.0;
	
	/* MIX SHADER -> SHADER */
	vec4 shader1 = mix(shader, bsdf, backfacing);

	/* OUTPUT MATERIAL -> RESULT */
	vec4 result = shader1;

	fFragColor = result;
}
