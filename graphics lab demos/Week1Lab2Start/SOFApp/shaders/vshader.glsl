#version 330
in vec3 vertexPos;
in vec4 vertexColor;
in vec3 vertexNormal;
in vec2 vertexUV;
out vec4 fragColor; // color to pass to fragment shader
out vec3 pixelPos; // passthrough - world space position
out vec3 pixelNorm; // passthrough - world space normal
out vec2 pixelUV; //passthrough-uv coodinate
uniform mat4 model;
uniform mat4 viewProj; // view-projection

void main()
{
// pass through
fragColor = vertexColor;
// transform vertex position and normal to world space
pixelPos = vec3(model * vec4( vertexPos, 1.0f));
pixelNorm = vec3(model * vec4( vertexNormal, 0.0f));
//passtrhough
pixelUV=vertexUV;
// final vertex position
gl_Position = viewProj * vec4( pixelPos, 1.0f);
}