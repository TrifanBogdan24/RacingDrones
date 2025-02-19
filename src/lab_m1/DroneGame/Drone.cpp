#include "Camera.h"
#include "Drone.h"
#include "RectangularParallelipiped.h"
#include "Transform3D.h"


#include "lab_m1/DroneGame/Obstacles/Tree.h"
#include "lab_m1/DroneGame/Obstacles/Dome.h"
#include "lab_m1/DroneGame/Obstacles/CheckPoint.h"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>

#define ARM_LENGTH 1.4f
#define ARM_WIDTH 0.1f
#define ARM_HEIGHT 0.1f

// A Drone LEG has the same length and width as the arm's WIDTH
#define LEG_WIDTH 0.1f
#define LEG_LENGTH 0.1f
#define LEG_HEIGHT 0.1f


#define PROPELLER_LENGTH 0.5f
#define PROPELLER_WIDTH 0.1f
#define PROPELLER_HEIGHT 0.05f

#define DRONE_MAX_FLIGHT_SPEED 17.0f
#define DRONE_MAX_ALTITUDE_ASCEND_SPEED 5.0f
#define DRONE_MAX_ALTITUDE_DESCEND_SPEED 7.0f

#define DRONE_FLIGHT_ACCELERATION 17.0f * 0.7f
#define DRONE_ALTITUDE_ASCEND_ACCELERATION 5.0f * 0.8f
#define DRONE_ALTITUDE_DESCEND_ACCELERATION 7.0f * 0.8f

#define DRONE_ROTATION_ANGLE_SPEED 180.f
#define DRONE_PORPELLER_ANGLE_SPEED 720.f
#define DRONE_TILT_ANGLE_SPEED 40.f 


#define DRONE_ARROW_RADIUS 0.5f
#define DRONE_ARROW_CYLINDER_LENGTH 1.0f
#define DRONE_ARROW_CONE_LENGTH 0.8f

#define CHECK_POINT_ARROW_RADIUS 0.15f
#define CHECK_POINT_ARROW_CYLINDER_LENGTH 1.0f
#define CHECK_POINT_ARROW_CONE_LENGTH 0.4f

#define MAXIMUM_REACHABLE_ALTITUDE 35.f

using namespace std;



Drone::Drone(
    glm::vec3 positionValue, float mapLengthValue,
    vector<Tree>* treesValue, vector<Dome>* domesValue, vector<CheckPoint>* checkPointsValue)
{
    this->position = positionValue;
    this->mapLength = mapLengthValue;

    this->trees = treesValue;
    this->domes = domesValue;
    this->checkPoints = checkPointsValue;

    this->arm1 = RectangularParallelipiped(ARM_LENGTH, ARM_WIDTH, ARM_HEIGHT);
    this->arm2 = RectangularParallelipiped(ARM_LENGTH, ARM_WIDTH, ARM_HEIGHT);

    this->leg1 = RectangularParallelipiped(LEG_WIDTH, LEG_WIDTH, LEG_HEIGHT);
    this->leg2 = RectangularParallelipiped(LEG_WIDTH, LEG_WIDTH, LEG_HEIGHT);
    this->leg3 = RectangularParallelipiped(LEG_WIDTH, LEG_WIDTH, LEG_HEIGHT);
    this->leg4 = RectangularParallelipiped(LEG_WIDTH, LEG_WIDTH, LEG_HEIGHT);

    this->propeller1 = RectangularParallelipiped(PROPELLER_LENGTH, PROPELLER_WIDTH, PROPELLER_HEIGHT);
    this->propeller2 = RectangularParallelipiped(PROPELLER_LENGTH, PROPELLER_WIDTH, PROPELLER_HEIGHT);
    this->propeller3 = RectangularParallelipiped(PROPELLER_LENGTH, PROPELLER_WIDTH, PROPELLER_HEIGHT);
    this->propeller4 = RectangularParallelipiped(PROPELLER_LENGTH, PROPELLER_WIDTH, PROPELLER_HEIGHT);

    this->angleOX = 0.0f;
    this->angleOZ = 0.0f;
    this->angleOY = 0.0f;
    this->anglePropellerOZ = 90.0f;


    this->flightSpeed = 0.f;
    this->rotationSpeed = 0.f;
    this->altitudeAscendSpeed = 0.f;
    this->altitudeDescendSpeed = 0.f;

    this->droneArrow = Arrow(DRONE_ARROW_RADIUS, DRONE_ARROW_CYLINDER_LENGTH, DRONE_ARROW_CONE_LENGTH);
    this->checkPointArrow = Arrow(CHECK_POINT_ARROW_RADIUS, CHECK_POINT_ARROW_CYLINDER_LENGTH, CHECK_POINT_ARROW_CONE_LENGTH);

    this->UpdateBody(0.f);
}




