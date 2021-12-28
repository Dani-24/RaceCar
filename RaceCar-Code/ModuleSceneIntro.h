#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/LinearMath/btVector3.h"

#define Vehicle_Fall_Dist -100
#define Camera_Fall_Dist -10

struct PhysBody3D;
struct PhysMotor3D;

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
	
	p2List<Cube> ground;

	// X, Y, Z and {sizeX, sizeY, sizeZ} and angle rotation
	void AddGround(int X, int Y, int Z, vec3 size, int angle = 0, bool rotateX = false, bool rotateY = false, bool rotateZ = false);
	// X, Y, Z and {sizeX, sizeY, sizeZ}
	void AddWall(int X, int Y, int Z, vec3 size);

	bool debug = false;

private:
	uint winFx;

	SDL_Texture* HighSpeedSprite;
};
