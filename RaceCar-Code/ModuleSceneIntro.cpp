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

	state = GameState::TITLESCREEN;
	freeCam = false;

	// Set camera al player:
	app->camera->Move(vec3(app->player->position.getX(), app->player->position.getY() + 7, app->player->position.getZ() - 20));
	app->camera->LookAt(vec3(app->player->position.getX(), app->player->position.getY(), app->player->position.getZ()));

	// Audios
	winFx = app->audio->LoadFx("Assets/audio/fx/win.wav");

	// ===================================
	//				Physbodys
	// ===================================

	// Map ground
	AddGround(-200, -6, 0, { 500, 10, 1000 }, Green);

	// River

	// 0
	AddGround(-25, -1, 0, { 10, 1, 25 });
	AddGround(-25, -1, 25, { 10, 1, 25 });
	AddGround(-25, -1, 50, { 10, 1, 25 });
	AddGround(-25, -1, 75, { 10, 1, 25 });
	AddGround(-25, -1, 100, { 10, 1, 25 });
	AddGround(-25, -1, 125, { 10, 1, 25 });
	AddGround(-25, -1, 150, { 10, 1, 25 });
	AddGround(-25, -1, 175, { 10, 1, 25 });
	AddGround(-25, -1, 200, { 10, 1, 25 });

	AddGround(-42, -1, 200, { 25, 1, 25 });

	// 1
	AddGround(0, -1, 0, { 25, 1, 25 });
	AddGround(0, -1, 25, { 25, 1, 25 });
	AddGround(0, -1, 50, { 25, 1, 25 });
	AddGround(0, -1, 75, { 25, 1, 25 });
	AddGround(0, -1, 100, { 25, 1, 25 });
	AddGround(0, -1, 125, { 25, 1, 25 });
	AddGround(0, -1, 150, { 25, 1, 25 });
	AddGround(0, -1, 175, { 25, 1, 25 });
	AddGround(0, -1, 200, { 25, 1, 25 });

	AddWall(13, 0, 100, { 1, 4, 200 });
	AddWall(-13, 0, 100, { 1, 4, 200 });

	// 2
	AddGround(0, -1, 225, { 25, 1, 25 });
	AddGround(0, -1, 250, { 25, 1, 25 });
	AddGround(-25, -1, 225, { 25, 1, 25 });
	AddGround(-25, -1, 250, { 25, 1, 25 });
	AddGround(-50, -1, 225, { 25, 1, 25 });
	AddGround(-50, -1, 250, { 25, 1, 25 });
	AddGround(-75, -1, 225, { 25, 1, 25 });
	AddGround(-75, -1, 250, { 25, 1, 25 });
	AddGround(-100, -1, 225, { 25, 1, 25 });
	AddGround(-100, -1, 250, { 25, 1, 25 });

	// 3
	AddGround(-100, -1, 200, { 25, 1, 25 });
	AddGround(-100, -1, 175, { 25, 1, 25 });
	AddGround(-100, -1, 150, { 25, 1, 25 });

	// 4
	AddGround(-100, -1, 125, { 25, 1, 25 });
	AddGround(-100, -1, 100, { 25, 1, 25 });
	AddGround(-75, -1, 125, { 25, 1, 25 });
	AddGround(-75, -1, 100, { 25, 1, 25 });
	AddGround(-75, -1, 75, { 25, 1, 25 });

	// 5
	AddGround(-75, -1, 50, { 25, 1, 25 });
	AddGround(-100, -1, 50, { 25, 1, 25 });
	AddGround(-125, -1, 50, { 25, 1, 25 });

	// 6
	AddGround(-150, -1, 75, { 25, 1, 50 }, Blue, -45, false, true);
	AddGround(-175, -1, 100, { 25, 1, 25 }, Blue, -45, false, true);

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
	switch (state)
	{
	case TITLESCREEN:

		// ==========================
		//			INPUT
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			state = GameState::SELECTIONSCREEN;
			LOG("Loading Selection screen");
		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			return UPDATE_STOP;
		}

		// ==========================
		//			Update
		// ==========================

		if (titleMusic == false) {
			app->audio->PlayMusic("Assets/audio/music/titleScreen.ogg");
			titleMusic = true;
			endMusic = menuMusic = gameplayMusic = false;
		}

		app->camera->Position = { -300, 300, 100 };
		app->camera->LookAt(vec3(-300, 0, 101));

		LOG("%.2f %.2f %.2f", app->camera->Position.x, app->camera->Position.y, app->camera->Position.z)

		break;
	case SELECTIONSCREEN:

		// ==========================
		//			INPUT
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			state = GameState::GAMEPLAY;
			LOG("Loading Gameplay");
		}
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
		}

		// ==========================
		//			Update
		// ==========================

		if (menuMusic == false) {
			app->audio->PlayMusic("Assets/audio/music/menu.ogg");
			menuMusic = true;
			endMusic = titleMusic = gameplayMusic = false;
		}

		break;
	case GAMEPLAY:

		// ==========================
		//			INPUT
		// ==========================

		if (gameplayMusic == false) {
			app->audio->PlayMusic("Assets/audio/music/tutorial.ogg");
			gameplayMusic = true;
			endMusic = menuMusic = titleMusic = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
		}

		// ==========================
		//			Update
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
			debug = !debug;
		}

		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			freeCam = !freeCam;
		}

		// CAMERA
		if (freeCam == false) {
			CameraPlayer();
		}

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			app->audio->PlayFx(winFx);
		}

		break;
	case ENDSCREEN:

		// ==========================
		//			INPUT
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
		}

		break;
	}
	
	// ====================================================
	//						Render Ground
	// ====================================================

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