/*
 * REMINDER:  Sistem de coordonate regulat dreapta (right regulated coordinate system)
 * https://ocw.cs.pub.ro/courses/_detail/egc/laboratoare/lab4sistemcoordonate0.png?id=egc%3Alaboratoare%3A04
*/
void Drone::UpdateBody(float deltaTime)
{
    // Arm 1
    arm1.modelMatrix = glm::mat4(1);
    arm1.modelMatrix *= Transform3D::Translate(this->position.x, this->position.y, this->position.z);
    arm1.modelMatrix *= Transform3D::RotateOX(glm::radians(angleOX));
    arm1.modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY - 45.0f));
    arm1.modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
    arm1.modelMatrix *= Transform3D::Scale(ARM_LENGTH, ARM_HEIGHT, ARM_WIDTH);

    // Arm 2
    arm2.modelMatrix = glm::mat4(1);
    arm2.modelMatrix *= Transform3D::Translate(this->position.x, this->position.y, this->position.z);
    arm2.modelMatrix *= Transform3D::RotateOX(glm::radians(angleOX));
    arm2.modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY + 45.0f));
    arm2.modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
    arm2.modelMatrix *= Transform3D::Scale(ARM_LENGTH, ARM_HEIGHT, ARM_WIDTH);

    float tiltAdjustmentX = ARM_HEIGHT / 2 * sin(glm::radians(angleOX));
    float tiltAdjustmentY = ARM_HEIGHT / 2 * sin(glm::radians(angleOZ));
    float tiltAdjustmentZ = ARM_HEIGHT / 2 * sin(glm::radians(angleOY));
    float baseHeight = LEG_HEIGHT / 2 + ARM_HEIGHT / 2;

    // Leg offsets
    glm::vec3 legOffsets[4] = {
        { ARM_LENGTH / 2 - LEG_LENGTH / 2, baseHeight + tiltAdjustmentX, 0 }, // Leg 1
        { -ARM_LENGTH / 2 + LEG_LENGTH / 2, baseHeight + tiltAdjustmentX, 0 }, // Leg 2

        // TODO: leg 3 and 4 are placed correctly when moving with W/S (BIG problem with A/D)
        { 0, baseHeight - tiltAdjustmentY, -ARM_LENGTH / 2 + LEG_LENGTH / 2 },
        { 0, baseHeight - tiltAdjustmentY, -ARM_LENGTH / 2 + LEG_LENGTH / 2 }
    };

    // Update all legs with new offsets
    RectangularParallelipiped* legs[4] = { &leg1, &leg2, &leg3, &leg4 };
    for (int i = 0; i < 4; i++) {
        legs[i]->modelMatrix =
            glm::mat4(1);
        legs[i]->modelMatrix *= Transform3D::Translate(position.x, position.y, position.z);
        legs[i]->modelMatrix *= Transform3D::RotateOX(glm::radians(angleOX));
        legs[i]->modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY + (i % 2 == 0 ? -45.0f : 45.0f)));
        legs[i]->modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
        legs[i]->modelMatrix *= Transform3D::Translate(legOffsets[i].x, legOffsets[i].y, legOffsets[i].z);
        legs[i]->modelMatrix *= Transform3D::Scale(LEG_WIDTH, LEG_HEIGHT, LEG_WIDTH);
    }

    // Update the propellers
    UpdatePropellers(deltaTime);
}



