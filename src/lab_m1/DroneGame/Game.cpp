#include "Game.h"
#include "Drone.h"
#include "Transform3D.h"
#include "Object3D.h"
#include "Camera.h"
#include "CameraType.h"
#include "Object3D.h"
#include "Player.h"
#include "ViewportArea.h"
#include "Obstacles/Tree.h"

#include <vector>
#include <string>
#include <iostream>

// For Randoms
#include <cstdlib>
#include <ctime>

#include <iomanip>


// RGB(117, 117, 117)
#define COLOR_GREY glm::vec3(117.f/255.f, 117.f/255.f, 117.f/255.f)

// RGB(0, 0, 0)
#define COLOR_BLACK glm::vec3(0.f/255.f, 0.f/255.f, 0.f/255.f)

// RGB(0, 0, 255)
#define COLOR_BLUE glm::vec3(0.f/255.f, 0.f/255.f, 255.f/255.f)

// RGB(6, 56, 7)
#define COLOR_GREEN_TREE_CONE glm::vec3(6.f/255.f, 56.f/255.f, 7.f/255.f)

// RGB(54, 24, 2)
#define COLOR_BROWN_TREE_TRUNK glm::vec3(54.f/255.f, 24.f/255.f, 2.f/255.f)

// RGB(211, 217, 227)
#define COLOR_WHITE_DOME_CUBE glm::vec3(211.f/255.f, 217.f/255.f, 227.f/255.f)


#define INITIAL_DRONE_POS glm::vec3(0.f, 1.5f, 0.f)



#define MAP_LENGTH 150.f

#define Z_FAR (100.f)
#define Z_NEAR (.01f)

using namespace std;
using namespace m1;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Game::Game()
{
}


Game::~Game()
{
}


void Game::Init()
{
    this->isDemo = true;
    std::srand(static_cast<unsigned>(std::time(0)));

    camera = new gamecamera::Camera();



    this->InitDomes();    // Place the DOMES first !!!
    this->InitForest();
    this->InitCheckPoints();
    this->InitDrones();





    Shader* simpleShader = new Shader("VertexColor");
    simpleShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneGame", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    simpleShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneGame", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    simpleShader->CreateAndLink();
    shaders[simpleShader->GetName()] = simpleShader;
    if (simpleShader->program == 0) {
        std::cerr << "Shader 'VertexColor' has not been compiled or linked correctly!" << std::endl;
        return;
    }

    Shader* terrainShader = new Shader("TerrainShader");
    terrainShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneGame", "shaders", "TerrainVertex.glsl"), GL_VERTEX_SHADER);
    terrainShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneGame", "shaders", "TerrainFragment.glsl"), GL_FRAGMENT_SHADER);
    terrainShader->CreateAndLink();
    shaders[terrainShader->GetName()] = terrainShader;
    if (terrainShader->program == 0) {
        std::cerr << "Shader 'TerrainShader' has not been compiled or linked correctly!" << std::endl;
        return;
    }

    polygonMode = GL_FILL;

    Mesh* terrainMesh = Object3D::CreateTerrain("Terrain");
    meshes["Terrain"] = terrainMesh;

    Mesh* mesh = Object3D::CreateBox("box");
    meshes[mesh->GetMeshID()] = mesh;







    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    left = -10;
    right = 10;
    bottom = -10;
    top = 10;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);



    terrainModelMatrix =
        glm::mat4(1.0f)
        * Transform3D::Scale(MAP_LENGTH, 0.1f, MAP_LENGTH)
        * Transform3D::Translate(-MAP_LENGTH / 2.f, 0.f, -MAP_LENGTH / 2.f);


    float fov = 70.f;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    // void Set(const glm::vec3 & position, const glm::vec3 & center, const glm::vec3 & up)
    camera->Set(glm::vec3(0, 2.f, 3.5f), glm::vec3(0, 2, 0), glm::vec3(0, 2, 0));

}


