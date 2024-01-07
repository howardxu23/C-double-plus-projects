#include "stdafx.h"
#include "ColourCubeApp.h"

void ColourCubeApp::Init()
{
	shader = new SOF::ShaderProgram("shaders/vshader.glsl",
		"shaders/fshader.glsl");

    glm::vec3 vertexPositions[] = {
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f)
    };

    glm::vec4 vertexColors[] =
    {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    };

    int triangles[] = { 0,4,5,0,5,1, 1,5,6,1,6,2, 2,6,7,2,7,3, 3,7,4,3,4,0, 4,7,6,4,6,5, 3,0,1,3,1,2 };

    std::vector<glm::vec3> vertexPos;
    std::vector<glm::vec4> vertexColor;

    for (int i = 0; i < 36; i++)
    {
        vertexPos.push_back(vertexPositions[triangles[i]]);
        vertexColor.push_back(vertexColors[triangles[i]]);
    }
    //This is where the magic happens, and the SOF::Geometry class automatically creates vertex buffer and array objects,
    //and hooks up the attributes to the shader attributes
    geom = new SOF::Geometry(36); // 36 verts - 12 triangles. 
    geom->AddAttribute(vertexPos, "vertexPos");
    geom->AddAttribute(vertexColor, "vertexColor");
    geom->Finalize(shader);
   
    // some render states 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void ColourCubeApp::Draw()
{
    // model, view and projection 
    //model matrices
    glm::mat4 model = glm::rotate(glm::mat4(),angle,glm::vec3(0.707f,0.707f,0.0f));
    //glm::mat4 model = glm::mat4();
    //view matric
    glm::mat4 view = glm::lookAt(glm::vec3(-5.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //projectrion matrics
    glm::mat4 proj = glm::perspective(glm::radians(70.0f),
        (float)SOF::Window::GetWidth() / SOF::Window::GetHeight(), 0.01f, 100.0f);
    glm::mat4 mvp = proj * view * model;

    shader->Use(); // enable the shader 
    shader->SetUniformMat4("mvp", mvp); // set the model-view-projection matrix uniform in the shader program 
    geom->DrawPrimitives();
}
void ColourCubeApp::Update(float deltaT)
{
    
    angle += glm::radians(30.0f) * deltaT;
}