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

	currentLap = LapState::FIRSTLAP;

	areYouWinningSon = RaceState::DEFAULT;

	playingMusic = false;
	freeCam = false;

	// ===================================
	//				Audio
	// ===================================
	winFx = app->audio->LoadFx("Assets/audio/fx/gameplay_win.wav");
	loseFx = app->audio->LoadFx("Assets/audio/fx/gameplay_lose.wav");
	lapFx = app->audio->LoadFx("Assets/audio/fx/gameplay_lap.wav");
	finalLapFx = app->audio->LoadFx("Assets/audio/fx/gameplay_lastLap.wav");
	checkpointFx = app->audio->LoadFx("Assets/audio/fx/gameplay_checkpoint.wav");

	// ===================================
	//				Textures
	// ===================================
	susTex = app->renderer3D->LoadTexture("Assets/textures/amogus.png");

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

	// Map scenery details
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

void ModuleSceneIntro::CreateCircuit() {
	// ================
	//  Circuit track:
	// ================

	AddLinearCircuit({ 50, 0, 100 }, { 50, 0, 600 }, 100);

	AddCheckPoint({ 50, 1, 300 }, 90, 30, Black); // meta

	AddCircularCircuit({ 50, 1, 600 }, { -25, 1, 675 }, -0.4f, 60, 30);

	AddLinearCircuit({ -25, 0, 675 }, { -425, 0, 675 }, 100);

	AddCircularCircuit({ -425, 1, 675 }, { -425, 1, 525 }, -0.99f, 120, 60);

	AddLinearCircuit({ -425, 0, 525 }, { -335, 0, 525 }, 50);

	AddCube({ -189, -17.5f, 525 }, { 300, 10, Circuit_Width * 1.5f }, Red, -5, false, false, true);

	AddLinearCircuit({ -335, 0, 525 }, { -100, -21, 525 }, 50);

	AddCircularCircuit({ -100, -20, 521 }, { -60, -20, 471 }, 0.45f, 30, 15);

	AddLinearCircuit({ -55.5f, -20, 480 }, { -55.5f, -20, 300 }, 50);

	AddCircularCircuit({ -50, -20, 300 }, { -150, -20, 300 }, 0.99f, 30, 15);

	AddCube({ -300, -17.5f, 264 }, { Circuit_Width * 1.5f , 10, 300 }, Red, -5, true);

	//Old Circuit
	/*// 1
	AddCheckPoint({ 0, 0, 210 }, 90, 30, Black); // meta
	AddLinearCircuit({ 0, 0, 25 }, { 0, 0, 425 }, 60);

	// 1.2
	AddCircularCircuit({ 0, 0, 425 }, { -25, 0, 450 }, -0.45f, 10, 5);

	// 2
	AddLinearCircuit({ -25, 0, 450 }, { -50, 0, 450 }, 5);

	// 2.1
	AddCircularCircuit({ -50, 0, 450 }, { -75, 0, 425 }, -0.45f, 10, 5);

	// 3
	AddLinearCircuit({ -75, 0, 425 }, { -75, 0, 375 }, 10);

	// 4
	AddCircularCircuit({ -75, 0, 375 }, { -70, 0, 370 }, -0.225f, 5, 0);
	AddLinearCircuit({ -65, 0, 365 }, { -45, 0, 350 }, 5);

	// 5
	AddCircularCircuit({ -45, 0, 350 }, { -45, 0, 325 }, 0.45f, 10, 1);
	AddLinearCircuit({ -45, 0, 325 }, { -60, 0, 300 }, 5);

	AddCheckPoint({ -51, 0, 315 }, -58, 30, Red);

	AddCircularCircuit({ -60, 0, 300 }, { -75, 0, 300 }, 0.45f, 10, 0);

	// 6
	AddLinearCircuit({ -80, 0, 302 }, { -165, 0, 365 }, 20);
	AddWallCircuit({ -165, 0, 365 }, { -235, 0, 415 }, 15, false);

	// 7
	AddLinearCircuit({ -225, 0, 410 }, { -275, 0, 410 }, 10);

	// 7 brigde
	AddLinearCircuit({ -275, 0, 410 }, { -325, 0, 410 }, 10);

	// 7.2
	AddLinearCircuit({ -325, 0, 410 }, { -350, 0, 410 }, 5);
	AddCircularCircuit({ -350, 0, 410 }, { -375, 0, 385 }, -0.45f, 10, 3);

	// 8
	AddWallCircuit({ -375, 0, 385 }, { -375, 0, 300 }, 15, false);

	// 9
	AddCircularCircuit({ -375, 0, 300 }, { -400, 0, 275 }, 0.45f, 10, 3);
	AddLinearCircuit({ -400, 0, 275 }, { -425, 0, 275 }, 5);
	AddCircularCircuit({ -425, 0, 275 }, { -450, 0, 300 }, 0.45f, 10, 3);

	// 10
	AddLinearCircuit({ -450, 0, 300 }, { -450, 0, 400 }, 20);

	// 11
	AddCircularCircuit({ -450, 0, 400 }, { -550, 0, 400 }, -0.99f, 40, 20);

	// 12
	AddLinearCircuit({ -550, 0, 400 }, { -550, 0, 300 }, 20);

	// 13
	AddWallCircuit({ -570, 0, 290 }, { -590, 0, 290 }, 4, false);
	AddLinearCircuit({ -560, 0, 300 }, { -560, 0, 200 }, 20, 50);

	// Rute 14 ================
	AddLinearCircuit({ -543, 0, 190 }, { -520, 0, 170 }, 5);

	// 16 B
	AddCircularCircuit({ -520, 0, 170 }, { -490, 0, 160 }, -0.225f, 10, 3);

	AddCircularCircuit({ -490, 0, 160 }, { -465, 0, 135 }, 0.45f, 10, 3);

	AddLinearCircuit({ -465, 0, 135 }, { -465, 0, 75 }, 10);

	// 17 B
	AddCircularCircuit({ -465, 0, 75 }, { -495, 0, 50 }, 0.35f, 10, 3);

	// 18 B
	AddLinearCircuit({ -495, 0, 50 }, { -550, 0, 50 }, 10);

	// 19 B
	AddCircularCircuit({ -550, 0, 50 }, { -550, 0, 0 }, -0.99f, 25, 8);

	// 20 B
	AddLinearCircuit({ -550, 0, 0 }, { -425, 0, 0 }, 25);

	// 21 B
	AddCircularCircuit({ -425, 0, 0 }, { -400, 0, 25 }, -0.45f, 10, 3);
	AddCircularCircuit({ -400, 0, 25 }, { -375, 0, 50 }, 0.45f, 10, 3);

	// 22 B ====
	AddWallCircuit({ -375, 0, 50 }, { -340, 0, 50 }, 5, true);
	AddLinearCircuit({ -340, 0, 50 }, { -90, 0, 50 }, 40);

	// 22 C ====
	AddCircularCircuit({ -360, 0, 30 }, { -335, 0, 5 }, -0.45f, 15, 3, 40);
	AddLinearCircuit({ -335, 0, 5 }, { -300, 0, 5 }, 5, 40);

	// Rute 15 ================
	AddWallCircuit({ -570, 0, 200 }, { -570, 0, 165 }, 5, false);
	AddLinearCircuit({ -570, 0, 165 }, { -570, 0, 115 }, 10);

	// 16 A
	AddCircularCircuit({ -570, 0, 115 }, { -545, 0, 90 }, -0.45f, 10, 3);

	AddLinearCircuit({ -545, 0, 90 }, { -350, 0, 90 }, 30);

	// 17 A
	AddCircularCircuit({ -350, 0, 90 }, { -325, 0, 115 }, -0.45f, 10, 3);

	// 18
	AddCircularCircuit({ -325, 0, 115 }, { -300, 0, 140 }, 0.45f, 10, 3);

	// 19 A (Loop)
	AddLinearCircuit({ -300, 0, 140 }, { -250, 0, 140 }, 10);

	AddLinearCircuit({ -250, 0, 100 }, { -200, 0, 100 }, 10);

	// 20 A
	AddLinearCircuit({ -200, 0, 100 }, { -100, 0, 100 }, 20);

	// 21 A
	AddCircularCircuit({ -100, 0, 100 }, { -75, 0, 75 }, 0.45f, 10, 3);

	// === Rute Union ===
	// 25 Pre

	// boxes - 21A
	AddCurveWallCircuit({ -75, 0, 75 }, { -50, 0, 42 }, -0.45f, 4, false);

	// boxes - 25
	AddCurveWallCircuit({ -75, 0, 25 }, { -50, 0, 58 }, 0.45f, 5, false);

	// 22B - 21A
	AddCurveWallCircuit({ -92, 0, 51 }, { -78, 0, 85 }, -0.45f, 3, false);

	// 22B - 25
	AddCurveWallCircuit({ -92, 0, 50 }, { -77, 0, 20 }, 0.45f, 3, false);

	// 25
	AddCircularCircuit({ -75, 0, 25 }, { -50, 0, 0 }, -0.45f, 10, 3);

	AddLinearCircuit({ -50, 0, 0 }, { -25, 0, 0 }, 5);

	AddCircularCircuit({ -25, 0, 0 }, { 0, 0, 25 }, -0.45f, 10, 3);

	// 0
	AddLinearCircuit({ -50, 0, 50 }, { 25, 0, 50 }, 10, 15);

	AddCircularCircuit({ 25, 0, 50 }, { 50, 0, 75 }, -0.45f, 10, 3, 15);
	AddLinearCircuit({ 50, 0, 75 }, { 50, 0, 425 }, 60, 15);
	AddCircularCircuit({ 50, 0, 425 }, { 25, 0, 450 }, -0.45f, 10, 5, 15);*/
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
				//sceneryCubes.add(groundToAdd);

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
			central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

			vec3 to_center = normalize(central_pos - center_circle);
			pos = central_pos + ((Circuit_Width / 2.0f) * -to_center);
			c.SetPos(pos.x, pos.y + 1, pos.z);

			app->physics->AddBody(c, 0);
			sceneryCubes.add(c);
		}
	}
}

