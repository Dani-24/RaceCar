#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define Vehicle_Fall_Dist -100
#define Camera_Fall_Dist -25

#define Circuit_Width 30

struct PhysBody3D;
struct PhysMotor3D;

enum GameState {
	TITLESCREEN,
	GAMEPLAY,
	ENDSCREEN
};

enum LapState {
	START,
	FIRSTLAP,
	SECONDLAP,
	LASTLAP
};

enum RaceState {
	WIN,
	LOSE,
	DEFAULT
};

struct Sun {
	Sphere SunBall;
	float speed;
	float movement_width;
	float movement_height;
};

struct CheckPoint {
	PhysBody3D* body;
	Cylinder leftC, rightC;
	Cube colorBody;
	float angle;
	bool checked;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void SceneReset();

	void CameraPlayer();

	void CreateCircuit();

public:
	p2List<Cube> sceneryCubes;
	p2List<Cylinder> sceneryCylinders;
	p2List<CheckPoint> checkPoints;

	Sun sun;
	Timer sunTimer;

	void AddGround();

	// X, Y, Z and {sizeX, sizeY, sizeZ} and angle rotation
	void AddCube(vec3 position, vec3 size, Color RGB = Blue, int angle = 0, bool rotateX = false, bool rotateY = false, bool rotateZ = false);

	void AddLinearCircuit(vec3 initPos, vec3 finalPos, int sideWalls, int circuitW = Circuit_Width);

	void AddCircularCircuit(vec3 initPos, vec3 finalPos, float angle, int sideWalls, int sideWallsInt, int circuitW = Circuit_Width);

	void AddWallCircuit(vec3 initPos, vec3 finalPos, int walls, bool right);

	void AddCurveWallCircuit(vec3 initPos, vec3 finalPos, float angle, int walls, bool exterior);

	void AddCheckPoint(vec3 position, float angle, float circuitW = Circuit_Width, Color color = White, int id = 2, bool startChecked = true);

	bool debug = true, freeCam;

	GameState state;

	LapState currentLap;

	RaceState areYouWinningSon;

	bool playerUnderWater = false, salpicadura = false;

	Timer cronometro;

	bool playingMusic;

	uint winFx, loseFx;

	bool titleCam = false;

private:

	float lastPlayerPosY;

	// Textures
	uint susTex;
	vec3 susPos = { 0, 0, 0 };

	// Ground Map 0 = Terrain, 1 = Water with sand, 2 = Pure Water (The map is printed rotated 90 degrees)
	int groundCoord[10][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 2, 1, 1, 1, 1, 1, 1, 0, 0,
		2, 2, 1, 1, 1, 2, 2, 1, 0, 0,
		0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 1, 0, 0,
		0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
		0, 0, 2, 2, 2, 0, 0, 0, 0, 0,
		0, 0, 2, 2, 2, 0, 0, 0, 0, 0,
	};
};
