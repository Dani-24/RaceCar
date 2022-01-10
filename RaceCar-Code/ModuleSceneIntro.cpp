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

	// Audios
	winFx = app->audio->LoadFx("Assets/audio/fx/gameplay_win.wav");
	turboFx = app->audio->LoadFx("Assets/audio/fx/gameplay_turbo.wav");
	kickFx = app->audio->LoadFx("Assets/audio/fx/zas.wav");

	// Textures
	susTex = app->renderer3D->LoadTexture("Assets/textures/amogus.png");

	// Sun
	sun.SunBall.radius = 25;
	sun.SunBall.color = Yellow;
	sun.speed = 0.00001f;
	sun.movement_height = 250.0f; 
	sun.movement_width = 300.0f;

	sunTimer.Start();

	// ===================================
	//				Physbodys
	// ===================================

	// Map ground
	AddGround();

	AddCube({ 0, 0, 50 }, { 10, 10, 10 }, Yellow);

	AddLinearCircuit({ 0, 0, 0 }, { 0, 0, 50 }, 5);
	
	AddCircularCircuit({ 0, 0, 50 }, {-25, 0, 75 }, 10);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	scenery.clear();

	return true;
}

void ModuleSceneIntro::AddGround() {
	Cube groundToAdd;

	groundToAdd.size = { 50, 1, 50 };

	int type = 0;
	// X
	for (int i = 0; i < 10; i++)
	{
		// Z
		for (int j = 0; j < 10; j++) {

			groundToAdd.SetPos(i * -50, 0, j * 50);

			switch (groundCoord[i][j])
			{
			case 0:
				groundToAdd.color = Green;
				break;
			case 1:
				groundToAdd.color = Blue;
				break;
			}

			app->physics->AddBody(groundToAdd, 0);
			scenery.add(groundToAdd);
		}
	}

}

void ModuleSceneIntro::AddCube(vec3 position, vec3 size, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Cube cubeToAdd;

	cubeToAdd.color = RGB;

	cubeToAdd.SetPos(position.x, position.y, position.z);

	cubeToAdd.size = size;

	// angle, XYZ
	if (rotateX == true) {
		cubeToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		cubeToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		cubeToAdd.SetRotation(angle, { 0, 0, 1 });
	}

	app->physics->AddBody(cubeToAdd, 0);

	scenery.add(cubeToAdd);
}

void ModuleSceneIntro::AddLinearCircuit(vec3 initPos, vec3 finalPos, int sideWalls) {

	// Distance between the 2 points
	float distance = sqrt(pow(finalPos.x - initPos.x, 2) + pow(finalPos.y - initPos.y, 2) + pow(finalPos.z - initPos.z, 2));
	float cubeDistance = distance / sideWalls;

	// Direction
	vec3 direction = finalPos - initPos;
	float directionModule = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
	direction /= directionModule;

	// Something
	vec3 noSeQueEsEstoHulio = { -direction.z, 0, direction.x };
	float noSeQueEsEstoHulioModule = sqrt((noSeQueEsEstoHulio.x * noSeQueEsEstoHulio.x) + (noSeQueEsEstoHulio.y * noSeQueEsEstoHulio.y) + (noSeQueEsEstoHulio.z * noSeQueEsEstoHulio.z));
	noSeQueEsEstoHulio /= noSeQueEsEstoHulioModule;

	vec3 pos;

	// Create Cube & assign size
	Cube c;
	c.size = { 1, 1, 2 };

	for (uint j = 0; j < sideWalls; j++)
	{
		// Cube colors
		c.color = (j % 2 == 0) ? Cyan : Magenta;

		// Cube left
		pos = (initPos + (direction * j * cubeDistance)) + ((Circuit_Width / 2) * noSeQueEsEstoHulio);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		app->physics->AddBody(c, 0);
		scenery.add(c);

		// Cube right
		pos = (initPos + (direction * j * cubeDistance)) + ((Circuit_Width / 2) * -noSeQueEsEstoHulio);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		app->physics->AddBody(c, 0);
		scenery.add(c);
	}
}

