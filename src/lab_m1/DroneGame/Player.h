#pragma once

#include "Drone.h"
#include "CameraType.h"
#include "Obstacles/CheckPoint.h"
#include "ViewportArea.h"



class Player {
public:
	enum class PlayerType {
		DEMO_PLAYER,
		FIRST_PLAYER,
		SECOND_PLAYER
	};

public:
	PlayerType playerType;
	Drone drone;
	vector<CheckPoint>* checkPoints;
	MiniMapCameraType miniMapCameraType;
	MainScreenCameraType mainScreenCameraType;
	int idxCurrrentCheckPoint;
	float elapsedTimeReachedCheckedPoint;
	float elapsedTimeCompletedRace;
	
	ViewportArea miniMapArea;
	ViewportArea screenArea;



public:
	Player() = default;
	Player(
		PlayerType playerTypeValue, Drone droneValue, vector<CheckPoint>* checkPointsValue,
		ViewportArea screenValue, ViewportArea miniMapValue
	);
	void Reset();                  // Resets just the time, not the drone's position :)
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void RotateLeft(float deltaTime);
	void RotateRight(float deltaTime);
	void UpdateCheckPointState();

private:
	void ReachedCheckPoint();
	void CompletedRace();

};
