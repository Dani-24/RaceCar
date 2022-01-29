#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 2000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

#define MAX_SPEED 200.0f
#define MAX_SPEED_BACKWARDS 50.0f
#define MAX_TURBO_SPEED 300.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void CreateCar();

	vec3 GetVehicleForwardVec();
	void Respawn();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void SetInitPos();

public:
	PhysVehicle3D* vehicle;

	float turn;
	float acceleration;
	float brake;

	btVector3 position;

	vec3 initialPos = { 50, 1, 275 };

	bool allowPlayerControl = false;

	float countdown, killerCountDown;

	bool countDownSoundPlay = false;

	int lastCheckPointID = 100;
private:
	uint engineFx;
	bool playingEngineFx = false;

	uint fallFx;
	bool fallFxPlayed = false;

	uint turboFx;
	bool turboFxPlayed = false;

	uint respawnFx, finalLapFx, checkpointFx, uwCheckpointFx, lapFx;
	uint countFx, lastCountFx;
};