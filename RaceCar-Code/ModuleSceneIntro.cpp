#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Scene assets");
	bool ret = true;

	state = GameState::TITLESCREEN;

	currentLap = LapState::START;

	areYouWinningSon = RaceState::DEFAULT;

	playingMusic = false;
	freeCam = false;

	// audio
	winFx = app->audio->LoadFx("Assets/audio/fx/gameplay_win.wav");
	loseFx = app->audio->LoadFx("Assets/audio/fx/gameplay_lose.wav");

	// ===================================
	//				Textures
	// ===================================
	susTex = app->renderer3D->LoadTexture("Assets/textures/amogus.png");
	a1 = app->renderer3D->LoadTexture("Assets/textures/a1.png");
	// a2 era SUS (aka falso png)
	a3 = app->renderer3D->LoadTexture("Assets/textures/a3.png");
	a4 = app->renderer3D->LoadTexture("Assets/textures/a4.png");

	f1 = app->renderer3D->LoadTexture("Assets/textures/f1.png");
	f2 = app->renderer3D->LoadTexture("Assets/textures/f2.png");
	f3 = app->renderer3D->LoadTexture("Assets/textures/f3.png");
	f4 = app->renderer3D->LoadTexture("Assets/textures/f4.png");

	thrompTex = app->renderer3D->LoadTexture("Assets/textures/t.png");

	meta = app->renderer3D->LoadTexture("Assets/textures/m.png");
	shrek = app->renderer3D->LoadTexture("Assets/textures/s.png");

	pistonCup = app->renderer3D->LoadTexture("Assets/textures/cpiston.png");

	// ===================================
	//				 Sun
	// ===================================
	sun.SunBall.radius = 25;
	sun.SunBall.color = Yellow;
	sun.speed = 0.00005f;
	sun.movement_height = 250.0f;
	sun.movement_width = 450.0f;

	sunTimer.Start();

	// ===================================
	//				Physbodys
	// ===================================

	// Map ground
	AddGround();

	// Map scenery details (there is no time for it sorry :P)
	//AddCube({ 30, 10, 200 }, { 25, 20, 50 }, Yellow);

	// Circuit
	CreateCircuit();

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	sceneryCubes.clear();
	sunTimer.Stop();

	return true;
}

void ModuleSceneIntro::SceneReset() {

	state = GameState::TITLESCREEN;
	currentLap = LapState::START;
	areYouWinningSon = RaceState::DEFAULT;

	playingMusic = false;
	freeCam = false;

	checkPoints.getFirst()->data.checked = false;
	p2List_item<CheckPoint>* c = checkPoints.getFirst()->next;
	while (c != NULL) {
		c->data.checked = true;
		c = c->next;
	}

	cronometro.Stop();

	app->player->countdown = 5;
	app->player->killerCountDown = 60;

	app->player->countDownSoundPlay = false;

	app->player->lastCheckPointID = 100;
	app->player->Respawn();

	app->player->allowPlayerControl = false;
}

