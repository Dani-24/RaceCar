#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// Variables :
	engineFx = app->audio->LoadFx("Assets/audio/fx/gameplay_kartEngine.wav");
	turboFx = app->audio->LoadFx("Assets/audio/fx/gameplay_turbo.wav");
	fallFx = app->audio->LoadFx("Assets/audio/fx/gameplay_fallFromMap.wav");
	respawnFx = app->audio->LoadFx("Assets/audio/fx/zas.wav");

	CreateCar();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::CreateCar() {
	// ======================================================
	//                         Vehicle
	// ======================================================

	VehicleInfo car;

	// Car properties ----------------------------------------
	// Hitbox
	car.chassis1_size.Set(3.5f, 3, 7);
	// Respaldo
	car.chassis2_size.Set(2, 2, 0.5f);
	// Asiento
	car.chassis3_size.Set(2, 0.5f, 2);
	// Marco Volante
	car.chassis4_size.Set(2, 1.75f, 0.5f);
	// Zona motor
	car.chassis5_size.Set(2, 0.75f, 3);
	// aleron delantero
	car.chassis6_size.Set(4, 0.25, 1);
	// lateral der
	car.chassis7_size.Set(0.9f, 0.5f, 3);
	// lateral iz
	car.chassis8_size.Set(0.9f, 0.5f, 3);
	// maletero
	car.chassis9_size.Set(2.5f, 1.5f, 1.5f);
	// aleron
	car.chassis10_size.Set(3.25f, 0.25f, 1);
	// pata aleron der
	car.chassis11_size.Set(0.25f, 0.5f, 0.25f);
	// pata aleron iz
	car.chassis12_size.Set(0.25f, 0.5f, 0.25f);
	// tubo der
	car.chassis13_size.Set(0.75f, 0.5f, 1);
	// tubo iz
	car.chassis14_size.Set(0.75f, 0.5f, 1);
	// Volante
	car.chassis15_size.Set(1, 0.5f, 0.25f);
	// aleron delante delante
	car.chassis16_size.Set(2.5f, 0.25f, 1.5f);
	// Zona motor up
	car.chassis17_size.Set(2, 0.5f, 2);
	// tubo der up
	car.chassis18_size.Set(0.75f, 1, 0.5f);
	// tubo iz up
	car.chassis19_size.Set(0.75f, 1, 0.5f);

	// Hitbox
	car.chassis1_offset.Set(0, 1, 0);
	// Respaldo
	car.chassis2_offset.Set(0, 1.5f, -1.5f);
	// Asiento
	car.chassis3_offset.Set(0, 0.5f, -0.5f);
	// Marco Volante
	car.chassis4_offset.Set(0, 0.75f, 0.75f);
	// Zona motor
	car.chassis5_offset.Set(0, 0.25f, 2.5f);
	// aleron delantero
	car.chassis6_offset.Set(0, 0, 3.5f);
	// lateral der
	car.chassis7_offset.Set(-1.5f, 0, 0);
	// lateral iz
	car.chassis8_offset.Set(1.5f, 0, 0);
	// maletero
	car.chassis9_offset.Set(0, 0.75f, -2.5f);
	// aleron
	car.chassis10_offset.Set(0, 2, -2.75f);
	// pata aleron der
	car.chassis11_offset.Set(-0.75f, 1.75f, -2.75f);
	// pata aleron iz
	car.chassis12_offset.Set(0.75f, 1.75f, -2.75f);
	// tubo der
	car.chassis13_offset.Set(-1, 0, -3.25f);
	// tubo iz
	car.chassis14_offset.Set(1, 0, -3.25f);
	// Volante
	car.chassis15_offset.Set(0, 1.75f, 0.25f);
	// aleron delantero delante
	car.chassis16_offset.Set(0, 0, 4);
	// zona motor up
	car.chassis17_offset.Set(0, 0.75f, 2);
	// tubo der up
	car.chassis18_offset.Set(-1, 0.5f, -3.5f);
	// tubo iz up
	car.chassis19_offset.Set(1, 0.5f, -3.5f);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 10.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 100000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.8f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis1_size.x * 0.5f;
	float half_length = car.chassis1_size.z * 0.5f;

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
	car.wheels[2].connection.Set(half_width + 0.6f * wheel_width, connection_height, -half_length + wheel_radius);
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
	car.wheels[3].connection.Set(-half_width - 0.6f * wheel_width, connection_height, -half_length + wheel_radius);
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
	vehicle->SetPos(initialPos.x, initialPos.y, initialPos.z);

	vehicle->collision_listeners.add(this);
	vehicle->SetId(1);
	vehicle->SetAsSensor(true);
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	int time = app->scene_intro->cronometro.Read() / 1000;

	if (app->scene_intro->state != GameState::TITLESCREEN) {
		position.setValue(vehicle->GetPos().getX(), vehicle->GetPos().getY(), vehicle->GetPos().getZ());

		if (position.getY() < Camera_Fall_Dist) {
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

		if (position.getY() < Vehicle_Fall_Dist || app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			Respawn({ app->scene_intro->checkPoints.getFirst()->data.body->GetPos().getX(), 1, app->scene_intro->checkPoints.getFirst()->data.body->GetPos().getZ() }, app->scene_intro->checkPoints.getFirst()->data.angle + 2.68f);
		}

		// =========================================================
		//						Car Movement
		// =========================================================

		// Jump bc yes
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && app->scene_intro->debug == true) {
			vehicle->Push(0, 300, 0);
		}

		// Reset Variables (more or less like !Key_Down)
		turn = acceleration = brake = 0.0f;

		if (allowPlayerControl == true) {
			// Move forward
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				if (app->scene_intro->playerUnderWater == false) {
					// Max Velocity fixed
					if (vehicle->GetKmh() <= MAX_SPEED) {
						acceleration = MAX_ACCELERATION * 2;

						if (playingEngineFx == false) {
							app->audio->PlayFx(engineFx);
							playingEngineFx = true;
						}
					}

					if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
						if (vehicle->GetKmh() < MAX_TURBO_SPEED) {
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
						if (vehicle->GetKmh() > MAX_SPEED) {
							brake = BRAKE_POWER / 20;
						}
					}
				}
				else {
					// Max Velocity fixed
					if (vehicle->GetKmh() <= MAX_SPEED / 1.5f) {
						acceleration = MAX_ACCELERATION / 1.25f;

						if (playingEngineFx == false) {
							app->audio->PlayFx(engineFx);
							playingEngineFx = true;
						}
					}

					if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
						if (vehicle->GetKmh() < MAX_TURBO_SPEED / 1.5f) {
							acceleration = MAX_ACCELERATION * 2;
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
						if (vehicle->GetKmh() > MAX_SPEED / 1.5f) {
							brake = BRAKE_POWER / 30;
						}
					}
				}

			}

			// Move backwards
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				if (app->scene_intro->playerUnderWater == false) {
					if (vehicle->GetKmh() > 10) {
						acceleration = -MAX_ACCELERATION * 5;
					}
					else if (vehicle->GetKmh() > -MAX_SPEED_BACKWARDS) {
						acceleration = -MAX_ACCELERATION;

						if (playingEngineFx == false) {
							app->audio->PlayFx(engineFx);
							playingEngineFx = true;
						}
					}
				}
				else {
					if (vehicle->GetKmh() > 10) {
						acceleration = -MAX_ACCELERATION * 10;
					}
					else if (vehicle->GetKmh() > -MAX_SPEED_BACKWARDS / 1.5f) {
						acceleration = -MAX_ACCELERATION;

						if (playingEngineFx == false) {
							app->audio->PlayFx(engineFx);
							playingEngineFx = true;
						}
					}
				}
			}

			// Turn left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() > 110) {
					if (turn < TURN_DEGREES) {
						turn += TURN_DEGREES / 2;
					}
				}
				else if (vehicle->GetKmh() > 90) {
					if (turn < TURN_DEGREES) {
						turn += TURN_DEGREES / 1.75;
					}
				}
				else if (vehicle->GetKmh() > 75) {
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
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() > 110) {
					if (turn < TURN_DEGREES) {
						turn -= TURN_DEGREES / 2;
					}
				}
				else if (vehicle->GetKmh() > 90) {
					if (turn < TURN_DEGREES) {
						turn -= TURN_DEGREES / 1.75;
					}
				}
				else if (vehicle->GetKmh() > 75) {
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
		}

		// Brake if there is no acceleration
		if (app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
			brake = BRAKE_POWER / 100;

			if (vehicle->GetKmh() > 100) {
				brake = BRAKE_POWER / 20;
			}
		}

		// Apply inputs to vehicle
		vehicle->ApplyEngineForce(acceleration);
		vehicle->Turn(turn);
		vehicle->Brake(brake);
	}
	// =========================================================
	//						Post Update
	// =========================================================

	vehicle->Render();

	// =========================================================
	//						Window Title
	// =========================================================

	char title[80];
	if (app->scene_intro->state == GameState::TITLESCREEN) {
		sprintf_s(title, "Racing GP Piston Cup || Press Space/Enter to start");
	}
	else {
		if (app->scene_intro->currentLap == LapState::FIRSTLAP) {
			if (app->scene_intro->areYouWinningSon != RaceState::LOSE) {
				sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 1/3 || Time: %d seconds", vehicle->GetKmh(), time);
			}
			else {
				sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 1/3 || YOU LOSE", vehicle->GetKmh());
			}
		}
		else if (app->scene_intro->currentLap == LapState::SECONDLAP) {
			if (app->scene_intro->areYouWinningSon != RaceState::LOSE) {
				sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 2/3 || Time: %d seconds", vehicle->GetKmh(), time);
			}
			else {
				sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 2/3 || YOU LOSE", vehicle->GetKmh());
			}
		}
		else if (app->scene_intro->currentLap == LapState::LASTLAP) {
			if (app->scene_intro->areYouWinningSon != RaceState::LOSE) {
				if (app->scene_intro->areYouWinningSon == RaceState::WIN) {
					sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 3/3 || YOU WIN", vehicle->GetKmh());
				}
				else {
					sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 3/3 || Time: %d seconds", vehicle->GetKmh(), time);
				}
			}
			else {
				sprintf_s(title, "Racing GP Piston Cup || Car Speed: %.1f Km/h || Lap 3/3 || YOU LOSE", vehicle->GetKmh());
			}
		}
	}
	app->window->SetTitle(title);
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2->id == 2)
	{
		LOG("PUTO");
	}
}

void ModulePlayer::Respawn(vec3 position, float angle) {
	vehicle->SetPos(position.x, position.y, position.z);
	vehicle->SetAngularVelocity(0, 0, 0);
	vehicle->Orient(angle + M_PI / 2);
	vehicle->SetLinearVelocity(0, 0, 0);

	//app->audio->PlayFx(respawnFx);
}

vec3 ModulePlayer::GetVehicleForwardVec() {
	return vehicle->GetForwardVector();
}