void Drone::UpdatePropellers(float deltaTime)
{
    this->anglePropellerOZ += DRONE_PORPELLER_ANGLE_SPEED * deltaTime;

    if (anglePropellerOZ >= 360.f) {
        anglePropellerOZ -= 360.f;
    }

    // Propeller 1 (on top of Lower Right leg)
    glm::vec3 propellerOffset1 = glm::vec3(ARM_LENGTH / 2, LEG_HEIGHT + ARM_HEIGHT / 2 + PROPELLER_HEIGHT / 2, 0);
    propeller1.modelMatrix = glm::mat4(1);
    propeller1.modelMatrix *= Transform3D::Translate(position.x, position.y, position.z);
    propeller1.modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
    propeller1.modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY - 45.0f));
    propeller1.modelMatrix *= Transform3D::Translate(propellerOffset1.x, propellerOffset1.y, propellerOffset1.z);
    propeller1.modelMatrix *= Transform3D::RotateOY(glm::radians(anglePropellerOZ));
    propeller1.modelMatrix *= Transform3D::Scale(PROPELLER_LENGTH, PROPELLER_HEIGHT, PROPELLER_WIDTH);

    // Propeller 2 (on top of Lower Left leg)
    glm::vec3 propellerOffset2 = glm::vec3(-ARM_LENGTH / 2, LEG_HEIGHT + ARM_HEIGHT / 2 + PROPELLER_HEIGHT / 2, 0);
    propeller2.modelMatrix = glm::mat4(1);
    propeller2.modelMatrix *= Transform3D::Translate(position.x, position.y, position.z);
    propeller2.modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
    propeller2.modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY + 45.0f));
    propeller2.modelMatrix *= Transform3D::Translate(propellerOffset2.x, propellerOffset2.y, propellerOffset2.z);
    propeller2.modelMatrix *= Transform3D::RotateOY(glm::radians(anglePropellerOZ));
    propeller2.modelMatrix *= Transform3D::Scale(PROPELLER_LENGTH, PROPELLER_HEIGHT, PROPELLER_WIDTH);

    // Propeller 3 (on top of Upper Right leg)
    glm::vec3 propellerOffset3 = glm::vec3(0, LEG_HEIGHT + ARM_HEIGHT / 2 + PROPELLER_HEIGHT / 2, -ARM_LENGTH / 2);
    propeller3.modelMatrix = glm::mat4(1);
    propeller3.modelMatrix *= Transform3D::Translate(position.x, position.y, position.z);
    propeller3.modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
    propeller3.modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY + 45.0f));
    propeller3.modelMatrix *= Transform3D::Translate(propellerOffset3.x, propellerOffset3.y, propellerOffset3.z);
    propeller3.modelMatrix *= Transform3D::RotateOY(glm::radians(anglePropellerOZ));
    propeller3.modelMatrix *= Transform3D::Scale(PROPELLER_LENGTH, PROPELLER_HEIGHT, PROPELLER_WIDTH);

    // Propeller 4 (on top of Upper Left leg)
    glm::vec3 propellerOffset4 = glm::vec3(0, LEG_HEIGHT + ARM_HEIGHT / 2 + PROPELLER_HEIGHT / 2, -ARM_LENGTH / 2);
    propeller4.modelMatrix = glm::mat4(1);
    propeller4.modelMatrix *= Transform3D::Translate(position.x, position.y, position.z);
    propeller4.modelMatrix *= Transform3D::RotateOZ(glm::radians(angleOZ));
    propeller4.modelMatrix *= Transform3D::RotateOY(glm::radians(angleOY - 45.0f));
    propeller4.modelMatrix *= Transform3D::Translate(propellerOffset4.x, propellerOffset4.y, propellerOffset4.z);
    propeller4.modelMatrix *= Transform3D::RotateOY(glm::radians(anglePropellerOZ));
    propeller4.modelMatrix *= Transform3D::Scale(PROPELLER_LENGTH, PROPELLER_HEIGHT, PROPELLER_WIDTH);
}