void Game::InitDrones()
{
    glm::ivec2 resolution = window->GetResolution();

    Drone droneDemo = Drone(
        INITIAL_DRONE_POS, MAP_LENGTH,
        &trees, &domes, &checkPoints
    );
    InitDroneMeshes(droneDemo, "Demo");
    this->demoPlayer = Player(
        Player::PlayerType::DEMO_PLAYER,
        droneDemo, &this->checkPoints,
        ViewportArea(0.f, 0.f, resolution.x, resolution.y),
        ViewportArea(resolution.x / 2.f + 0.5f, 0, resolution.x / 2.f, resolution.y)

    );


    Drone dronePlayer1 = Drone(
        INITIAL_DRONE_POS, MAP_LENGTH,
        &trees, &domes, &checkPoints
    );
    InitDroneMeshes(dronePlayer1, "Player1");
    this->player1 = Player(
        Player::PlayerType::FIRST_PLAYER,
        dronePlayer1, &this->checkPoints,
        ViewportArea(0.f, 0.f, resolution.x, resolution.y),
        ViewportArea(resolution.x / 2.f + 0.5f, 0, resolution.x / 2.f, resolution.y)
    );


    Drone dronePlayer2 = Drone(
        INITIAL_DRONE_POS, MAP_LENGTH,
        &trees, &domes, &checkPoints
    );
    InitDroneMeshes(dronePlayer2, "Player2");
    this->player2 = Player(
        Player::PlayerType::SECOND_PLAYER,
        dronePlayer2, &this->checkPoints,
        ViewportArea(resolution.x / 2.f + 0.5f, 0, resolution.x / 2.f, resolution.y),
        ViewportArea(resolution.x / 2.f + 0.5f, 0, resolution.x / 2.f, resolution.y)
    );




}

void Game::InitDroneMeshes(Drone& drone, string name)
{
    Mesh* meshArm1 = Object3D::CreateBox(name + "DroneArm1");
    meshes[meshArm1->GetMeshID()] = meshArm1;
    drone.arm1.mesh = meshArm1;

    Mesh* meshArm2 = Object3D::CreateBox(name + "DroneArm2");
    meshes[meshArm2->GetMeshID()] = meshArm2;
    drone.arm2.mesh = meshArm2;

    Mesh* meshLeg1 = Object3D::CreateBox(name + "DroneLeg1");
    meshes[meshLeg1->GetMeshID()] = meshLeg1;
    drone.leg1.mesh = meshLeg1;


    Mesh* meshLeg2 = Object3D::CreateBox(name + "DroneLeg2");
    meshes[meshLeg2->GetMeshID()] = meshLeg2;
    drone.leg2.mesh = meshLeg2;


    Mesh* meshLeg3 = Object3D::CreateBox(name + "DroneLeg3");
    meshes[meshLeg3->GetMeshID()] = meshLeg3;
    drone.leg3.mesh = meshLeg3;


    Mesh* meshLeg4 = Object3D::CreateBox(name + "DroneLeg4");
    meshes[meshLeg4->GetMeshID()] = meshLeg4;
    drone.leg4.mesh = meshLeg4;


    Mesh* meshPropeller1 = Object3D::CreateBox(name + "DronePropeller1");
    meshes[meshPropeller1->GetMeshID()] = meshPropeller1;
    drone.propeller1.mesh = meshPropeller1;


    Mesh* meshPropeller2 = Object3D::CreateBox (name + "DronePropeller2");
    meshes[meshPropeller2->GetMeshID()] = meshPropeller2;
    drone.propeller2.mesh = meshPropeller2;


    Mesh* meshPropeller3 = Object3D::CreateBox(name + "DronePropeller3");
    meshes[meshPropeller3->GetMeshID()] = meshPropeller3;
    drone.propeller3.mesh = meshPropeller3;


    Mesh* meshPropeller4 = Object3D::CreateBox(name + "DronePropeller4");
    meshes[meshPropeller4->GetMeshID()] = meshPropeller4;
    drone.propeller4.mesh = meshPropeller4;


    Mesh* meshDroneArrowCylinder = Object3D::CreateCylinder(name + "DroneArrowCylinder");
    meshes[meshDroneArrowCylinder->GetMeshID()] = meshDroneArrowCylinder;
    drone.droneArrow.cylinder.mesh = meshDroneArrowCylinder;

    Mesh* meshDroneArrowCone = Object3D::CreateCone(name + "DroneArrowCone");
    meshes[meshDroneArrowCone->GetMeshID()] = meshDroneArrowCone;
    drone.droneArrow.cone.mesh = meshDroneArrowCone;

    Mesh* meshCheckPointArrowCylinder = Object3D::CreateCylinder(name + "CheckPointArrowCylinder");
    meshes[meshCheckPointArrowCylinder->GetMeshID()] = meshCheckPointArrowCylinder;
    drone.checkPointArrow.cylinder.mesh = meshCheckPointArrowCylinder;

    Mesh* meshCheckPointArrowCone = Object3D::CreateCone(name + "CheckPointArrowCone");
    meshes[meshCheckPointArrowCone->GetMeshID()] = meshCheckPointArrowCone;
    drone.checkPointArrow.cone.mesh = meshCheckPointArrowCone;
}



void Game::InitForest()
{
    int idxTree = 0;
    InitTrees(rand() % 10 + 60, idxTree, 2);
    InitTrees(rand() % 10 + 40, idxTree, 3);
    InitTrees(rand() % 5 + 20, idxTree, 4);
}


