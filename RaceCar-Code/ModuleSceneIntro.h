#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/LinearMath/btVector3.h"

#define Vehicle_Fall_Dist -100
#define Camera_Fall_Dist -10

#define Circuit_Width 30

struct PhysBody3D;
struct PhysMotor3D;

enum GameState {
	TITLESCREEN,
	SELECTIONSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct Sun {
	Sphere SunBall;
	float speed;
	float movement_width;
	float movement_height;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CameraPlayer();

public:
	
	p2List<Cube> scenery;

	Sun sun;
	Timer sunTimer;

	void AddGround();

	// X, Y, Z and {sizeX, sizeY, sizeZ} and angle rotation
	void AddCube(vec3 position, vec3 size, Color RGB = Blue, int angle = 0, bool rotateX = false, bool rotateY = false, bool rotateZ = false );

	void AddLinearCircuit(vec3 initPos, vec3 finalPos, int sideWalls);

	void AddCircularCircuit(vec3 initPos, vec3 finalPos, int sideWalls);

	void AddCheckPoint(vec3 position, float angle);

	bool debug = true, freeCam;

	GameState state;

private:
	uint winFx, turboFx, kickFx;

	bool titleMusic = false, menuMusic = false, gameplayMusic = false, endMusic = false;

	uint susTex;
	vec3 susPos = {0, 0, 0};

	// Ground Map 0 = Terrain, 1 = Water (The map is printed rotated 90 degrees), 2 = circuit
	int groundCoord[10][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
};