void ModuleSceneIntro::CreateCircuit() {
	// ================
	//  Circuit track:
	// ================

	AddConstrainThing({ 50, 2, 400 }, 90);

	AddLinearCircuit({ 50, 0, 100 }, { 50, 0, 600 }, 100);

	AddCheckPoint({ 50, 0, 300 }, 90, 30, Black, 2, false); // meta

	AddCircularCircuit({ 50, 1, 600 }, { -25, 1, 675 }, -0.4f, 60, 30);

	AddLinearCircuit({ -25, 0, 675 }, { -425, 0, 675 }, 100);

	AddCheckPoint({ -250, 0, 675 }, 0, 30, Orange, 3); // checkpoint

	AddCircularCircuit({ -425, 1, 675 }, { -425, 1, 525 }, -0.99f, 120, 60);

	AddLinearCircuit({ -425, 0, 525 }, { -335, 0, 525 }, 50);

	AddCheckPoint({ -425, 0, 525 }, 180, 30, Orange, 4); // checkpoint

	AddConstrainThing({ -450, 2, 500 }, 50);

	// rampa 1
	AddCube({ -189, -17.5f, 525 }, { 300, 10, Circuit_Width * 1.5f }, Yellow, -5, false, false, true);

	AddLinearCircuit({ -335, 0, 525 }, { -100, -21, 525 }, 50);

	AddCircularCircuit({ -100, -20, 521 }, { -60, -20, 471 }, 0.45f, 30, 15);

	AddLinearCircuit({ -55.5f, -20, 480 }, { -55.5f, -20, 300 }, 50);

	AddCircularCircuit({ -55.5f, -20, 300 }, { -130.5f, -20, 225 }, 0.45f, 30, 15);

	AddLinearCircuit({ -110, -20, 225 }, { -200, -20, 200 }, 25);

	AddCheckPoint({ -200, -20, 200 }, -45, 30, Orange, 5); // checkpoint

	AddWallCircuit({ -195, -20, 202 }, { -275, -20, 175 }, 20, false);

	AddWallCircuit({ -200, -20, 195 }, { -275, -20, 125 }, 20, true);

	AddCurveWallCircuit({ -270, -15, 130 }, { -320, -15, 180 }, 0.45f, 20, true);

	AddWallCircuit({ -325, -20, 162 }, { -310, -20, 180 }, 7, true);

	// rampa 2
	AddCube({ -300, -17.5f, 264 }, { Circuit_Width * 1.5f , 10, 300 }, Yellow, -5, true);

	AddLinearCircuit({ -300, -20, 190 }, { -300, 0, 420 }, 50, 40);

	AddCheckPoint({ -300, 0, 420 }, 90, 30, Orange, 6); // checkpoint

	AddCircularCircuit({ -300, 0, 420 }, { -360, 0, 460 }, -0.45f, 30, 15, 40);

	AddLinearCircuit({ -360, 0, 460 }, { -425, 0, 440 }, 20, 40);

	AddWallCircuit({ -425, 0, 445 }, { -475, 0, 425 }, 15, false);

	AddWallCircuit({ -455, 0, 434 }, { -480, 0, 425 }, 5, true);

	// Camino Wall
	AddLinearCircuit({ -475, 0, 425 }, { -525, 0, 425 }, 10);

	AddCube({ -525, 0, 425 }, { 100, 10, Circuit_Width }, Green, -25, false, false, true);

	AddCube({ -570, 20, 250 }, { 100, 10, 400}, Green, -50, false, false, true);

	AddCube({ -525, 0, 75 }, { 100, 10, Circuit_Width }, Green, -25, false, false, true);

	// Camino Jump
	AddLinearCircuit({ -432, 0, 422 }, { -432, 0, 300 }, 40);

	AddCube({ -432, 0, 300 }, { Circuit_Width, 10, 100 }, Yellow, 15, true, false, false);

	//

	AddWallCircuit({ -425, 0, 175 }, { -305, 0, 50 }, 30, true);

	AddWallCircuit({ -512, 0, 75 }, { -475, 0, 100 }, 10, true);

	AddWallCircuit({ -510, 0, 75 }, { -300, 0, 50 }, 50, false);

	AddLinearCircuit({ -300, 0, 50 }, { 0, 0, 50 }, 60);

	AddConstrainThing({ -300, 2, 150 }, 0);

	AddCheckPoint({ -250, 0, 50 }, 180, 30, Orange, 7); // checkpoint

	AddCube({ -225, 0, 50 }, { 10, 2, Circuit_Width }, Yellow, 5, false, false, true);

	AddCube({ -125, 0, 50 }, { 200, 2, Circuit_Width }, Yellow);

	AddCircularCircuit({ 0, 0, 50 }, { 50, 0, 100 }, -0.45f, 30, 15);

	// ======================================================
	//						   Deco
	// ======================================================

	// Faro
	AddCylinder({ -225, 25, 375 }, 10, 100, Yellow, 90, false, false, true);
	AddCylinder({ -225, 75, 375 }, 15, 20, Yellow, 90, false, false, true);
	AddCube({ -210, 10, 375 }, { 30, 20 ,50 }, Yellow);

	// Gradas
	AddCube({ 85, 5, 450 }, { 20, 10 ,100 }, White, 30, false, false, true);
	AddCube({ 85, 5, 150 }, { 20, 10 ,100 }, White, 30, false, false, true);

	AddMushroom({ -450, 500 });
	AddMushroom({ -400, 550 });
	AddMushroom({ -460, 575 });
	AddMushroom({ -500, 600 });
	AddMushroom({ -550, 570 });
	AddMushroom({ -510, 490 });
	AddMushroom({ -420, 700 });
	AddMushroom({ -500, 690 });
	AddMushroom({ -400, 650 });
	AddMushroom({ -400, 500 });
}

