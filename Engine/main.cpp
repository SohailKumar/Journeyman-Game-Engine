#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h";
#include "SDL3/SDL_main.h";

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

// Start function
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	SDL_SetAppMetadata("Renderer Test", "0.1", "journeymanengine");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Renderer Test", 1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderLogicalPresentation(renderer, 1280, 720, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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

SDL_AppResult SDL_AppIterate(void *appstate) 
{
	const double now = ((double)SDL_GetTicks()) / 1000.0; // ms to sec

	const float red = (float)(0.5 + 0.5 * SDL_sin(now));
	const float blue = (float)(0.5 + 0.5 * SDL_sin(now));
	const float green = (float)(0.5 + 0.5 * SDL_sin(now));
	SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{

}