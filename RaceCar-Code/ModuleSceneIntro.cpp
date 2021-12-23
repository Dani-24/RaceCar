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

	//app->camera->Move(vec3(0.0f, 7.5f, -20.0f));
	//app->camera->LookAt(vec3(0, 0, 0));

	app->camera->Move(vec3(app->player->position.getX(), app->player->position.getY() + 7, app->player->position.getZ() - 20));
	app->camera->LookAt(vec3(app->player->position.getX(), app->player->position.getY(), app->player->position.getZ()));

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

	/*app->camera->Move(vec3(app->player->position.getX(), app->player->position.getY() + 7, app->player->position.getZ() + 10));
	app->camera->LookAt(vec3(app->player->position.getX(), app->player->position.getY(), app->player->position.getZ()));*/

	// Ground?
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