void ModuleSceneIntro::AddCheckPoint(vec3 position, float angle, float circuitW, Color color) {
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
	sensorCP.body->SetId(2);
	sensorCP.angle = angle;
	sensorCP.checked = false;
	sensorCP.leftC = leftFlag;
	sensorCP.rightC = rightFlag;

	sceneryCylinders.add(sensorCP.leftC);
	app->physics->AddBody(sensorCP.leftC, 0);
	sceneryCylinders.add(sensorCP.rightC);
	app->physics->AddBody(sensorCP.rightC, 0);

	checkPoints.add(sensorCP);
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

			playingMusic = false;
			// Aqui deberia resetear la scena para hacer loop

		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			return UPDATE_STOP;
		}

		// ==========================
		//			Update
		// ==========================

		app->camera->Position = { -250, 475, 250 };
		app->camera->LookAt(vec3(-250, 0, 251));

		break;
	case GAMEPLAY:

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

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
			cronometro.Stop();
			playingMusic = false;
			app->player->Respawn(app->player->initialPos, -1.57f);
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

		if (app->player->allowPlayerControl != true) {
			app->player->allowPlayerControl = true;
		}

		//LOG("X: %.2f, Y: %.2f, Z: %.2f", app->player->position.getX(), app->player->position.getY(), app->player->position.getZ());

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

		break;
	case ENDSCREEN:

		// ==========================
		//			INPUT
		// ==========================

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			state = GameState::TITLESCREEN;
			LOG("Exiting to Title");
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

			break;
		case LOSE:
			if (playingMusic == false) {
				playingMusic = true;
				app->audio->PlayMusic("Assets/audio/music/end_lose.ogg");
				app->audio->PlayFx(loseFx);
			}

			break;
		case DEFAULT:
			//               ª
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

	// ====================================================
	//						Render Ground
	// ====================================================

	p2List_item<Cube>* c = sceneryCubes.getFirst();
	while (c != NULL) {
		c->data.Render();
		c = c->next;
	}

	p2List_item<Cylinder>* cy = sceneryCylinders.getFirst();
	while (cy != NULL) {
		cy->data.Render();
		cy = cy->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CameraPlayer() {
	if (freeCam == false) {
		if (app->player->position.getY() > Camera_Fall_Dist) {
			// Camera following player
			float cameraDistance = 20;

			// Get player position + forward vec3 from X and Z axis
			float camX = app->player->position.getX() - cameraDistance * app->player->GetVehicleForwardVec().x;
			float camY = app->player->position.getY() + 8;
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
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}