void Drone::UpdateArrows(int idxCurrentCheckPoint)
{

    this->droneArrow.cylinder.modelMatrix =
        glm::mat4(1)
        * Transform3D::Translate(position.x, position.y, position.z)
        * Transform3D::RotateOY(glm::radians(angleOY - 90.f))
        * Transform3D::RotateOZ(glm::radians(90.f))
        * Transform3D::Scale(droneArrow.cylinder.radius, droneArrow.cylinder.length / 2.f, droneArrow.cylinder.radius);

    this->droneArrow.cone.modelMatrix =
        glm::mat4(1)
        * Transform3D::Translate(position.x, position.y, position.z)
        * Transform3D::RotateOY(glm::radians(angleOY - 90.f))
        * Transform3D::RotateOZ(glm::radians(90.f))
        * Transform3D::Translate(0.f, droneArrow.cylinder.length, 0.f)
        * Transform3D::Scale(droneArrow.cone.radius * 2.f, droneArrow.cone.length / 2.f, droneArrow.cone.radius * 2.f);


    // If all checkpoints have been reached, there is no need for this arrow
    if (idxCurrentCheckPoint >= this->checkPoints->size()) {
        return;
    }
    
    CheckPoint& checkPoint = checkPoints->at(idxCurrentCheckPoint);
    glm::vec3 point1(checkPoint.position.x, 0.f, checkPoint.position.z);
    glm::vec3 point2(this->position.x, 0.f, this->position.z);

    glm::vec3 direction = point1 - point2;
    float angleRadians = glm::atan(direction.z, direction.x);
    glm::vec3 symmetricDirection = glm::vec3(-direction.x, direction.y, direction.z);
    float symmetricAngleRadians = glm::atan(symmetricDirection.z, symmetricDirection.x);
    float angle = glm::degrees(symmetricAngleRadians);


    this->checkPointArrow.cylinder.modelMatrix =
        glm::mat4(1)
        * Transform3D::Translate(position.x, position.y + 1.f, position.z)
        * Transform3D::RotateOY(glm::radians(angle))
        * Transform3D::RotateOZ(glm::radians(90.f))
        * Transform3D::Scale(checkPointArrow.cylinder.radius, checkPointArrow.cylinder.length / 2.f, checkPointArrow.cylinder.radius);

    this->checkPointArrow.cone.modelMatrix =
        glm::mat4(1)
        * Transform3D::Translate(position.x, position.y + 1.f, position.z)
        * Transform3D::RotateOY(glm::radians(angle))
        * Transform3D::RotateOZ(glm::radians(90.f))
        * Transform3D::Translate(0.f, checkPointArrow.cylinder.length, 0.f)
        * Transform3D::Scale(checkPointArrow.cone.radius * 2.5f, checkPointArrow.cone.length / 2.f, checkPointArrow.cone.radius * 2.5f);

}


void Drone::MoveLeft(float deltaTime)
{
    this->flightSpeed += DRONE_FLIGHT_ACCELERATION * deltaTime;
    if (this->flightSpeed > DRONE_MAX_FLIGHT_SPEED) {
        this->flightSpeed = DRONE_MAX_FLIGHT_SPEED;
    }

    glm::vec3 newPosition = this->position;

    float distance = this->flightSpeed * deltaTime;
    float angleRad = glm::radians(this->angleOY + 90.0f);
    newPosition.x -= distance * sin(angleRad);
    newPosition.z -= distance * cos(angleRad);

    if (!IsCollision(newPosition)) {
        this->position = newPosition;
        this->UpdateBody(deltaTime);
    }


    // this->angleOZ += DRONE_TILT_ANGLE_SPEED * deltaTime;
    if (angleOZ > 7.5f) {
        angleOZ = 7.5f;
    }

    this->UpdateBody(deltaTime);
}

