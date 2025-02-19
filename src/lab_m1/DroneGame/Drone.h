#pragma once


#include "components/simple_scene.h"
#include "RectangularParallelipiped.h"
#include "lab_m1/DroneGame/Camera.h"

#include "lab_m1/DroneGame/Obstacles/Tree.h"
#include "lab_m1/DroneGame/Obstacles/Dome.h"
#include "lab_m1/DroneGame/Obstacles/CheckPoint.h"
#include "Arrow.h"

class Drone {
    public:
        glm::vec3 position;
        RectangularParallelipiped arm1;
        RectangularParallelipiped arm2;
        RectangularParallelipiped leg1;
        RectangularParallelipiped leg2;
        RectangularParallelipiped leg3;
        RectangularParallelipiped leg4;
        RectangularParallelipiped propeller1;
        RectangularParallelipiped propeller2;
        RectangularParallelipiped propeller3;
        RectangularParallelipiped propeller4;

        Arrow droneArrow;
        Arrow checkPointArrow;


    public:
        float angleOX;  // axa stanga-dreapta
        float angleOY;  // axa sus-jos
        float angleOZ;  // axa inainte-inapoi

        float anglePropellerOZ;
        gamecamera::Camera* camera;

        int mapLength;

        vector<Tree>* trees;
        vector<Dome>* domes;
        vector<CheckPoint>* checkPoints;


    private:
        float flightSpeed;
        float rotationSpeed;
        float altitudeAscendSpeed;
        float altitudeDescendSpeed;

    public:
        Drone() = default;
        Drone(
            glm::vec3 positionValue, float mapLenghtValue,
            vector<Tree>* treesValue, vector<Dome>* domesValue, vector<CheckPoint>* checkPointsValue
        );


    public:
        void UpdateBody(float deltaTime);
        void MoveLeft(float deltaTime);
        void MoveRight(float deltaTime);
        void MoveForward(float deltaTime);
        void MoveBackward(float deltaTime);
        void AscendAltitude(float deltaTime);
        void DescendAltitude(float deltaTime);
        void RotateLeft(float deltaTime);
        void RotateRight(float deltaTime);
    private:
        void UpdatePropellers(float deltaTime);
    public:
        void UpdateArrows(int idxCurrentCheckPoint);
    public:
        void AnulateFlightSpeed();
        void AnulateAltitudeAscendSpeed();
        void AnulateAltitudeDescendSpeed();
    private:
        bool IsCollision(glm::vec3 newPosition);
        bool IsPlaneCollision(glm::vec3 newPosition);
        bool IsTreeCollision(glm::vec3 newPosition, Tree& tree);
        bool IsDomeCollision(glm::vec3 newPosition, Dome& dome);
};

