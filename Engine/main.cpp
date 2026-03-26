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

	try {
#if (defined(DEBUG) | defined(_DEBUG))
		root = new Ogre::Root("../bin/Debug/plugins.cfg");
#else
		root = new Ogre::Root("../bin/Release/plugins.cfg");
#endif
	}
	catch (Ogre::Exception& e) {
		std::cerr << "...................................................\n";
		std::cerr << "Ogre Exception: " << e.getFullDescription() << std::endl;
	}

	
	if (!root) {
		return SDL_APP_FAILURE;
	}

	try {
		auto* renderSystem = root->getRenderSystemByName("OpenGL Rendering Subsystem");
		root->setRenderSystem(renderSystem);
		root->initialise(false);


		Ogre::NameValuePairList params;
		params["externalWindowHandle"] = windowHandleStr;
		Ogre::RenderWindow* ogreWin = root->createRenderWindow("Render Test Window", windowWidth, windowHeight, false, &params);

		scnMgr = root->createSceneManager();

		//if (Ogre::RTShader::ShaderGenerator::initialize()) {
		//	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

		//	// 2. Add the SceneManager to the generator
		//	shadergen->addSceneManager(scnMgr);

		//	// 3. Make sure the RTSS knows where to find its own core shaders!
		//	// These are usually in the /Media/RTShaderLib folder of the Ogre SDK
		//	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("path/to/RTShaderLib", "FileSystem");
		//}
		//else {
		//	throw std::exception("bad");
		//}

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
		
		//if (Ogre::RTShader::ShaderGenerator::initialize())
		//{
		//	// Register the scene manager.
		//	Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(scnMgr);
		//}

		//Ogre::RTShader::ShaderGenerator* mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		//mShaderGenerator->addSceneManager(scnMgr);
		//// Apply the shader generated based techniques.
		////vp->setMaterialScheme(Ogre::MSN_SHADERGEN);
		//// Set the material scheme to the RTSS default
		//vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		Ogre::ConfigFile cf;
		// Use your full path here as you already have it
		cf.load("C:\\Users\\sol\\source\\repos\\Journeyman-Game-Engine\\bin\\Debug\\resources.cfg");

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
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
			//{
			//	Ogre::Plane plane(Ogre::Vector3(0, 1, 0), -10);

			//	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 20, Ogre::Vector3::UNIT_Z);

			//	Ogre::Entity* ent = scnMgr->createEntity("Name Of Plane", "plane");

			//	scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
			//	ent->setMaterialName("PlainWhiteX");
			//}
			{
				Ogre::MeshManager::getSingleton().createPlane(
					"MyPlane",
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					Ogre::Plane(Ogre::Vector3(0,1,0), -10), // Normal pointing up
					20, 20, 1, 1,                        // Width, Height, Segments
					true, 1, 5, 5,                         // Normals, TexCoords, U tile, V tile
					Ogre::Vector3::UNIT_Z                  // Up vector
				);

				// Create the material
				Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(
					"PlaneMaterial",
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
				);

				// Set the diffuse color (e.g., a nice Blue)
				Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
				pass->setDiffuse(0.0, 0.5, 1.0, 1.0);
				pass->setAmbient(0.1, 0.1, 0.1);

				// CRITICAL: Tell the material to use the RTSS generated technique
				//mat->getTechnique(0)->setSchemeName(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

				Ogre::Entity* planeEnt = scnMgr->createEntity("PlaneEntity", "MyPlane");
				planeEnt->setMaterialName("PlaneMaterial");

				Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
				node->attachObject(planeEnt);
				node->setPosition(Ogre::Vector3(0, 0, -30.0f));
			}
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