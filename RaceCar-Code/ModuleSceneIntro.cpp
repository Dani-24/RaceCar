#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

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

	app->audio->PlayMusic("Assets/audio/music/menuBGmusic.ogg");

	winFx = app->audio->LoadFx("Assets/audio/fx/win.wav");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// Camera Movement

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		debug = !debug;
	}

	if (debug == true) {
		CameraPlayer();
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		app->audio->PlayFx(winFx);
	}

	// Ground?
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CameraPlayer() {

	// Camera following player

	float cameraDistance = 15;

	float camX = app->player->position.getX() - cameraDistance * app->player->GetVehicleForwardVec().x;
	float camY = app->player->position.getY() + 6;
	float camZ = app->player->position.getZ() - cameraDistance * app->player->GetVehicleForwardVec().z;
 
	// Set camera
	app->camera->Position = { camX, camY, camZ };

	float posX = app->player->position.getX();
	float posZ = app->player->position.getZ();

	app->camera->LookAt(vec3(posX, 1, posZ));
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

