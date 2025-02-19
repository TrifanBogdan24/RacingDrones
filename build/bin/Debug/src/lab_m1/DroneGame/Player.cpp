#include "Drone.h"
#include "Player.h"
#include "Obstacles/CheckPoint.h"
#include "CameraType.h"
#include "ViewportArea.h"


#include <iomanip>

using namespace std;


Player::Player(
    PlayerType playerTypeValue, Drone droneValue, vector<CheckPoint>* checkPointsValue,
    ViewportArea screenValue, ViewportArea miniMapValue)
{
    this->playerType = playerTypeValue;
    this->drone = droneValue;
    this->checkPoints = checkPointsValue;

    this->miniMapCameraType = MiniMapCameraType::ORTHOGONAL;
    this->mainScreenCameraType = MainScreenCameraType::FIRST_PERSON;

    this->screenArea = screenValue;
    this->miniMapArea = miniMapValue;

    Reset();
}


void Player::Reset()
{
    this->idxCurrrentCheckPoint = 0;
    this->elapsedTimeReachedCheckedPoint = Engine::GetElapsedTime();
    this->elapsedTimeCompletedRace = Engine::GetElapsedTime();
}

void Player::MoveLeft(float deltaTime)
{
	this->drone.MoveLeft(deltaTime);
}

void Player::MoveRight(float deltaTime)
{
	this->drone.MoveRight(deltaTime);
}

void Player::MoveForward(float deltaTime)
{
	this->drone.MoveForward(deltaTime);
}

void Player::MoveBackward(float deltaTime)
{
	this->drone.MoveBackward(deltaTime);
}

void Player::RotateLeft(float deltaTime)
{
	this->drone.RotateLeft(deltaTime);
}

void Player::RotateRight(float deltaTime)
{
	this->drone.RotateRight(deltaTime);
}


void Player::UpdateCheckPointState()
{
    // If all checkpoints have been reached, there is no need to compute wheter or not the drone collides with the checkpoints
    if (idxCurrrentCheckPoint >= checkPoints->size()) {
        return;
    }

    CheckPoint& checkPoint = this->checkPoints->at(idxCurrrentCheckPoint);

    if (checkPoint.pillarHeight <= drone.position.y && drone.position.y <= checkPoint.totalHeight
        && checkPoint.position.x - checkPoint.gateLength / 2.f + checkPoint.thickness <= drone.position.x
        && drone.position.x <= checkPoint.position.x + checkPoint.gateLength / 2.f - checkPoint.thickness
        && checkPoint.position.z - checkPoint.thickness / 2.0 - 1.f <= drone.position.z
        && drone.position.z <= checkPoint.position.z + checkPoint.thickness / 2.0 + 1.f) {

        ReachedCheckPoint();
    }
}


void Player::ReachedCheckPoint()
{
    float totalElapsedTime = Engine::GetElapsedTime();
    float deltaTime = totalElapsedTime - this->elapsedTimeReachedCheckedPoint;
    this->elapsedTimeReachedCheckedPoint = totalElapsedTime;
    // I don't want to see numbers like 2e+02 

    cout << fixed << setprecision(2);

    if (this->playerType == PlayerType::DEMO_PLAYER) {
        cout << "[DEMO] Check point reached after " << deltaTime << " seconds!\n";
    }
    else if (this->playerType == PlayerType::FIRST_PLAYER) {
        cout << "[FIRST PLAYER] Check point reached after " << deltaTime << " seconds!\n";
    }
    else if (this->playerType == PlayerType::SECOND_PLAYER) {
        cout << "[SECOND PLAYER] Check point reached after " << deltaTime << " seconds!\n";
    }
    
    this->idxCurrrentCheckPoint++;

    if (idxCurrrentCheckPoint == (int) (*this->checkPoints).size()) {
        CompletedRace();
    }
}

void Player::CompletedRace()
{
    float totalElapsedTime = Engine::GetElapsedTime();
    float deltaTime = totalElapsedTime - this->elapsedTimeCompletedRace;
    this->elapsedTimeCompletedRace = totalElapsedTime;

    cout << fixed << setprecision(2);

    if (this->playerType == PlayerType::DEMO_PLAYER) {
        cout << "Completed race in " << deltaTime << " seconds!\n";
    }
    else if (this->playerType == PlayerType::FIRST_PLAYER) {
        cout << "First player completed race in " << deltaTime << " seconds!\n";
    }
    else if (this->playerType == PlayerType::SECOND_PLAYER) {
        cout << "Second player completed race in " << deltaTime << " seconds!\n";
    }

}

