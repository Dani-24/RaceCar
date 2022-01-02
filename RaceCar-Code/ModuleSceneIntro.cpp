#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "Timer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// Set camera al player:
	app->camera->Move(vec3(app->player->position.getX(), app->player->position.getY() + 7, app->player->position.getZ() - 20));
	app->camera->LookAt(vec3(app->player->position.getX(), app->player->position.getY(), app->player->position.getZ()));

	// Audios
	app->audio->PlayMusic("Assets/audio/music/menuBGmusic.ogg");
	winFx = app->audio->LoadFx("Assets/audio/fx/win.wav");

	// ===================================
	//				Physbodys
	// ===================================

	// Map ground
	//AddGround(-200, -2, 0, { 500, 1, 1000 }, Green);

	// River

	// 1
	AddGround(0, -1, 0, { 25, 1, 300 });
	AddGround(0, -1, 300, { 25, 1, 300 });
	AddWall(13, 0, 0, { 1, 4, 700 });
	AddWall(-13, 0, 0, { 1, 4, 700 });

	// 2
	AddGround(-62, -1, 400, { 100, 1, 100 });

	// 3
	AddGround(-100, -1, 250, {25, 1, 200});


	// BG Color
	app->renderer3D->SetBGColor(255, 0, 153);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	ground.clear();

	return true;
}

void ModuleSceneIntro::AddGround(int X, int Y, int Z, vec3 size, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Cube groundToAdd;

	groundToAdd.color = RGB;

	groundToAdd.SetPos(X, Y, Z);

	groundToAdd.size = size;

	// angle, XYZ
	if (rotateX == true) {
		groundToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		groundToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		groundToAdd.SetRotation(angle, { 0, 0, 1 });
	}

	app->physics->AddBody(groundToAdd, 0);

	ground.add(groundToAdd);
}

void ModuleSceneIntro::AddWall(int X, int Y, int Z, vec3 size)
{
	Cube wallToAdd;

	wallToAdd.color = Turquoise;

	wallToAdd.SetPos(X, Y, Z);

	wallToAdd.size = size;

	app->physics->AddBody(wallToAdd, 0);

	ground.add(wallToAdd);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// Camera Movement

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debug = !debug;
	}

	if (debug != true) {
		CameraPlayer();
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		app->audio->PlayFx(winFx);
	}

	p2List_item<Cube>* c = ground.getFirst();
	while (c != NULL) {
		c->data.Render();
		c = c->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CameraPlayer() {

	if (app->player->position.getY() > Camera_Fall_Dist) {
		// Camera following player
		float cameraDistance = 15;

		// Get player position + forward vec3 from X and Z axis
		float camX = app->player->position.getX() - cameraDistance * app->player->GetVehicleForwardVec().x;
		float camY = app->player->position.getY() + 6;
		float camZ = app->player->position.getZ() - cameraDistance * app->player->GetVehicleForwardVec().z;

		// Set camera
		app->camera->Position = { camX, camY, camZ };

		// Get player position
		float posX = app->player->position.getX();
		float posY = app->player->position.getY();
		float posZ = app->player->position.getZ();

		app->camera->LookAt(vec3(posX, posY, posZ));
	}
	else {
		// Get player position (Camera stop moving)
		float posX = app->player->position.getX();
		float posY = app->player->position.getY();
		float posZ = app->player->position.getZ();

		app->camera->LookAt(vec3(posX, posY, posZ));
	}
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}