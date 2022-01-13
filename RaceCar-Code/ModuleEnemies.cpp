#include "Globals.h"
#include "Application.h"
#include "ModuleEnemies.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModuleEnemies::ModuleEnemies(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	turn = acceleration = brake = 0.0f;
}

ModuleEnemies::~ModuleEnemies()
{}

bool ModuleEnemies::Start()
{
	LOG("Loading Enemies");

	// Variables :

	engineFx = app->audio->LoadFx("Assets/audio/fx/gameplay_kartEngine.wav");
	turboFx = app->audio->LoadFx("Assets/audio/fx/gameplay_turbo.wav");
	fallFx = app->audio->LoadFx("Assets/audio/fx/gameplay_fallFromMap.wav");

	float Y = 1;

	AddKart({5, Y, 200});
	AddKart({ -5, Y, 195 });
	AddKart({ 5, Y, 190 });
	AddKart({ -5, Y, 185 });
	AddKart({ 5, Y, 180 });
	AddKart({ -5, Y, 175 });
	AddKart({ 5, Y, 170 });
	AddKart({ -5, Y, 165 });
	AddKart({ 5, Y, 160 });
	AddKart({ -5, Y, 155 });
	AddKart({ 5, Y, 150 });

	return true;
}

void ModuleEnemies::AddKart(vec3 position) {
	// ======================================================
	//                         Vehicle
	// ======================================================

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.5f;
	float half_length = car.chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicles.add(app->physics->AddVehicle(car));
	vehicles.getLast()->data->SetPos(position.x, position.y, position.z);
}

bool ModuleEnemies::CleanUp()
{
	LOG("Unloading enemies");

	return true;
}

update_status ModuleEnemies::Update(float dt)
{
	p2List_item<PhysVehicle3D*>* c = vehicles.getFirst();

	while (c != NULL) {
		if (app->scene_intro->state == GameState::GAMEPLAY) {
			if (c->data->GetPos().getY() < Camera_Fall_Dist) {
				if (fallFxPlayed == false) {
					app->audio->PlayFx(fallFx);
					fallFxPlayed = true;
				}
			}
			else {
				if (fallFxPlayed == true) {
					fallFxPlayed = false;
				}
			}
			if (c->data->GetPos().getY() < Vehicle_Fall_Dist || app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
				c->data->SetPos(0, 1, 0);
			}
			// =========================================================
			//						Car Movement
			// =========================================================

			// Jump bc yes
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && app->scene_intro->debug == true) {
				c->data->Push(0, 300, 0);
			}

			// Reset Variables (more or less like !Key_Down)
			turn = acceleration = brake = 0.0f;
			// Move forward
			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				// Max Velocity fixed to 100 km/h
				if (c->data->GetKmh() <= 100) {
					acceleration = MAX_ACCELERATION;
					if (playingEngineFx == false) {
						app->audio->PlayFx(engineFx);
						playingEngineFx = true;
					}
				}
				if (app->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT) {
					if (c->data->GetKmh() < 150) {
						acceleration = MAX_ACCELERATION * 10;
						if (turboFxPlayed == false) {
							app->audio->PlayFx(turboFx);
							turboFxPlayed = true;
						}
					}
				}
				else {
					if (turboFxPlayed == true) {
						turboFxPlayed = false;
					}
					if (c->data->GetKmh() > 100) {
						brake = BRAKE_POWER / 20;
					}
				}
			}
			// Move backwards
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				if (c->data->GetKmh() > 10) {
					acceleration = -MAX_ACCELERATION * 5;
				}
				else if (c->data->GetKmh() > -25) {
					acceleration = -MAX_ACCELERATION;
					if (playingEngineFx == false) {
						app->audio->PlayFx(engineFx);
						playingEngineFx = true;
					}
				}
			}
			// Brake if there is no acceleration
			if (app->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT) {
				brake = BRAKE_POWER / 100;
				if (c->data->GetKmh() > 100) {
					brake = BRAKE_POWER / 20;
				}
			}
			// Turn left
			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				if (c->data->GetKmh() > 110) {
					if (turn < TURN_DEGREES) {
						turn += TURN_DEGREES / 5;
					}
				}
				else if (c->data->GetKmh() > 98) {
					if (turn < TURN_DEGREES) {
						turn += TURN_DEGREES / 3;
					}
				}
				else if (c->data->GetKmh() > 75) {
					if (turn < TURN_DEGREES) {
						turn += TURN_DEGREES / 1.5;
					}
				}
				else {
					if (turn < TURN_DEGREES) {
						turn += TURN_DEGREES;
					}
				}
			}
			// Turn Right
			if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				if (c->data->GetKmh() > 110) {
					if (turn < TURN_DEGREES) {
						turn -= TURN_DEGREES / 5;
					}
				}
				else if (c->data->GetKmh() > 98) {
					if (turn < TURN_DEGREES) {
						turn -= TURN_DEGREES / 3;
					}
				}
				else if (c->data->GetKmh() > 75) {
					if (turn < TURN_DEGREES) {
						turn -= TURN_DEGREES / 1.5;
					}
				}
				else {
					if (turn < TURN_DEGREES) {
						turn -= TURN_DEGREES;
					}
				}
			}
			// Apply inputs to vehicle
			c->data->ApplyEngineForce(acceleration);
			c->data->Turn(turn);
			c->data->Brake(brake);
		}
		// =========================================================
		//						Post Update
		// =========================================================
		c->data->Render();
		c = c->next;
	}
	return UPDATE_CONTINUE;
}