void Game::InitDomes()
{

    vector<glm::vec3> semiSphereColors;

    semiSphereColors.push_back(glm::vec3(12.f / 255.f, 175.f / 255.f, 196.f / 255.f));  // RGB(12, 175, 196)
    semiSphereColors.push_back(glm::vec3(12.f / 255.f, 95.f / 255.f, 196.f / 255.f));   // RGB(12, 96, 196)
    semiSphereColors.push_back(glm::vec3(15.f / 255.f, 12.f / 255.f, 196.f / 255.f));   // RGB(15, 12, 196)
    semiSphereColors.push_back(glm::vec3(103.f / 255.f, 6.f / 255.f, 214.f / 255.f));   // RGB(103, 6, 214)

    int numDomes = rand() % 5 + 15;


    for (int i = 0; i < numDomes; i++) {
        string name = "";
        
        name = "Dome" + to_string(i) + "-Cube";
        Mesh* cubeMesh = Object3D::CreateBox(name);
        meshes[cubeMesh->GetMeshID()] = cubeMesh;
        
        name = "Dome" + to_string(i) + "-SemiSphere";
        Mesh* semiSphereMesh = Object3D::CreateSphere(name);
        meshes[semiSphereMesh->GetMeshID()] = semiSphereMesh;

        float length = 4.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (5.5f - 4.0f));
        float angleOY = static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 180.f);
        glm::vec3 position = FindNewDomeCoordinate(length);
        glm::vec3 semiSphereColor = semiSphereColors[i % semiSphereColors.size()];
        
        this->domes.push_back(
            Dome(position, length, angleOY, cubeMesh, semiSphereMesh, COLOR_WHITE_DOME_CUBE, semiSphereColor)
        );
    }
}



void Game::InitCheckPoints()
{
    int numCheckPoints = std::rand() % 5 + 25;

    for (int i = 0; i < numCheckPoints; i++) {
        string name = "";

        name = "Pillar" + to_string(i) + "-LeftPillar";
        Mesh* leftPillar = Object3D::CreateBox(name);
        meshes[leftPillar->GetMeshID()] = leftPillar;

        name = "Pillar" + to_string(i) + "-RightPillar";
        Mesh* rightPillar = Object3D::CreateBox(name);
        meshes[rightPillar->GetMeshID()] = rightPillar;

        name = "Pillar" + to_string(i) + "-LowerLine";
        Mesh* lowerLine = Object3D::CreateBox(name);
        meshes[lowerLine->GetMeshID()] = lowerLine;

        name = "Pillar" + to_string(i) + "-UpperLine";
        Mesh* upperLine = Object3D::CreateBox(name);
        meshes[upperLine->GetMeshID()] = upperLine;

        float pillarHeight = 1.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (7.5f - 1.0f));
        float gateLength = 1.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2.0f - 1.5f));
        float gateHeight = 1.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2.0f - 1.5f));
        
        float thickness = 0.1;
        glm::vec3 position = FindNewCheckPointCoordinate(gateLength + 0.1 * 2, thickness);


        this->checkPoints.push_back(
            CheckPoint(position, pillarHeight, gateLength, gateHeight, leftPillar, rightPillar, lowerLine, upperLine)
        );
    }


    // Resets just the time, not the drone's position :)
    demoPlayer.Reset();
    player1.Reset();
    player2.Reset();
}



void Game::ReinitCheckPoints()
{
    int numCheckPoints = (int) checkPoints.size();
    checkPoints.clear();

    for (int i = 0; i < numCheckPoints; i++) {
        string name = "";

        name = "Pillar" + to_string(i) + "-LeftPillar";
        Mesh* leftPillar = meshes[name];

        name = "Pillar" + to_string(i) + "-RightPillar";
        Mesh* rightPillar = meshes[name];

        name = "Pillar" + to_string(i) + "-LowerLine";
        Mesh* lowerLine = meshes[name];

        name = "Pillar" + to_string(i) + "-UpperLine";
        Mesh* upperLine = meshes[name];

        float pillarHeight = 1.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2.0f - 1.0f));
        float gateLength = 1.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2.0f - 1.5f));
        float gateHeight = 1.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2.0f - 1.5f));

        float thickness = 0.1;
        glm::vec3 position = FindNewCheckPointCoordinate(gateLength + 0.1 * 2, thickness);


        this->checkPoints.push_back(
            CheckPoint(position, pillarHeight, gateLength, gateHeight, leftPillar, rightPillar, lowerLine, upperLine)
        );
    }


    demoPlayer.idxCurrrentCheckPoint = 0;
    player1.idxCurrrentCheckPoint = 0;
    player2.idxCurrrentCheckPoint = 0;
}