void ModuleSceneIntro::AddCircularCircuit(vec3 initPos, vec3 finalPos, int sideWalls) {
	int factor = 0.15f;
	assert(factor < 1.0f && factor > -1.0f);
	float distance = length(finalPos - initPos);
	vec3 mid_point = (finalPos - initPos) / 2.0f + initPos;

	vec3 direction = finalPos - initPos;
	direction = normalize(direction);

	vec3 hulio = { -direction.z, 0, direction.x };
	hulio = normalize(hulio);

	float max_radi = distance / 2.0f;
	float seg_to_high_point = factor * max_radi;

	vec3 h = mid_point + (seg_to_high_point * hulio);

	float mFH = (h.z - finalPos.z) / (h.x - finalPos.x);
	float mIH = (initPos.z - h.z) / (initPos.x - h.x);

	vec3 center_circle = { 0, 0, 0 };
	center_circle.x = (mFH * mIH * (initPos.z - finalPos.z) + mFH * (h.x + initPos.x) - mIH * (finalPos.x + h.x)) / (2.0f * (mFH - mIH));
	center_circle.z = (-1 / mFH) * (center_circle.x - ((finalPos.x + h.x) / 2.0f)) + ((finalPos.z + h.z) / 2.0f);

	vec3 c_to_i = normalize(initPos - center_circle);
	vec3 c_to_f = normalize(finalPos - center_circle);
	float theta = acos(dot(c_to_f, c_to_i));
	float radius = length(finalPos - center_circle);

	float angle_ref = 0.0f;
	if (initPos.z >= center_circle.z && initPos.x < center_circle.x) {
		angle_ref = acos(dot(c_to_i, { 1, 0, 0 }));
	}
	else if (initPos.z >= center_circle.z && initPos.x >= center_circle.x) {
		angle_ref = acos(dot(c_to_i, { 1, 0, 0 }));
	}
	else if (initPos.z < center_circle.z && initPos.x >= center_circle.x) {
		angle_ref = 2 * M_PI - acos(dot(c_to_i, { 1, 0, 0 }));
	}
	else if (initPos.z < center_circle.z && initPos.x < center_circle.x) {
		angle_ref = 2 * M_PI - acos(dot(c_to_i, { 1, 0, 0 }));
	}

	vec3 pos;

	// Create the cube & size
	Cube c;
	c.size = { 1, 1, 2 };

	vec3 central_pos;
	// Curve Exterior
	for (uint j = 0; j < sideWalls; j++)
	{
		c.color = (j % 2 == 0) ? Yellow : Red;
		float sub_angle = (factor > 0.0f) ? -(float)j / sideWalls * theta : (float)j / sideWalls * theta;

		central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
		central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

		vec3 to_center = normalize(central_pos - center_circle);
		pos = central_pos + ((Circuit_Width / 2.0f) * to_center);
		c.SetPos(pos.x, pos.y + 1, pos.z);

		app->physics->AddBody(c, 0);
		scenery.add(c);
	}

	// Curve Interior
	for (uint j = 0; j < sideWalls; j++)
	{
		c.color = (j % 2 == 0) ? Yellow : Red;
		float sub_angle = (factor > 0.0f) ? -(float)j / sideWalls * theta : (float)j / sideWalls * theta;

		central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
		central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

		vec3 to_center = normalize(central_pos - center_circle);
		pos = central_pos + ((Circuit_Width / 2.0f) * -to_center);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		
		app->physics->AddBody(c, 0);
		scenery.add(c);
	}
}

void ModuleSceneIntro::AddCheckPoint(vec3 position, float angle) {

}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// ====================================================
	//			 Sun (independent from state)
	// ====================================================

	sun.SunBall.SetPos(-220 + cosf(sunTimer.Read() * sun.speed) * sun.movement_width, sinf(sunTimer.Read() * sun.speed) * sun.movement_height, 250);
	sun.SunBall.Render();

	// Sun Pos
	//LOG("%.2f, %.2f, %.2f ", sun.SunBall.GetPos().x, sun.SunBall.GetPos().y, sun.SunBall.GetPos().z);

	// ==========================
	//			States
	// ==========================
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

		app->camera->Position = { -250, 425, 225 };
		app->camera->LookAt(vec3( -250, 0, 226));

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

		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			app->audio->PlayFx(kickFx);
		}

		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			app->audio->PlayFx(turboFx);
		}

		// Draw

		susPos.x = app->player->position.getX() - 1.5f;
		susPos.y = app->player->position.getY() + 2;
		susPos.z = app->player->position.getZ();

		app->renderer3D->DrawTexture(susTex, susPos, 3.0f);

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

	p2List_item<Cube>* c = scenery.getFirst();
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