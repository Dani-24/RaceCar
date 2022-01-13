#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModuleEnemies : public Module
{
public:
	ModuleEnemies(Application* app, bool start_enabled = true);
	virtual ~ModuleEnemies();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddKart(vec3 position);

public:
	p2List<PhysVehicle3D*> vehicles;

	float turn;
	float acceleration;
	float brake;

private:
	uint engineFx;
	bool playingEngineFx = false;

	uint fallFx;
	bool fallFxPlayed = false;

	uint turboFx;
	bool turboFxPlayed = false;
};