void Game::InitTrees(int numTrees, int& idxTree, int numCones)
{
    for (int i = 0; i < numTrees; i++) {
        idxTree++;
        
        string trunkName = "Tree" + to_string(idxTree) + "-Trunk";
        Mesh* trunkMesh = Object3D::CreateCylinder(trunkName);
        meshes[trunkMesh->GetMeshID()] = trunkMesh;
        
        float trunkHeight = 0.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (3.0f - 0.5f));
        float trunkRadius = 0.2f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (1.0f - 0.2f));
        Tree::Trunk trunk(trunkHeight, trunkRadius, trunkMesh);

        vector<Tree::Cone> cones;

        float coneHeight = 2.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (3.5f - 2.0f));
        float coneRadius = 1.75f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2.25f - 1.75f));


        for (int j = 0; j < numCones; j++) {
            string coneName = trunkName + "-Cone" + to_string(j);
            Mesh* coneMesh = Object3D::CreateCone(coneName);
            meshes[coneMesh->GetMeshID()] = coneMesh;

            cones.push_back(Tree::Cone(coneHeight, coneRadius, coneMesh));
        }

        glm::vec3 position = FindNewTreeCoordinate(coneRadius);
        this->trees.push_back(Tree(position, trunk, cones));
    }
}


glm::vec3 Game::FindNewCheckPointCoordinate(float rectengularLength, float rectengularWidth)
{
    float thickness = 0.1;

    glm::vec3 position;

    do {
        position = GenerateRandomCoordinate();
    } while (!IsValidCheckPointCoordinate(position, rectengularLength, rectengularWidth));

    return position;
}

glm::vec3 Game::FindNewTreeCoordinate(float coneRadius)
{
    glm::vec3 position;

    do {
        position = GenerateRandomCoordinate();
    } while (!IsValidTreeCoordinate(position, coneRadius));
    
    return position;
}



glm::vec3 Game::FindNewDomeCoordinate(float length)
{
    glm::vec3 position;

    do {
        position = GenerateRandomCoordinate();
    } while (!IsValidDomeCoordinate(position, length));


    return position;
}



bool Game::IsValidCheckPointCoordinate(glm::vec3& position, float rectengularLength, float rectengularWidth)
{
    if ((position.x >= -5.0f && position.x <= 5.0f) && (position.z >= -5.0f && position.z <= 5.0f)) {
        return false;
    }

    for (CheckPoint& checkPoint : checkPoints) {
        if (abs(position.z - checkPoint.position.z) <= 3.f && abs(position.x - checkPoint.position.x) <= 10.f) {
            return false;
        }
    }


    for (Tree& tree : this->trees) {
        float distance = glm::length(position - tree.position);

        if (distance < (rectengularWidth + tree.cones[0].radius)) {
            return false;
        }
    }

    for (Dome& dome : this->domes) {
        float distance = glm::length(position - dome.position);

        if (distance < (rectengularLength + dome.length * sqrt(2.f) / 2.f)) {
            return false;
        }
    }

    return true;
}


bool Game::IsValidTreeCoordinate(glm::vec3& position, float coneRadius)
{
    if ((position.x >= -3.0f && position.x <= 3.0f) && (position.z >= -3.0f && position.z <= 3.0f)) {
        return false;
    }

    return IsValidObstacleCoordinate(position, coneRadius);
}

bool Game::IsValidDomeCoordinate(glm::vec3& position, float length)
{
    if ((position.x >= -10.f && position.x <= 10.f) && (position.z >= -10.f && position.z <= 10.f)) {
        return false;
    }


    // Cercul circumscris patratului
    return (IsValidObstacleCoordinate(position, length * sqrt(2.0) / 2.0f));
}


bool Game::IsValidObstacleCoordinate(glm::vec3& position, float radius)
{
    for (Tree& tree : this->trees) {
        float distance = glm::length(position - tree.position);

        if (distance < (radius + tree.cones[0].radius)) {
            return false;
        }
    }

    for (Dome& dome : this->domes) {
        float distance = glm::length(position - dome.position);

        if (distance < (radius + dome.length * sqrt(2.f) / 2.f)) {
            return false;
        }
    }

    return true;
}


glm::vec3 Game::GenerateRandomCoordinate()
{
    float x = (float)(rand() % (int)(MAP_LENGTH - 5.f)) - (MAP_LENGTH - 5.f) / 2.0;
    float z = (float)(rand() % (int)(MAP_LENGTH - 5.f)) - (MAP_LENGTH - 5.f) / 2.0;

    // Place the position on the xOz plane
    return glm::vec3(x, 0.f, z);
}





