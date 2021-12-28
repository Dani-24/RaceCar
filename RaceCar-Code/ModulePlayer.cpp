#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	position.setValue(0, 0, 0);
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// Variables :

	engineFx = app->audio->LoadFx("Assets/audio/fx/kartEngine.wav");

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

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
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

	vehicle = app->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, 0);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	position = vehicle->GetPos();

	if (position.getY() < Vehicle_Fall_Dist || app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		vehicle->SetPos(0, 0, 0);
	}

	// =========================================================
	//						Car Movement
	// =========================================================

	// Jump bc yes
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && app->scene_intro->debug == true) {
		vehicle->Push(0, 3000, 0);
	}

	// Reset Variables (more or less like !Key_Down)
	turn = acceleration = brake = 0.0f;
	// Move forward
	if(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		// Max Velocity fixed to 100 km/h
		// If velocity is negative the vehicle brakes instead of accelerating
		if (vehicle->GetKmh() < 0) {
			brake = BRAKE_POWER / 20;

		}
		else if (vehicle->GetKmh() <= 100) {
			acceleration = MAX_ACCELERATION;

			if (playingEngineFx == false) {
				app->audio->PlayFx(engineFx);
				playingEngineFx = true;
			}
		}
	}
	
	// Move backwards
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		// Same as forward movement but for backwards. Velocity capped at 25km/h
		if (vehicle->GetKmh() > 0) {
			brake = BRAKE_POWER / 20;
		}
		else if (vehicle->GetKmh() > -25) {
			acceleration = -MAX_ACCELERATION;

			if (playingEngineFx == false) {
				app->audio->PlayFx(engineFx);
				playingEngineFx = true;
			}
		}
	}

	// Brake if there is no acceleration
	if (app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
		brake = BRAKE_POWER / 200;
	}

	// Turn left
	if(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES) {
			turn += TURN_DEGREES;
		}
	}

	// Turn Right
	if(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES) {
			turn -= TURN_DEGREES;
		}
	}

	// Apply inputs to vehicle
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	// =========================================================
	//						Post Update
	// =========================================================

	vehicle->Render();

	// =========================================================
	//						Window Title
	// =========================================================

	char title[80];
	sprintf_s(title, "Racing GP Piston Cup ඞ || Car Speed: %.1f Km/h", vehicle->GetKmh());
	app->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

vec3 ModulePlayer::GetVehicleForwardVec() {
	return vehicle->GetForwardVector();
}