void Drone::MoveRight(float deltaTime)
{

    this->flightSpeed += DRONE_FLIGHT_ACCELERATION * deltaTime;
    if (this->flightSpeed > DRONE_MAX_FLIGHT_SPEED) {
        this->flightSpeed = DRONE_MAX_FLIGHT_SPEED;
    }

    glm::vec3 newPosition = this->position;


    float distance = this->flightSpeed * deltaTime;
    float angleRad = glm::radians(this->angleOY - 90.0f);
    newPosition.x -= distance * sin(angleRad);
    newPosition.z -= distance * cos(angleRad);

    if (!IsCollision(newPosition)) {
        this->position = newPosition;
        this->UpdateBody(deltaTime);
    }


    // this->angleOZ -= DRONE_TILT_ANGLE_SPEED * deltaTime;
    if (angleOZ < -7.5f) {
        angleOZ = -7.5f;
    }


    this->UpdateBody(deltaTime);
}


void Drone::MoveForward(float deltaTime)
{
    this->flightSpeed += DRONE_FLIGHT_ACCELERATION * deltaTime;
    if (this->flightSpeed > DRONE_MAX_FLIGHT_SPEED) {
        this->flightSpeed = DRONE_MAX_FLIGHT_SPEED;
    }
    
    glm::vec3 newPosition = this->position;


    float distance = this->flightSpeed * deltaTime;


    float angleRad = glm::radians(this->angleOY);
    
    newPosition.x -= distance * sin(angleRad);
    newPosition.z -= distance * cos(angleRad);

    if (!IsCollision(newPosition)) {
        this->position = newPosition;
        this->UpdateBody(deltaTime);
    }


    // this->angleOX -= DRONE_TILT_ANGLE_SPEED * deltaTime;
    if (angleOX < -7.5f) {
        angleOX = -7.5f;
    }


    this->UpdateBody(deltaTime);
}

void Drone::MoveBackward(float deltaTime)
{
    this->flightSpeed += DRONE_FLIGHT_ACCELERATION * deltaTime;
    if (this->flightSpeed > DRONE_MAX_FLIGHT_SPEED) {
        this->flightSpeed = DRONE_MAX_FLIGHT_SPEED;
    }

    glm::vec3 newPosition = this->position;

    float distance = this->flightSpeed * deltaTime;
    float angleRad = glm::radians(this->angleOY);

    newPosition.x += distance * sin(angleRad);
    newPosition.z += distance * cos(angleRad);

    if (!IsCollision(newPosition)) {
        this->position = newPosition;
        this->UpdateBody(deltaTime);
    }

    // this->angleOX += DRONE_TILT_ANGLE_SPEED * deltaTime;
    if (angleOX > 7.5f) {
        angleOX = 7.5f;
    }
}


void Drone::AscendAltitude(float deltaTime)
{

    this->altitudeAscendSpeed += DRONE_ALTITUDE_ASCEND_ACCELERATION * deltaTime;
    if (this->altitudeAscendSpeed > DRONE_MAX_ALTITUDE_ASCEND_SPEED) {
        this->altitudeAscendSpeed = DRONE_MAX_ALTITUDE_ASCEND_SPEED;
    }

    glm::vec3 newPosition = this->position;
    float distance = this->altitudeAscendSpeed * deltaTime;
    newPosition.y += distance;

    if (!IsCollision(newPosition)) {
        this->position = newPosition;
        this->UpdateBody(deltaTime);
    }
}

void Drone::DescendAltitude(float deltaTime)
{
    this->altitudeDescendSpeed += DRONE_ALTITUDE_DESCEND_ACCELERATION * deltaTime;
    if (this->altitudeDescendSpeed > DRONE_MAX_ALTITUDE_DESCEND_SPEED) {
        this->altitudeDescendSpeed = DRONE_MAX_ALTITUDE_DESCEND_SPEED;
    }

    glm::vec3 newPosition = this->position;
    float distance = this->altitudeDescendSpeed * deltaTime;
    newPosition.y -= distance;
    
    if (!IsCollision(newPosition)) {
        this->position = newPosition;
        this->UpdateBody(deltaTime);
    }
}


