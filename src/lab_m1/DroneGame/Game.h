#pragma once

#include "components/simple_scene.h"
#include "Camera.h"
#include "CameraType.h"
#include "ViewportArea.h"

#include "Obstacles/Tree.h"
#include "Obstacles/Dome.h"
#include "Obstacles/CheckPoint.h"


#include "RectangularParallelipiped.h"
#include "Drone.h"
#include "Player.h"


#include <vector>



namespace m1
{
    class Game : public gfxc::SimpleScene
    {
    public:
        Game();
        ~Game();

        void Init() override;
        void InitDroneMeshes(Drone& drone, string name);
        void InitDrones();
        void InitForest();  // Multiple Trees
        void InitTrees(int numTrees, int& idxTree, int numCones);
        void InitDomes();
        void InitCheckPoints();
        void ReinitCheckPoints();


    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
    
        void RenderTerrainMesh(Mesh* mesh, const glm::mat4& modelMatrix, const glm::vec3& color1, const glm::vec3& color2, float frequency);
        void RenderColoredMesh(Mesh* mesh, const glm::mat4& modelMatrix, const glm::vec3& color);
        void RenderDrones();
        void RenderDroneBody(Player& player);
        void RenderDroneArrows(Player &player);      // in ViewPort
        void RenderScene(Player& player);
        void RenderMainScreen(Player& player);
        void RenderMiniMap(Player& player);            // ViewPort



        void UpdateScene(float deltaTimeSeconds);
        void UpdateCheckPointState();
        glm::vec3 FindNewTreeCoordinate(float coneRadius);
        glm::vec3 FindNewDomeCoordinate(float length);
        glm::vec3 FindNewCheckPointCoordinate(float rectengularLength, float rectengularWidth);
        glm::vec3 GenerateRandomCoordinate();
        bool IsValidTreeCoordinate(glm::vec3& position, float coneRadius);
        bool IsValidDomeCoordinate(glm::vec3& position, float length);
        bool IsValidCheckPointCoordinate(glm::vec3& position, float rectengularLength, float rectengularWidth);
        bool IsValidObstacleCoordinate(glm::vec3& position, float radius);





    protected:
        gamecamera::Camera* camera;
        glm::mat4 modelMatrix;
        glm::mat4 projectionMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;
        ViewportArea miniViewportArea;
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;

        vector<Tree> trees;
        vector<Dome> domes;
        vector<CheckPoint> checkPoints;

        Player demoPlayer;
        Player player1;
        Player player2;
        
        bool isDemo;

        glm::mat4 terrainModelMatrix;


    };
}   // namespace m1

