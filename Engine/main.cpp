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
#include <OgreRTShaderSystem.h>

static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;
Ogre::Root* root;
Ogre::SceneManager* scnMgr;

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

	root = new Ogre::Root("../bin/Release/plugins.cfg");
	if (!root) {
		return SDL_APP_FAILURE;
	}
	std::cout << "WHATR IS IN HERE: %s" << windowHandleStr << std::endl;

	try {
		auto* renderSystem = root->getRenderSystemByName("Direct3D11 Rendering Subsystem");
		root->setRenderSystem(renderSystem);
		root->initialise(false);


		Ogre::NameValuePairList params;
		params["externalWindowHandle"] = windowHandleStr;
		Ogre::RenderWindow* ogreWin = root->createRenderWindow("Render Test Window", windowWidth, windowHeight, false, &params);

		scnMgr = root->createSceneManager();

		if (Ogre::RTShader::ShaderGenerator::initialize()) {
			Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

			// 2. Add the SceneManager to the generator
			shadergen->addSceneManager(scnMgr);

			// 3. Make sure the RTSS knows where to find its own core shaders!
			// These are usually in the /Media/RTShaderLib folder of the Ogre SDK
			//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("path/to/RTShaderLib", "FileSystem");
		}

		scnMgr->setAmbientLight(Ogre::ColourValue::ColourValue(0.5, 0.0, 0.0));

		Ogre::Light* light = scnMgr->createLight("MainLight");
	    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	    lightNode->attachObject(light);
		lightNode->setPosition(0, 10, 15);

	    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		camNode->setPosition(0, 0, 15);
	    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

		Ogre::Camera* cam = scnMgr->createCamera("myCam");
		cam->setNearClipDistance(5); // specific to this sample
		cam->setAutoAspectRatio(true);
		camNode->attachObject(cam);

		Ogre::Viewport* vp = ogreWin->addViewport(cam);
		vp->setBackgroundColour(Ogre::ColourValue::ColourValue(0.2, 0.2 , 0.2));
		

	}
	catch (Ogre::Exception& e) {
		std::cerr << "...................................................\n";
		std::cerr << "Ogre Exception: " << e.getFullDescription() << std::endl;
	}

	return SDL_APP_CONTINUE;
}

// Runs on events (mouse input, keypresses, etc)
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

		try {
			// 1. Create the ManualObject
			Ogre::ManualObject* cube = scnMgr->createManualObject("UserCube");

			// 2. Start defining the mesh using a material
			cube->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);

			// Define the 8 vertices of a cube
			// Front face
			cube->position(-5, 5, 5); // 0
			cube->position(5, 5, 5); // 1
			cube->position(5, -5, 5); // 2
			cube->position(-5, -5, 5); // 3

			// Back face
			cube->position(-5, 5, -5); // 4
			cube->position(5, 5, -5); // 5
			cube->position(5, -5, -5); // 6
			cube->position(-5, -5, -5); // 7

			// 3. Define the triangles (Indices)
			// Front
			cube->triangle(0, 3, 2);
			cube->triangle(2, 1, 0);
			// Right
			cube->triangle(1, 2, 6);
			cube->triangle(6, 5, 1);
			// Back
			cube->triangle(5, 6, 7);
			cube->triangle(7, 4, 5);
			// Left
			cube->triangle(4, 7, 3);
			cube->triangle(3, 0, 4);
			// Top
			cube->triangle(4, 0, 1);
			cube->triangle(1, 5, 4);
			// Bottom
			cube->triangle(3, 7, 6);
			cube->triangle(6, 2, 3);

			cube->end();

			// 4. Attach it to a SceneNode to see it
			Ogre::SceneNode* cubeNode = scnMgr->getRootSceneNode()->createChildSceneNode();
			cubeNode->attachObject(cube);
		}
		catch (Ogre::Exception& e) {
			std::cerr << "...................................................\n";
			std::cerr << "Ogre Exception: " << e.getFullDescription() << std::endl;
		}

		printf("helllo\n");
	}

	return SDL_APP_CONTINUE;
}

//Update function
SDL_AppResult SDL_AppIterate(void *appstate) 
{
	try {
		if (root) {
			root->renderOneFrame();
		}
	}
	catch (Ogre::Exception& e) {
		std::cerr << "...................................................\n";
		std::cerr << "Ogre Exception: " << e.getFullDescription() << std::endl;
	}

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