void Drone::RotateLeft(float deltaTime)
{
    this->angleOY += DRONE_ROTATION_ANGLE_SPEED * deltaTime;


    if (this->angleOY >= 360.f) {
        this->angleOY -= 360.f;
    }

    this->UpdateBody(deltaTime);
}

void Drone::RotateRight(float deltaTime)
{
    this->angleOY -= DRONE_ROTATION_ANGLE_SPEED * deltaTime;


    if (this->angleOY < 0.f) {
        this->angleOY += 360.f;
    }

    this->UpdateBody(deltaTime);
}



void Drone::AnulateFlightSpeed()
{
    this->flightSpeed = 0.f;
}

void Drone::AnulateAltitudeAscendSpeed()
{
    this->altitudeAscendSpeed = 0.f;
}

void Drone::AnulateAltitudeDescendSpeed()
{
    this->altitudeDescendSpeed = 0.f;
}




bool Drone::IsCollision(glm::vec3 newPosition)
{
    if (IsPlaneCollision(newPosition)) {
        return true;
    }

    for (Tree& tree : *(this->trees)) {
        if (IsTreeCollision(newPosition, tree)) {
            return true;
        }
    }

    for (Dome& dome : *(this->domes)) {
        if (IsDomeCollision(newPosition, dome)) {
            return true;
        }
    }

    return false;
}


bool Drone::IsPlaneCollision(glm::vec3 newPosition)
{
    if (-mapLength / 2.f > newPosition.x || newPosition.x > mapLength / 2.f) {
        return true;
    }
    if (-mapLength / 2.f > newPosition.z || newPosition.z > mapLength / 2.f) {
        return true;
    }
    if (0.f > newPosition.y || newPosition.y > MAXIMUM_REACHABLE_ALTITUDE) {
        return true;
    }
    return false;
}


bool Drone::IsTreeCollision(glm::vec3 newPosition, Tree& tree)
{
    float maxDroneAltitude = this->position.y + ARM_HEIGHT / 2.f + LEG_LENGTH + PROPELLER_HEIGHT;
    float droneMaxRadius = (ARM_LENGTH + PROPELLER_LENGTH) / 2.f;

    float minDroneAltitude = this->position.y - ARM_HEIGHT / 2.f;
    float droneMinRadius = ARM_LENGTH / 2.f;

    float coneHeight = tree.cones[0].height;
    float trunkHeight = tree.trunk.height;
    float totalTreeHeight = trunkHeight + (tree.cones.size() - 1) * tree.coneHeightOffsetPercentage * coneHeight + coneHeight;

    if (newPosition.y - ARM_HEIGHT / 2.f >= totalTreeHeight) {
        return false;
    }

    float treeTrunkRadius = tree.trunk.radius;
    float droneMinDistance = glm::distance(glm::vec2(newPosition.x, newPosition.z), glm::vec2(tree.position.x, tree.position.z));


    if (maxDroneAltitude <= trunkHeight) {
        return (droneMinDistance < (droneMinRadius + treeTrunkRadius));
    }

    for (size_t i = 0; i < tree.cones.size(); i++) {
        float coneBaseHeight = trunkHeight + i * tree.coneHeightOffsetPercentage * coneHeight;
        float coneTopHeight = coneBaseHeight + coneHeight;
        float coneRadiusAtHeight = tree.cones[i].radius * (1.0f - (newPosition.y - coneBaseHeight) / (coneTopHeight - coneBaseHeight));

        if (newPosition.y - ARM_HEIGHT / 2.f >= coneBaseHeight && newPosition.y - ARM_HEIGHT / 2.f <= coneTopHeight) {
            if (droneMinDistance < (droneMinRadius + coneRadiusAtHeight)) {
                return true;
            }
        }
    }

    float droneMaxDistance = glm::distance(glm::vec2(newPosition.x, newPosition.z), glm::vec2(tree.position.x, tree.position.z));

    if (droneMaxDistance < (droneMaxRadius + treeTrunkRadius)) {
        return true; 
    }

    if (newPosition.y + ARM_HEIGHT / 2.f < trunkHeight) {
        return false;
    }

    for (size_t i = 0; i < tree.cones.size(); i++) {
        float coneBaseHeight = trunkHeight + i * tree.coneHeightOffsetPercentage * coneHeight;
        float coneTopHeight = coneBaseHeight + coneHeight;
        float coneRadiusAtHeight = tree.cones[i].radius * (1.0f - (newPosition.y + ARM_HEIGHT / 2.f - coneBaseHeight) / (coneTopHeight - coneBaseHeight));

        if (newPosition.y + ARM_HEIGHT / 2.f >= coneBaseHeight && newPosition.y + ARM_HEIGHT / 2.f <= coneTopHeight) {
            if (droneMaxDistance < (droneMaxRadius + coneRadiusAtHeight)) {
                return true;
            }
        }
    }

    return false;
}