void Game::RenderColoredMesh(Mesh* mesh, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shaders["VertexColor"]->program) return;

    glUseProgram(shaders["VertexColor"]->program);

    GLint locObjectColor = glGetUniformLocation(shaders["VertexColor"]->program, "objectColor");
    glUniform3fv(locObjectColor, 1, glm::value_ptr(color));


    glUniformMatrix4fv(glGetUniformLocation(shaders["VertexColor"]->program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaders["VertexColor"]->program, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaders["VertexColor"]->program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    mesh->Render();
}

void Game::RenderTerrainMesh(Mesh* mesh, const glm::mat4& modelMatrix, const glm::vec3& color1, const glm::vec3& color2, float frequency)
{
    if (!mesh || !shaders["TerrainShader"]->program) return;

    glUseProgram(shaders["TerrainShader"]->program);


    GLint locObjectColor1 = glGetUniformLocation(shaders["TerrainShader"]->program, "color1");
    glUniform3fv(locObjectColor1, 1, glm::value_ptr(color1));

    GLint locObjectColor2 = glGetUniformLocation(shaders["TerrainShader"]->program, "color2");
    glUniform3fv(locObjectColor2, 1, glm::value_ptr(color2));


    glUniformMatrix4fv(glGetUniformLocation(shaders["TerrainShader"]->program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaders["TerrainShader"]->program, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaders["TerrainShader"]->program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    mesh->Render();
}





void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void Game::RenderDroneBody(Player& player)
{
    Drone& drone = player.drone;

    RenderColoredMesh(drone.arm1.mesh, drone.arm1.modelMatrix, COLOR_GREY);
    RenderColoredMesh(drone.arm2.mesh, drone.arm2.modelMatrix, COLOR_GREY);

    RenderColoredMesh(drone.leg1.mesh, drone.leg1.modelMatrix, COLOR_GREY);
    RenderColoredMesh(drone.leg2.mesh, drone.leg2.modelMatrix, COLOR_GREY);
    RenderColoredMesh(drone.leg3.mesh, drone.leg3.modelMatrix, COLOR_GREY);
    RenderColoredMesh(drone.leg4.mesh, drone.leg4.modelMatrix, COLOR_GREY);

    RenderColoredMesh(drone.propeller1.mesh, drone.propeller1.modelMatrix, COLOR_BLACK);
    RenderColoredMesh(drone.propeller2.mesh, drone.propeller2.modelMatrix, COLOR_BLACK);
    RenderColoredMesh(drone.propeller3.mesh, drone.propeller3.modelMatrix, COLOR_BLACK);
    RenderColoredMesh(drone.propeller4.mesh, drone.propeller4.modelMatrix, COLOR_BLACK);
}

void Game::RenderDroneArrows(Player& player)
{
    Drone& drone = player.drone;

    RenderColoredMesh(drone.droneArrow.cylinder.mesh, drone.droneArrow.cylinder.modelMatrix, COLOR_BLACK);
    RenderColoredMesh(drone.droneArrow.cone.mesh, drone.droneArrow.cone.modelMatrix, COLOR_BLACK);

    // If all checkpoints (for the current race) have been reached, no need to render this arrow
    if (player.idxCurrrentCheckPoint < checkPoints.size()) {
        RenderColoredMesh(drone.checkPointArrow.cylinder.mesh, drone.checkPointArrow.cylinder.modelMatrix, COLOR_BLUE);
        RenderColoredMesh(drone.checkPointArrow.cone.mesh, drone.checkPointArrow.cone.modelMatrix, COLOR_BLUE);
    }

    
}


void Game::RenderScene(Player& player)
{
    for (Tree& tree : trees) {
        RenderColoredMesh(tree.trunk.mesh, tree.trunk.modelMatrix, COLOR_BROWN_TREE_TRUNK);
        for (Tree::Cone& cone : tree.cones) {
            RenderColoredMesh(cone.mesh, cone.modelMatrix, COLOR_GREEN_TREE_CONE);
        }
    }

    for (Dome& dome : domes) {
        RenderColoredMesh(dome.cubeMesh, dome.cubeModelMatrix, COLOR_WHITE_DOME_CUBE);
        RenderColoredMesh(dome.semiSphereMesh, dome.semiSphereModelMatrix, dome.semiSphereColor);
    }



    for (int i = 0; i < checkPoints.size(); i++) {
        glm::vec3 color;

        if (i == player.idxCurrrentCheckPoint) {
            // Current Checkpoint
            glm::vec3 blue(0.f / 255.f, 0.f / 255.f, 255.f / 255.f);
            color = blue;

        }
        else if (i == player.idxCurrrentCheckPoint + 1) {
            // Next Checkpoint
            glm::vec3 yellow(255.f / 255.f, 247.f / 255.f, 3.f / 255.f);
            color = yellow;
        }
        else if (i < player.idxCurrrentCheckPoint) {
            // Visited Checkpoint
            glm::vec3 green(94.f / 255.f, 255.f / 255.f, 5.f / 255.f);
            color = green;

        }
        else if (i > player.idxCurrrentCheckPoint) {
            // Not visited checkpoint
            glm::vec3 red(1.f, 0.f, 0.f);
            color = red;
        }

        CheckPoint& checkPoint = checkPoints[i];
        RenderColoredMesh(checkPoint.leftPillarMesh, checkPoint.leftPillarModelMatrix, color);
        RenderColoredMesh(checkPoint.rightPillarMesh, checkPoint.rightPillarModelMatrix, color);
        RenderColoredMesh(checkPoint.lowerLineMesh, checkPoint.lowerLineModelMatrix, color);
        RenderColoredMesh(checkPoint.upperLineMesh, checkPoint.upperLineModelMatrix, color);
    }

    if (isDemo) {
        RenderDroneBody(demoPlayer);
    }
    else {
        RenderDroneBody(player1);
        RenderDroneBody(player2);
    }
    // for display the axis: DrawCoordinateSystem();
}




void Game::UpdateScene(float deltaTimeSeconds)
{
    int numCheckPoints = (int)this->checkPoints.size();

    if (isDemo) {
        demoPlayer.drone.UpdateBody(deltaTimeSeconds);
        demoPlayer.UpdateCheckPointState();


        if (demoPlayer.idxCurrrentCheckPoint == (int)checkPoints.size()) {
            ReinitCheckPoints();
        }
    }
    else {
        player1.drone.UpdateBody(deltaTimeSeconds);
        player1.UpdateCheckPointState();

        player2.drone.UpdateBody(deltaTimeSeconds);
        player2.UpdateCheckPointState();

        if (player1.idxCurrrentCheckPoint == (int)checkPoints.size() && player2.idxCurrrentCheckPoint == (int)checkPoints.size()) {
            if (player1.elapsedTimeCompletedRace < player2.elapsedTimeCompletedRace) {
                cout << "\nPlayer 1 WON! Congrats!\n\n";
            }
            else if (player2.elapsedTimeCompletedRace < player1.elapsedTimeCompletedRace) {
                cout << "\nPlayer 2 WON! Congrats!\n\n";
            }
            else {
                cout << "\nLet's call that a TIE! Take another race :)\n\n";
            }

            ReinitCheckPoints();
        }
    }
}


void Game::Update(float deltaTimeSeconds)
{
    // Clear depth buffer and set polygon mode
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);


    UpdateScene(deltaTimeSeconds);

    glm::ivec2 resolution = window->GetResolution();

    ViewportArea viewport;

    if (isDemo)
    {
        glViewport(0, 0, resolution.x, resolution.y);
        RenderMainScreen(demoPlayer);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
        RenderMiniMap(demoPlayer);
    }
    else
    {
        glViewport(0, 0, resolution.x / 2.f - 0.5f, resolution.y);
        RenderMainScreen(player1);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
        RenderMiniMap(player1);

        glViewport(resolution.x / 2.f + 0.5f, 0, resolution.x / 2.f, resolution.y);
        RenderMainScreen(player2);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(miniViewportArea.x + resolution.x / 2.f, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
        RenderMiniMap(player2);
    }
}

void Game::RenderMainScreen(Player& player)
{
    Drone& drone = player.drone;

    float fov = 70.f;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);

    if (player.mainScreenCameraType == MainScreenCameraType::FIRST_PERSON) {
        glm::vec3 cameraOffset = glm::vec3(0.3f, 0.15f, 0.3f);

        glm::vec3 offsetPosition = glm::vec3(
            -sin(glm::radians(360.f - drone.angleOY)) * cameraOffset.x,
            cameraOffset.y,
            cos(glm::radians(360.f - drone.angleOY)) * cameraOffset.z
        );

        glm::vec3 cameraPosition = drone.position + offsetPosition;

        camera->Set(cameraPosition, drone.position, glm::vec3(0.f, 0.99f, 0.f));
    }
    else if (player.mainScreenCameraType == MainScreenCameraType::THIRD_PERSON) {
        glm::vec3 cameraOffset = glm::vec3(2.5f, 1.25f, 2.5f);

        glm::vec3 offsetPosition = glm::vec3(
            -sin(glm::radians(360.f -drone.angleOY)) * cameraOffset.x,
            cameraOffset.y,
            cos(glm::radians(360.f -drone.angleOY)) * cameraOffset.z
        );

        glm::vec3 cameraPosition =drone.position + offsetPosition;

        camera->Set(cameraPosition,drone.position, glm::vec3(0.f, 1.f, 0.f));
    }


    glm::vec3 green = glm::vec3(0.f, 1.0f, 0.0f);
    glm::vec3 brown = glm::vec3(31.f/255.f, 19.f/255.f, 6.f/255.f);
    float frequency = 5.0f; // Controls the scale of the noise


    RenderTerrainMesh(meshes["Terrain"], terrainModelMatrix, green, brown, frequency);

    RenderScene(player);
}


void Game::RenderMiniMap(Player& player)
{
    Drone& drone = player.drone;
    drone.UpdateArrows(player.idxCurrrentCheckPoint);

    float fov = 70.f;

    if (player.miniMapCameraType == MiniMapCameraType::ORTHOGONAL) {
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    }
    else if (player.miniMapCameraType == MiniMapCameraType::PERSPECTIVE) {
        float fov = 70.f;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    }


    float droneY = player.drone.position.y;
    drone.position.y = 15.0f;
    drone.UpdateBody(0.f);
    drone.UpdateArrows(player.idxCurrrentCheckPoint);

    camera->Set(
        glm::vec3(drone.position.x, 20.f, drone.position.z),
        glm::vec3(drone.position.x, 0.f, drone.position.z),
        glm::vec3(1, -1, 0)
    );


    RenderColoredMesh(meshes["Terrain"], terrainModelMatrix, glm::vec3(143.f / 255.f, 99.f / 255.f, 4.f / 255.f));
    RenderScene(player);
    RenderDroneArrows(player);


    drone.position.y = droneY;
    drone.UpdateBody(0.f);
}

void Game::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */
void Game::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_A))
    {
        if (!window->KeyHold(GLFW_KEY_D)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.MoveLeft(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        if (!window->KeyHold(GLFW_KEY_A)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.MoveRight(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_W))
    {
        if (!window->KeyHold(GLFW_KEY_S)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.MoveForward(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_S))
    {

        if (!window->KeyHold(GLFW_KEY_W)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.MoveBackward(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        if (!window->KeyHold(GLFW_KEY_DOWN)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.AscendAltitude(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        if (!window->KeyHold(GLFW_KEY_UP)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.DescendAltitude(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        if (!window->KeyHold(GLFW_KEY_RIGHT)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.RotateLeft(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        if (!window->KeyHold(GLFW_KEY_LEFT)) {
            Player& player = (isDemo) ? demoPlayer : player1;
            player.drone.RotateRight(deltaTime);
        }
    }

    if (!(window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_D)
        || window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_S))) {
        Player& player = (isDemo) ? demoPlayer : player1;
        player.drone.AnulateFlightSpeed();
    }

    if (!window->KeyHold(GLFW_KEY_UP) && !window->KeyHold(GLFW_KEY_DOWN)) {
        Player& player = (isDemo) ? demoPlayer : player1;
        player.drone.AnulateAltitudeAscendSpeed();
        player.drone.AnulateAltitudeDescendSpeed();
    }

    if (window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_Q)) {
        this->demoPlayer.miniMapCameraType = MiniMapCameraType::ORTHOGONAL;
        return;
    }


    if (window->KeyHold(GLFW_KEY_KP_4) && !isDemo)
    {
        if (!window->KeyHold(GLFW_KEY_KP_6)) {
            player2.drone.MoveLeft(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_6) && !isDemo)
    {
        if (!window->KeyHold(GLFW_KEY_KP_4)) {
            player2.drone.MoveRight(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_8) && !isDemo)
    {
        if (!window->KeyHold(GLFW_KEY_KP_2) && !isDemo) {
            player2.drone.MoveForward(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_2) && !isDemo)
    {
        if (!window->KeyHold(GLFW_KEY_KP_8)) {
            player2.drone.MoveBackward(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_9) && !isDemo) {
        if (!window->KeyHold(GLFW_KEY_KP_7)) {
            player2.drone.AscendAltitude(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_7) && !isDemo) {
        if (!window->KeyHold(GLFW_KEY_KP_9)) {
            player2.drone.DescendAltitude(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_1) && !isDemo) {
        if (!window->KeyHold(GLFW_KEY_KP_3)) {
            player2.drone.RotateLeft(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_KP_3) && !isDemo) {
        if (!window->KeyHold(GLFW_KEY_KP_1)) {
            player2.drone.RotateRight(deltaTime);
        }
    }

    if (!isDemo &&
        !(window->KeyHold(GLFW_KEY_KP_4) || window->KeyHold(GLFW_KEY_KP_6)
        || window->KeyHold(GLFW_KEY_KP_8) || window->KeyHold(GLFW_KEY_KP_2))) {
       player2.drone.AnulateFlightSpeed();

    }

    if (!window->KeyHold(GLFW_KEY_KP_7) && !window->KeyHold(GLFW_KEY_KP_9)) {
        player2.drone.AnulateAltitudeAscendSpeed();
        player2.drone.AnulateAltitudeDescendSpeed();
    }







    if (isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_O)) {
        this->demoPlayer.miniMapCameraType = MiniMapCameraType::ORTHOGONAL;
        return;
    }
    if (isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_P)) {
        this->demoPlayer.miniMapCameraType = MiniMapCameraType::PERSPECTIVE;
        return;
    }
    if (isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_1)) {
        this->demoPlayer.mainScreenCameraType = MainScreenCameraType::FIRST_PERSON;
        return;
    }
    if (isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_3)) {
        this->demoPlayer.mainScreenCameraType = MainScreenCameraType::THIRD_PERSON;
        return;
    }





    if (!isDemo && window->KeyHold(GLFW_KEY_LEFT_CONTROL) && window->KeyHold(GLFW_KEY_O)) {
        this->player1.miniMapCameraType = MiniMapCameraType::ORTHOGONAL;
        return;
    }
    if (!isDemo && window->KeyHold(GLFW_KEY_LEFT_CONTROL) && window->KeyHold(GLFW_KEY_P)) {
        this->player1.miniMapCameraType = MiniMapCameraType::PERSPECTIVE;
        return;
    }
    if (!isDemo && window->KeyHold(GLFW_KEY_LEFT_CONTROL) && window->KeyHold(GLFW_KEY_1)) {
        this->player1.mainScreenCameraType = MainScreenCameraType::FIRST_PERSON;
        return;
    }
    if (!isDemo && window->KeyHold(GLFW_KEY_LEFT_CONTROL) && window->KeyHold(GLFW_KEY_3)) {
        this->player1.mainScreenCameraType = MainScreenCameraType::THIRD_PERSON;
        return;
    }





    if (!isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_O)) {
        this->player2.miniMapCameraType = MiniMapCameraType::ORTHOGONAL;
        return;
    }
    if (!isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_P)) {
        this->player2.miniMapCameraType = MiniMapCameraType::PERSPECTIVE;
        return;
    }
    if (!isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_8)) {
        this->player2.mainScreenCameraType = MainScreenCameraType::FIRST_PERSON;
        return;
    }
    if (!isDemo && window->KeyHold(GLFW_KEY_RIGHT_CONTROL) && window->KeyHold(GLFW_KEY_9)) {
        this->player2.mainScreenCameraType = MainScreenCameraType::THIRD_PERSON;
        return;
    }
}


void Game::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_TAB)
    {
        if (this->isDemo) {
            // Making a smooth transition between the demo and the actual game
            
            this->player1.drone.position = demoPlayer.drone.position;
            this->player2.drone.position = demoPlayer.drone.position;
            
            this->player1.drone.angleOY = demoPlayer.drone.angleOY;
            this->player2.drone.angleOY = demoPlayer.drone.angleOY;

            this->player1.mainScreenCameraType = demoPlayer.mainScreenCameraType;
            this->player2.mainScreenCameraType = demoPlayer.mainScreenCameraType;

            this->player1.miniMapCameraType = demoPlayer.miniMapCameraType;
            this->player2.miniMapCameraType = demoPlayer.miniMapCameraType;

            this->ReinitCheckPoints();
        }
        this->isDemo = false;
    }

    if (key == GLFW_KEY_I)
    {
        miniViewportArea.y += 10;
    }
    if (key == GLFW_KEY_K)
    {
        miniViewportArea.y -= 10;
    }
    if (key == GLFW_KEY_J)
    {
        miniViewportArea.x -= 10;
    }
    if (key == GLFW_KEY_L)
    {
        miniViewportArea.x += 10;
    }
    if (key == GLFW_KEY_U)
    {
        miniViewportArea.width -= 10;
        const float aspectRatio = 16.0f / 9.0f;
        miniViewportArea.height = static_cast<int>(miniViewportArea.width / aspectRatio);
    }
    if (key == GLFW_KEY_M)
    {
        miniViewportArea.width += 10;
        const float aspectRatio = 16.0f / 9.0f;
        miniViewportArea.height = static_cast<int>(miniViewportArea.width / aspectRatio);
    }
}


void Game::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Game::OnWindowResize(int width, int height)
{
}