void ModuleSceneIntro::AddGround() {
	Cube groundToAdd;

	float size = 75;

	groundToAdd.size = { size, 1, size };

	int type = 0;
	// X
	for (int i = 0; i < 10; i++)
	{
		// Z
		for (int j = 0; j < 10; j++) {

			// Last +size is to move map ground 75 pixels to left
			groundToAdd.SetPos(i * -size + size, 0, j * size);

			if (debug == true) {
				LOG("Adding ground at x: %.2f, z: %.2f", i * -size + size, j * size);
			}

			switch (groundCoord[i][j])
			{
			case 0:
				groundToAdd.color = Green;

				app->physics->AddBody(groundToAdd, 0);
				sceneryCubes.add(groundToAdd);

				break;
			case 1:
				groundToAdd.color = { 0.0f, 0.0f, 1.0f, 0.8f };
				sceneryCubes.add(groundToAdd);

				// Sand platform under water
				groundToAdd.SetPos(i * -size + size, -20, j * size);
				groundToAdd.color = Yellow;
				app->physics->AddBody(groundToAdd, 0);
				sceneryCubes.add(groundToAdd);

				break;
			case 2:
				groundToAdd.color = { 0.0f, 0.0f, 1.0f, 0.8f };

				sceneryCubes.add(groundToAdd);
				break;
			}
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

	sceneryCubes.add(cubeToAdd);
}

void ModuleSceneIntro::AddSphere(vec3 position, float radius, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Sphere sphereToAdd;

	sphereToAdd.color = RGB;

	sphereToAdd.SetPos(position.x, position.y, position.z);

	sphereToAdd.radius = radius;

	// angle, XYZ
	if (rotateX == true) {
		sphereToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		sphereToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		sphereToAdd.SetRotation(angle, { 0, 0, 1 });
	}

	app->physics->AddBody(sphereToAdd, 0);

	scenerySpheres.add(sphereToAdd);
}

void ModuleSceneIntro::AddCylinder(vec3 position, float radius, float height, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Cylinder cylinderToAdd;

	cylinderToAdd.color = RGB;

	cylinderToAdd.SetPos(position.x, position.y, position.z);

	cylinderToAdd.radius = radius;

	cylinderToAdd.height = height;

	// angle, XYZ
	if (rotateX == true) {
		cylinderToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		cylinderToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		cylinderToAdd.SetRotation(angle, { 0, 0, 1 });
	}

	app->physics->AddBody(cylinderToAdd, 0);

	sceneryCylinders.add(cylinderToAdd);
}

void ModuleSceneIntro::AddLinearCircuit(vec3 initPos, vec3 finalPos, int sideWalls, int circuitW) {

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
	c.size = { 1, 2, 1 };

	for (uint j = 0; j < sideWalls; j++)
	{
		// Cube colors
		c.color = (j % 2 == 0) ? Cyan : Magenta;

		// Cube right
		pos = (initPos + (direction * j * cubeDistance)) + ((circuitW / 2) * noSeQueEsEstoHulio);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		app->physics->AddBody(c, 0);
		sceneryCubes.add(c);

		// Cube left
		pos = (initPos + (direction * j * cubeDistance)) + ((circuitW / 2) * -noSeQueEsEstoHulio);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		app->physics->AddBody(c, 0);
		sceneryCubes.add(c);
	}
}

void ModuleSceneIntro::AddCircularCircuit(vec3 initPos, vec3 finalPos, float angle, int sideWallsEx, int sideWallsInt, int circuitW) {
	assert(angle < 1.0f && angle > -1.0f);
	float distance = length(finalPos - initPos);
	vec3 mid_point = (finalPos - initPos) / 2.0f + initPos;

	vec3 direction = finalPos - initPos;
	direction = normalize(direction);

	vec3 hulio = { -direction.z, 0, direction.x };
	hulio = normalize(hulio);

	float max_radi = distance / 2.0f;
	float seg_to_high_point = angle * max_radi;

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

	vec3 central_pos;
	// Curve Exterior
	for (uint j = 0; j < sideWallsEx; j++)
	{
		c.size = { 1, 3, 1 };
		c.color = (j % 2 == 0) ? White : Red;
		float sub_angle = (angle > 0.0f) ? -(float)j / sideWallsEx * theta : (float)j / sideWallsEx * theta;

		central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
		central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

		vec3 to_center = normalize(central_pos - center_circle);
		pos = central_pos + ((circuitW / 2.0f) * to_center);
		c.SetPos(pos.x, initPos.y + 1, pos.z);

		app->physics->AddBody(c, 0);
		sceneryCubes.add(c);
	}

	// Curve Interior
	for (uint j = 0; j < sideWallsInt; j++)
	{
		c.size = { 1, 2, 1 };
		c.color = (j % 2 == 0) ? Orange : Red;
		float sub_angle = (angle > 0.0f) ? -(float)j / sideWallsInt * theta : (float)j / sideWallsInt * theta;

		central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
		central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

		vec3 to_center = normalize(central_pos - center_circle);
		pos = central_pos + ((circuitW / 2.0f) * -to_center);
		c.SetPos(pos.x, initPos.y + 1, pos.z);

		app->physics->AddBody(c, 0);
		sceneryCubes.add(c);
	}
}

void ModuleSceneIntro::AddWallCircuit(vec3 initPos, vec3 finalPos, int walls, bool right) {

	// Distance between the 2 points
	float distance = sqrt(pow(finalPos.x - initPos.x, 2) + pow(finalPos.y - initPos.y, 2) + pow(finalPos.z - initPos.z, 2));
	float cubeDistance = distance / walls;

	// Direction
	vec3 direction = finalPos - initPos;
	float directionModule = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
	direction /= directionModule;

	// Something
	vec3 formulasFisicasMagicosas = { -direction.z, 0, direction.x };
	float formulasFisicasMagicosasModule = sqrt((formulasFisicasMagicosas.x * formulasFisicasMagicosas.x) + (formulasFisicasMagicosas.y * formulasFisicasMagicosas.y) + (formulasFisicasMagicosas.z * formulasFisicasMagicosas.z));
	formulasFisicasMagicosas /= formulasFisicasMagicosasModule;

	vec3 pos;

	// Create Cube & assign size
	Cube c;
	c.size = { 1, 2, 1 };

	for (uint j = 0; j < walls; j++)
	{
		// Cube colors
		c.color = (j % 2 == 0) ? Cyan : Magenta;

		if (right == true) {
			// Cube right
			pos = (initPos + (direction * j * cubeDistance)) + ((Circuit_Width / 2) * formulasFisicasMagicosas);
			c.SetPos(pos.x, pos.y + 1, pos.z);
			app->physics->AddBody(c, 0);
			sceneryCubes.add(c);
		}
		else {
			// Cube left
			pos = (initPos + (direction * j * cubeDistance)) + ((Circuit_Width / 2) * -formulasFisicasMagicosas);
			c.SetPos(pos.x, pos.y + 1, pos.z);
			app->physics->AddBody(c, 0);
			sceneryCubes.add(c);
		}
	}
}

void ModuleSceneIntro::AddCurveWallCircuit(vec3 initPos, vec3 finalPos, float angle, int walls, bool exterior) {
	assert(angle < 1.0f && angle > -1.0f);
	float distance = length(finalPos - initPos);
	vec3 mid_point = (finalPos - initPos) / 2.0f + initPos;

	vec3 direction = finalPos - initPos;
	direction = normalize(direction);

	vec3 hulio = { -direction.z, 0, direction.x };
	hulio = normalize(hulio);

	float max_radi = distance / 2.0f;
	float seg_to_high_point = angle * max_radi;

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
	c.size = { 1, 2, 1 };

	vec3 central_pos;

	if (exterior == true) {
		// Curve Exterior
		for (uint j = 0; j < walls; j++)
		{
			c.color = (j % 2 == 0) ? White : Red;
			float sub_angle = (angle > 0.0f) ? -(float)j / walls * theta : (float)j / walls * theta;

			central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
			central_pos.y = initPos.y;
			central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

			vec3 to_center = normalize(central_pos - center_circle);
			pos = central_pos + ((Circuit_Width / 2.0f) * to_center);
			c.SetPos(pos.x, pos.y + 1, pos.z);

			app->physics->AddBody(c, 0);
			sceneryCubes.add(c);
		}
	}
	else {
		// Curve Interior
		for (uint j = 0; j < walls; j++)
		{
			c.color = (j % 2 == 0) ? Yellow : Red;
			float sub_angle = (angle > 0.0f) ? -(float)j / walls * theta : (float)j / walls * theta;

			central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
			central_pos.y = initPos.y;
			central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

			vec3 to_center = normalize(central_pos - center_circle);
			pos = central_pos + ((Circuit_Width / 2.0f) * -to_center);
			c.SetPos(pos.x, pos.y + 1, pos.z);

			app->physics->AddBody(c, 0);
			sceneryCubes.add(c);
		}
	}
}

void ModuleSceneIntro::AddCheckPoint(vec3 position, float angle, float circuitW, Color color, int id, bool startChecked) {
	// Sensor
	Cube sensor;
	sensor.size = { 5, 5, circuitW };
	sensor.SetPos(position.x, position.y + 3, position.z);
	sensor.SetRotation(angle, { 0, 1, 0 });

	// Variable to position the visual sensor
	float radius = circuitW / 2;
	vec3 positionLeftFlag(0, position.y + 2.9, radius);
	vec3 positionRightFlag(0, position.y + 2.9, -radius);
	float theta = angle * M_PI / 180;
	positionLeftFlag.x += radius * sin(theta); positionLeftFlag.z = positionLeftFlag.z * cos(theta);
	positionRightFlag.x -= radius * sin(theta); positionRightFlag.z = positionRightFlag.z * cos(theta);

	// Sensor left mark
	Cylinder leftFlag;
	leftFlag.radius = 2;
	leftFlag.height = 5;
	leftFlag.color = color;
	leftFlag.SetPos(positionLeftFlag.x + position.x, positionLeftFlag.y - 1, positionLeftFlag.z + position.z);
	leftFlag.SetRotation(90, { 0, 0, 1 });

	// Sensor right mark
	Cylinder rightFlag;
	rightFlag.radius = 2;
	rightFlag.height = 5;
	rightFlag.color = color;
	rightFlag.SetPos(positionRightFlag.x + position.x, positionRightFlag.y -1, positionRightFlag.z + position.z);
	rightFlag.SetRotation(90, { 0, 0, 1 });

	// Create Checkpoint
	CheckPoint sensorCP;
	sensorCP.body = app->physics->AddBody(sensor, 0.0f);
	sensorCP.body->SetAsSensor(true);
	sensorCP.body->SetId(id);
	sensorCP.angle = angle;
	sensorCP.checked = startChecked;
	sensorCP.leftC = leftFlag;
	sensorCP.rightC = rightFlag;
	sensorCP.colorBody = sensor;
	sensorCP.colorBody.color = Magenta;

	sceneryCylinders.add(sensorCP.leftC);
	app->physics->AddBody(sensorCP.leftC, 0);
	sceneryCylinders.add(sensorCP.rightC);
	app->physics->AddBody(sensorCP.rightC, 0);

	checkPoints.add(sensorCP);
}

void ModuleSceneIntro::AddConstrainThing(vec3 position, float angle) {
	Cube c1, c2, c3;
	c1.size = { 2, 2, 10};
	c1.SetPos(position.x, position.y, position.z - 10);
	c1.SetRotation(angle, { 0, 1, 0 });
	c1.color = Red;

	c2.size = { 2, 2, 10 };
	c2.SetPos(position.x, position.y, position.z + 10);
	c2.SetRotation(angle, { 0, 1, 0 });
	c2.color = Red;

	c3.size = { 2, 2, 10 };
	c3.SetPos(position.x, position.y, position.z);
	c3.SetRotation(angle, { 0, 1, 0 });
	c3.color = Red;

	btTransform frameInA;
	frameInA.getBasis().setEulerZYX(0, 0, M_PI / 2);
	frameInA.setOrigin(btVector3(0, 0, 0));

	btTransform frameInB;
	frameInB.getBasis().setEulerZYX(0, 0, M_PI / 2);
	frameInB.setOrigin(btVector3(0, 0, 0));

	GusanoRojo snake;
	snake.c1 = c1;
	snake.c2 = c2;
	snake.c3 = c3;
	snake.bodyA = app->physics->AddBody(snake.c1, 1);
	snake.bodyB = app->physics->AddBody(snake.c2, 1);
	snake.bodyC = app->physics->AddBody(snake.c3, 1);

	snake.position = position;

	app->physics->AddConstraintSlider(*snake.bodyA, *snake.bodyB, frameInA, frameInB);
	app->physics->AddConstraintSlider(*snake.bodyA, *snake.bodyC, frameInA, frameInB);
	app->physics->AddConstraintSlider(*snake.bodyB, *snake.bodyC, frameInA, frameInB);

	serpientes.add(snake);
}

void ModuleSceneIntro::AddMushroom(vec2 position) {
	AddCylinder({ position.x, 10,  position.y }, 2, 20, White, 90, false, false, true);
	AddCylinder({ position.x, 21,  position.y }, 15, 2, Red, 90, false, false, true);
}


// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// ====================================================
		//			 Sun (independent from state)
		// ====================================================

	sun.SunBall.SetPos(-220 + cosf(sunTimer.Read() * sun.speed) * sun.movement_width, sinf(sunTimer.Read() * sun.speed) * sun.movement_height, 250);
	sun.SunBall.Render();

	// ==========================
	//	   	  Game states
	// ==========================
	switch (state)
	{
	case TITLESCREEN:
	{
		// Check Music
		if (playingMusic == false) {
			app->audio->PlayMusic("Assets/audio/music/titleScreen.ogg");
			playingMusic = true;
		}

		// ==========================
		//			INPUT
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			state = GameState::GAMEPLAY;
			LOG("Loading Gameplay screen");

			cronometro.Start();
			currentLap = LapState::START;
			playingMusic = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			return UPDATE_STOP;
		}

		// ==========================
		//			Update
		// ==========================

		titleCam = true;
		CameraPlayer();

		break;
		}
	case GAMEPLAY:
	{
		titleCam = false;

		// Check Music
		if (playingMusic == false) {
			if (playerUnderWater != true) {
				if (currentLap != LapState::LASTLAP) {
					app->audio->PlayMusic("Assets/audio/music/gameplay_coconutMall.ogg");
				}
				else {
					app->audio->PlayMusic("Assets/audio/music/gameplay_coconutMall_lastLap.ogg");
				}
			}
			else {
				if (currentLap != LapState::LASTLAP) {
					app->audio->PlayMusic("Assets/audio/music/gameplay_underwater_coconutMall.ogg");
				}
				else {
					app->audio->PlayMusic("Assets/audio/music/gameplay_underwater_coconutMall_lastLap.ogg");
				}
			}
			playingMusic = true;
		}

		// ==========================
		//			INPUT
		// ==========================

		// Auto Win
		if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
			areYouWinningSon = RaceState::WIN;
			state = GameState::ENDSCREEN;
			playingMusic = false;
		}
		// Auto Loose
		if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
			areYouWinningSon = RaceState::LOSE;
			state = GameState::ENDSCREEN;
			playingMusic = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
			SceneReset();
		}

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
			debug = !debug;
		}

		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			freeCam = !freeCam;
		}

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			app->audio->PlayFx(winFx);
		}

		// ==========================
		//			Update
		// ==========================

		if (app->player->countdown <= 0) {
			app->player->allowPlayerControl = true;
		}

		// Underwater control
		if (app->player->position.getX() > -637.5f && app->player->position.getX() < 110) {
			if (app->player->position.getZ() > -37.5f && app->player->position.getZ() < 710) {
				if (app->player->position.getY() < 0 && lastPlayerPosY > 0) {
					playingMusic = false;
					playerUnderWater = true;
					app->physics->SetGravity({ 0, -5, 0 });
				}
				else if (app->player->position.getY() > 0 && lastPlayerPosY < 0) {
					playingMusic = false;
					playerUnderWater = false;
					app->physics->SetGravity({ 0, -10, 0 });
				}
			}
		}

		lastPlayerPosY = app->player->position.getY();

		// Camera
		CameraPlayer();

		// Draw
		susPos.x = app->player->position.getX() - 1.5f;
		susPos.y = app->player->position.getY() + 0.25;
		susPos.z = app->player->position.getZ() - 0.5f;

		app->renderer3D->DrawTexture(susTex, susPos, 3.0f);

		p2List_item<GusanoRojo>* d = serpientes.getFirst();
		while (d != NULL) {
			d->data.c1.SetPos(d->data.bodyA->GetPos().getX(), d->data.bodyA->GetPos().getY(), d->data.bodyA->GetPos().getZ());
			d->data.c1.SetRotationEuler(d->data.bodyA->GetRotation());
			d->data.c1.Render();

			d->data.c2.SetPos(d->data.bodyB->GetPos().getX(), d->data.bodyB->GetPos().getY(), d->data.bodyB->GetPos().getZ());
			d->data.c2.SetRotationEuler(d->data.bodyB->GetRotation());
			d->data.c2.Render();

			d->data.c3.SetPos(d->data.bodyC->GetPos().getX(), d->data.bodyC->GetPos().getY(), d->data.bodyC->GetPos().getZ());
			d->data.c3.SetRotationEuler(d->data.bodyC->GetRotation());
			d->data.c3.Render();
			d = d->next;
		}

		// Checkpoints
		p2List_item<CheckPoint>* c = checkPoints.getFirst();
		while(c != NULL){
			if (c->data.checked == false) {
				c->data.colorBody.Render();
			}
			c = c->next;
		}
		break;
		}
	case ENDSCREEN:
	{
		// ==========================
		//			INPUT
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
			SceneReset();
		}

		// ===============================
		//    DO shit, i mean, UPDATE()
		// ===============================

		switch (areYouWinningSon)
		{
		case WIN:
			if (playingMusic == false) {
				playingMusic = true;
				app->audio->PlayMusic("Assets/audio/music/end_win.ogg");
				app->audio->PlayFx(winFx);
			}

			app->renderer3D->DrawTexture(pistonCup, { 40, 0, 350 }, 20.0f);

			break;
		case LOSE:
			if (playingMusic == false) {
				playingMusic = true;
				app->audio->PlayMusic("Assets/audio/music/end_lose.ogg");
				app->audio->PlayFx(loseFx);
			}

			break;
		case DEFAULT:
			//               �
			break;
		}

		if (app->player->allowPlayerControl == true) {
			app->player->allowPlayerControl = false;
		}

		// Camera
		CameraPlayer();

		// Draw
		susPos.x = app->player->position.getX() - 1.5f;
		susPos.y = app->player->position.getY() + 2;
		susPos.z = app->player->position.getZ();

		app->renderer3D->DrawTexture(susTex, susPos, 3.0f);

		break;
	}
	}

	// ====================================================
	//						Render Ground
	// ====================================================

	p2List_item<Cube>* c = sceneryCubes.getFirst();
	while (c != NULL) {
		c->data.Render();
		c = c->next;
	}

	p2List_item<Sphere>* s = scenerySpheres.getFirst();
	while (s != NULL) {
		s->data.Render();
		s = s->next;
	}

	p2List_item<Cylinder>* cy = sceneryCylinders.getFirst();
	while (cy != NULL) {
		cy->data.Render();
		cy = cy->next;
	}

	// Draw textures

	app->renderer3D->DrawTexture(a1, { -700, -30, 250 }, 100.0f);
	app->renderer3D->DrawTexture(shrek, { -570, 20, 250 }, 100.0f);
	app->renderer3D->DrawTexture(a4, { -300, 1, 0 }, 50.0f);
	app->renderer3D->DrawTexture(f1, { -110, -25, 325 }, 25.0f);
	app->renderer3D->DrawTexture(f2, { -200, -20, 100 }, 15.0f);
	app->renderer3D->DrawTexture(f3, { -150, -50, 0 }, 50.0f);
	app->renderer3D->DrawTexture(f4, { -50, -20, 480 }, 15.0f);

	app->renderer3D->DrawTexture(thrompTex, { 10, -2, 300 }, 25.0f);
	app->renderer3D->DrawTexture(thrompTex, { 65, -2, 300 }, 25.0f);

	app->renderer3D->DrawTexture(meta, { 33, -5, 300 }, 35.0f);

	// Los SUSmbas las gradas
	app->renderer3D->DrawTexture(a3, { 75, 5, 480 }, 10.0f);
	app->renderer3D->DrawTexture(a3, { 76, 6, 420 }, 9.0f);
	app->renderer3D->DrawTexture(a3, { 76, 6, 190 }, 10.0f);
	app->renderer3D->DrawTexture(a3, { 74, 4, 140 }, 8.0f);


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CameraPlayer() {
	if (freeCam == false && titleCam == false) {
		if (app->player->position.getY() > Camera_Fall_Dist) {
			// Camera following player
			float cameraDistance = 20;

			// Get player position + forward vec3 from X and Z axis
			float camX = app->player->position.getX() - cameraDistance * app->player->GetVehicleForwardVec().x;
			float camY = app->player->position.getY() - cameraDistance * app->player->GetVehicleForwardVec().y + 8;
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
	else if (titleCam == true) {

		// La cinem�tica de la c�mara est� muy elaborada para nada es el movimiento del sol cambiando el eje Z por el Y y bajando la altura
		app->camera->Position = { -250 + cosf(sunTimer.Read() * sun.speed) * sun.movement_width, 25, 200 + sinf(sunTimer.Read() * sun.speed) * sun.movement_height };
		app->camera->LookAt(vec3(-375, 0, 375));
	}
}