bool Drone::IsDomeCollision(glm::vec3 newPosition, Dome& dome)
{
    if (newPosition.y - ARM_HEIGHT / 2.f >= dome.length + dome.length / 2.f) {
        return false;
    }



    float maxDroneAltitude = newPosition.y + ARM_HEIGHT / 2.f + LEG_LENGTH + PROPELLER_HEIGHT;
    float droneMaxRadius = (ARM_LENGTH + PROPELLER_LENGTH) / 2.f;

    float minDroneAltitude = newPosition.y - ARM_HEIGHT / 2.f;
    float droneMinRadius = ARM_LENGTH / 2.f;

    glm::vec3 domeCenter = dome.position + glm::vec3(0.f, dome.length, 0.f);
    float domeRadius = dome.length / 2.f;


    glm::vec2 rotatedDomeCenter = {
        cos(-dome.angleOY) * (newPosition.x - dome.position.x) - sin(-dome.angleOY) * (newPosition.z - dome.position.z),
        sin(-dome.angleOY) * (newPosition.x - dome.position.x) + cos(-dome.angleOY) * (newPosition.z - dome.position.z)
    };

    float halfSize = dome.length / 2.f;
    glm::vec2 squareMin = { -halfSize, -halfSize };
    glm::vec2 squareMax = { halfSize, halfSize };

    glm::vec2 closestPoint = {
        max(squareMin.x, min(rotatedDomeCenter.x, squareMax.x)),
        max(squareMin.y, min(rotatedDomeCenter.y, squareMax.y))
    };

    float distanceSquared = glm::length2(closestPoint - rotatedDomeCenter);
    float droneRadius = (ARM_LENGTH + PROPELLER_LENGTH) / 2.f;

    if (minDroneAltitude <= dome.length && distanceSquared <= droneRadius * droneRadius) {
        return true;
    }




    if (newPosition.y - ARM_HEIGHT / 2.f > domeCenter.y + domeRadius) {
        return false;
    }

    glm::vec2 droneXZ(newPosition.x, newPosition.z);
    glm::vec2 domeXZ(domeCenter.x, domeCenter.z);
    float horizontalDistanceSquared = glm::length2(droneXZ - domeXZ);

    if (minDroneAltitude <= domeCenter.y) {
        float distanceToMinCircle = sqrt(horizontalDistanceSquared + pow(domeCenter.y - minDroneAltitude, 2));
        if (distanceToMinCircle <= domeRadius + droneMinRadius) {
            return true;
        }
    }

    if (maxDroneAltitude <= domeCenter.y) {
        float distanceToMaxCircle = sqrt(horizontalDistanceSquared + pow(domeCenter.y - maxDroneAltitude, 2));
        if (distanceToMaxCircle <= domeRadius + droneMaxRadius) {
            return true;
        }
    }

    float droneAltitudeMidpoint = (minDroneAltitude + maxDroneAltitude) / 2.f;
    if (droneAltitudeMidpoint > domeCenter.y) {
        float verticalOffset = domeCenter.y - droneAltitudeMidpoint;

        float distanceToMidpoint = sqrt(horizontalDistanceSquared + pow(verticalOffset, 2));

        if (distanceToMidpoint <= domeRadius + (droneMaxRadius + droneMinRadius) / 2.f) {
            return true;
        }
    }

    return false;
}
