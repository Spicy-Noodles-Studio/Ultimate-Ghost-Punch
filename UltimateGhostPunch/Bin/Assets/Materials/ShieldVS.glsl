#version 330 core
in vec4 vertex;
in vec4 normal;

out vec4 vVertex; // en world space
out vec3 vNormal; // en world space
out mat4 vModelMat;
out mat4 vViewMat;
out mat4 vProjMat;
out mat4 vNormalMat;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat4 normalMat;

void main() {
    vVertex = vec4(modelMat * vertex);
    vNormal = mat3(normalMat) * normal.xyz;

	vModelMat = modelMat;
	vViewMat = viewMat;
	vProjMat = projectionMat;
	vNormalMat = normalMat;

    gl_Position =  projectionMat * viewMat *  modelMat * vertex;
}