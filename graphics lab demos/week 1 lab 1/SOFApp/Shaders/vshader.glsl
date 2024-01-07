#version 330 
in vec3 vertexPos; // vertex position 
in vec4 vertexColor; // vertex color 
out vec4 fragColor; // color to pass to fragment shader 
uniform mat4 mvp; // model view projection 

void main() 
{   
    // pass through color 
    fragColor = vertexColor; 
    // transform position and pass to fragment shader 
    gl_Position = mvp * vec4(vertexPos, 1); 
} 