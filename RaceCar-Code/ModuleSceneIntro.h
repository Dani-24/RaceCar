#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/LinearMath/btVector3.h"

#define Vehicle_Fall_Dist -100
#define Camera_Fall_Dist -25

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

	void CreateCircuit();

public:
	
	p2List<Cube> sceneryCubes;
	p2List<Cylinder> sceneryCylinders;
	p2List<PhysBody3D*> checkPoints;

	Sun sun;
	Timer sunTimer;

	void AddGround();

	// X, Y, Z and {sizeX, sizeY, sizeZ} and angle rotation
	void AddCube(vec3 position, vec3 size, Color RGB = Blue, int angle = 0, bool rotateX = false, bool rotateY = false, bool rotateZ = false );

	void AddLinearCircuit(vec3 initPos, vec3 finalPos, int sideWalls, int circuitW = Circuit_Width);

	void AddCircularCircuit(vec3 initPos, vec3 finalPos, float angle, int sideWalls, int sideWallsInt, int circuitW = Circuit_Width);

	void AddWallCircuit(vec3 initPos, vec3 finalPos, int walls, bool right);

	void AddCurveWallCircuit(vec3 initPos, vec3 finalPos, float angle, int walls, bool exterior);

	void AddCheckPoint(vec3 position, float angle, float circuitW = Circuit_Width, Color color = White);

	bool debug = true, freeCam;

	GameState state;

private:
	uint winFx, kickFx, lapFx, finalLapFx, checkpointFx;

	bool titleMusic = false, menuMusic = false, gameplayMusic = false, endMusic = false;

	// Textures

	uint susTex;
	vec3 susPos = {0, 0, 0};

	uint waterTex;
	p2List<vec3> waterCoord;

	// Ground Map 0 = Terrain, 1 = Water with sand, 2 = Water (The map is printed rotated 90 degrees)
	int groundCoord[10][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
		1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	};
};
