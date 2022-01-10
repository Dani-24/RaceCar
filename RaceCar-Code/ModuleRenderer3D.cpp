#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	colorChangeTimer.Start();

	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(Cyan.r, Cyan.g, Cyan.b, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.05f, 0.05f, 0.05f, 1.0f);
		lights[0].diffuse.Set(0.15f, 0.15f, 0.15f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		lights[1].ref = GL_LIGHT1;
		lights[1].ambient.Set(0.1f, 0.1f, 0.1f, 1.0f);
		lights[1].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[1].SetPos(0.0f, 100.0f, 0.0f);
		lights[1].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		lights[1].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->GetViewMatrix());

	// ====================================
	//				Lights	
	// ====================================

	// light 0 on cam pos
	lights[0].SetPos(app->camera->Position.x, app->camera->Position.y, app->camera->Position.z);

	lights[1].SetPos(app->scene_intro->sun.SunBall.GetPos().x, app->scene_intro->sun.SunBall.GetPos().y, app->scene_intro->sun.SunBall.GetPos().z);

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		lights[i].Render();
	}

	// ====================================
	//		   	 Color change
	// ====================================

	if (app->scene_intro->sun.SunBall.GetPos().y < 100 && app->scene_intro->sun.SunBall.GetPos().y > -50) {
		ChangeColor(Orange);
	}
	if (app->scene_intro->sun.SunBall.GetPos().y < -50) {
		ChangeColor(Black);
	}
	if (app->scene_intro->sun.SunBall.GetPos().y > 100) {
		ChangeColor(Cyan);
	}

	if (colorChanged != true) {
		ColorUpdate();
	}

	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::ColorUpdate() {
	// R
	if (currentColor.r < desiredColor.r) {
		currentColor.r += colorChangeSpeed;
	}
	else if (currentColor.r > desiredColor.r) {
		currentColor.r -= colorChangeSpeed;
	}
	else {
		redChanged = true;
	}

	// G
	if (currentColor.g < desiredColor.g) {
		currentColor.g += colorChangeSpeed;
	}
	else if (currentColor.g > desiredColor.g) {
		currentColor.g -= colorChangeSpeed;
	}
	else {
		greenChanged = true;
	}

	// B
	if (currentColor.b < desiredColor.b) {
		currentColor.b += colorChangeSpeed;
	}
	else if (currentColor.b > desiredColor.b) {
		currentColor.b -= colorChangeSpeed;
	}
	else {
		blueChanged = true;
	}

	glClearColor(currentColor.r,currentColor.g,currentColor.b, 1.0f);

	if (redChanged == true && greenChanged == true && blueChanged == true) {
		colorChanged = true;
	}
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(app->window->window);
	return UPDATE_CONTINUE;
}

 //Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	IMG_Quit();

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetBGColor(int R, int G, int B)
{
	glClearColor(R, G, B, 1.0f);
}

void ModuleRenderer3D::ChangeColor(Color colorToChange) {
	desiredColor = colorToChange;
	colorChanged = redChanged = greenChanged = blueChanged = false;
}

uint ModuleRenderer3D::LoadTexture(const char* path) {
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		LOG("error loading image %s", IMG_GetError());
		return 0;
	}

	Uint32 texture;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	int ret = glGetError();
	if (ret != GL_NO_ERROR)
	{
		LOG("GL Error in loadTexture: %i", ret);
	}

	SDL_FreeSurface(surface);
	return texture;
}

void ModuleRenderer3D::DrawTexture(uint texture, vec3 pos, float size) {
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(pos.x, pos.y, pos.z);
	glTexCoord2f(0, 1); glVertex3f(pos.x, pos.y + size, pos.z);
	glTexCoord2f(1, 1); glVertex3f(pos.x + size, pos.y + size, pos.z);
	glTexCoord2f(1, 0); glVertex3f(pos.x + size, pos.y, pos.z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}