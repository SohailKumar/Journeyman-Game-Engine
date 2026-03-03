#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h";
#include "SDL3/SDL_main.h";
#include <cstdio>
#include <wtypes.h>
#include <OgreRenderSystem.h>
#include <Ogre.h>
#include "OgreRoot.h"
#include <string>
#include <sstream>
#include <iostream>

static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;

// Start function
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	SDL_SetAppMetadata("Renderer Test", "0.1", "journeymanengine");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	int windowWidth = 1280;
	int windowHeight = 720;
	window = SDL_CreateWindow("Render Test", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	void* hwnd_ptr = (SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
	//std::uintptr_t hwnd = reinterpret_cast<std::uintptr_t>(hwnd_ptr);
	HWND hwnd = static_cast<HWND>(hwnd_ptr);
	Ogre::String windowHandleStr = Ogre::StringConverter::toString((size_t)hwnd);

	Ogre::Root* root = new Ogre::Root("../bin/Release/plugins.cfg");
	if (!root) {
		printf("H:LKAJDSFOIUWEROIJDFL:KSJFEOI");
		return SDL_APP_FAILURE;
	}
	std::cout << "WHATR IS IN HERE: %s" << windowHandleStr << std::endl;
	root->initialise(false);

	//Ogre::RenderSystem* rs = root->getRenderSystemByName("Direct3D11 Rendering Subsystem");
	//if (!rs) {
	//	// Fallback to OpenGL if D3D11 isn't found
	//	rs = root->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");
	//}
	//if (!rs) {
	//	printf("NOOOO");
	//	return SDL_APP_FAILURE;
	//}
	//Ogre::RenderSystem a = new RenderSystem();

	//root->initialise();

	Ogre::NameValuePairList params;
	params["externalWindowHandle"] = windowHandleStr;
	root->createRenderWindow("Render Test Window", windowWidth, windowHeight, false, &params);

	return SDL_APP_CONTINUE;
}

// Runs on events (mouse input, keypresses, etc)
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

//Update function
SDL_AppResult SDL_AppIterate(void *appstate) 
{
	//const double now = ((double)SDL_GetTicks()) / 1000.0; // ms to sec

	//const float red = (float)(0.5 + 0.5 * SDL_sin(now));
	//const float blue = (float)(0.5 + 0.5 * SDL_sin(now));
	//const float green = (float)(0.5 + 0.5 * SDL_sin(now));
	//SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

	//SDL_RenderClear(renderer);

	//SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{

}