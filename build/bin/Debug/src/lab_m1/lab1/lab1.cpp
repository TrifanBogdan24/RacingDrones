#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab1::Lab1() : red(0.f), green(0.f), blue(0.f), posX(0.f), posY(0.f), posZ(0.f), objectCase(0), degrees(0.f)
{
    // Initialize variables
    changeClearColor();
    changeObject();
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load more meshes from solution
    {
        Mesh* boxMesh = new Mesh("box");
        boxMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[boxMesh->GetMeshID()] = boxMesh;

        Mesh* sphereMesh = new Mesh("sphere");
        sphereMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[sphereMesh->GetMeshID()] = sphereMesh;

        Mesh* teapotMesh = new Mesh("teapot");
        teapotMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[teapotMesh->GetMeshID()] = teapotMesh;

        Mesh* archerMesh = new Mesh("archer");
        archerMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters/Archer"), "Archer.fbx");
        meshes[archerMesh->GetMeshID()] = archerMesh;
    }
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer
    glClearColor(red, green, blue, 1.f);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the box mesh
    RenderMesh(meshes["box"], glm::vec3(1.f, 0.5f, 0.f), glm::vec3(0.5f));

    // Render the box mesh again with different properties
    RenderMesh(meshes["box"], glm::vec3(-1.f, 0.5f, 0.f));

    // Render the selected object (sphere, teapot, or archer) based on input
    RenderMesh(meshes[objectName], glm::vec3(posX, posY, posZ), objScale);

    // TODO(student): You can add more objects to be rendered here.
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Handle object movement with key hold events
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        // Move the object on the Z axis
        if (window->KeyHold(GLFW_KEY_W)) posZ -= deltaTime * 2.f;
        if (window->KeyHold(GLFW_KEY_S)) posZ += deltaTime * 2.f;

        // Move the object on the X axis
        if (window->KeyHold(GLFW_KEY_A)) posX -= deltaTime * 2.f;
        if (window->KeyHold(GLFW_KEY_D)) posX += deltaTime * 2.f;

        // Move the object on the Y axis
        if (window->KeyHold(GLFW_KEY_Q)) posY -= deltaTime * 2.f;
        if (window->KeyHold(GLFW_KEY_E)) posY += deltaTime * 2.f;
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Handle color change
    if (key == GLFW_KEY_F) {
        colorCase = (colorCase + 1) % 4;
        changeClearColor();
    }

    // Cycle through objects
    if (key == GLFW_KEY_X) {
        objectCase = (objectCase + 1) % 3;
        changeObject();
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event if needed
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event if needed
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event if needed
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event if needed
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Add mouse scroll event if needed
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event if needed
}


void Lab1::changeClearColor()
{
    // Change the background color based on the color case
    switch (colorCase)
    {
    case 1:
        red = 0.286f;
        green = 0.074f;
        blue = 0.184f;
        break;

    case 2:
        red = 0.635f;
        green = 0.086f;
        blue = 0.376f;
        break;

    case 3:
        red = 0.811f;
        green = 0.188f;
        blue = 0.517f;
        break;

    default:
        red = 0.f;
        green = 0.f;
        blue = 0.f;
        break;
    }
}


void Lab1::changeObject()
{
    // Change the rendered object based on the object case
    switch (objectCase)
    {
    case 1:
        objectName = "sphere";
        objScale = glm::vec3(1.f);
        break;

    case 2:
        objectName = "teapot";
        objScale = glm::vec3(1.f);
        break;

    default:
        objectName = "archer";
        objScale = glm::vec3(0.01f);
        break;
    }
}
