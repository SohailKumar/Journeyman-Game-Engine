#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h";
#include "SDL3/SDL_main.h";
#include <cstdio>
#include <wtypes.h>
#include <OgreRenderSystem.h>
#include <Ogre.h>
#include "OgreRoot.h"

//overlay
#include <OgreImGuiOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include "imgui_impl_sdl3.h"

#include <string>
#include <sstream>
#include <iostream>
//#include <OgreRTShaderSystem.h>

#include "PhysicsObjectFactory.h"
#include "PhysicsObject.h"

static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;
Ogre::Root* root;
Ogre::SceneManager* scnMgr;
PhysicsObjectFactory* factory;

std::list<PhysicsObject*> PhysicsObjects;

int SetupOgre(Ogre::String windowHandleStr, unsigned int windowWidth, unsigned int windowHeight) {
	// Set RenderSystem
	auto* renderSystem = root->getRenderSystemByName("OpenGL Rendering Subsystem");
	root->setRenderSystem(renderSystem);
	root->initialise(false);

	// Transfer Window from SDL TO Ogre
	Ogre::NameValuePairList params;
	params["externalWindowHandle"] = windowHandleStr;
	Ogre::RenderWindow* ogreWin = root->createRenderWindow("Render Test Window", windowWidth, windowHeight, false, &params);
	scnMgr = root->createSceneManager();

	factory = new PhysicsObjectFactory();
	Ogre::Root::getSingleton().addMovableObjectFactory(factory);

	// Use OGRE_SDK from environment variables 
	char* buffer = nullptr;
	size_t size = 0;
	if (_dupenv_s(&buffer, &size, "OGRE_SDK") == 0 && buffer != nullptr) {
		//Ogre::ResourceGroupManager::getSingleton().addResourceLocation((std::string(buffer) +"\\Media"), "FileSystem", "General", false);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media", "FileSystem", "Internal");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation((std::string(buffer) + "\\Media\\Main"), "FileSystem", "Internal");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation((std::string(buffer) + "\\Media\\RTShaderLib"), "FileSystem", "Internal", false);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation((std::string(buffer) + "\\Samples\\Media\\models"), "FileSystem", "General", false, false);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation((std::string(buffer) + "\\Samples\\Media\\materials\\scripts"), "FileSystem", "General", false, false);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation((std::string(buffer) + "\\Samples\\Media\\materials\\textures"), "FileSystem", "General", false, false);
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		free(buffer);
	}
	else {
		return SDL_APP_FAILURE;
	}

	// Setup Standard Scene Objects
	scnMgr->setAmbientLight(Ogre::ColourValue::ColourValue(1.0, 1.0, 1.0));

	Ogre::Light* light = scnMgr->createLight("MainLight");
	Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setPosition(0, 10, 15);

	Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(0, 0, 100);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	Ogre::Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(false);
	camNode->attachObject(cam);

	Ogre::Viewport* vp = ogreWin->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue::ColourValue(0.2, 0.2, 0.2));
}

// Start function
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

	// Create SDL Window
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

	// Get Window Handle for Ogre
	void* hwnd_ptr = (SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
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
		Ogre::OverlaySystem* mOverlaySystem = new Ogre::OverlaySystem();

		SetupOgre(windowHandleStr, windowWidth, windowHeight);

		//scnMgr->addRenderQueueListener(mOverlaySystem);
		//Ogre::ImGuiOverlay* imguiOverlay = new Ogre::ImGuiOverlay();
		//Ogre::OverlayManager* overlayManager = Ogre::OverlayManager::getSingletonPtr();
		//if (overlayManager)
		//{ overlayManager->addOverlay(imguiOverlay); }

		//ImGui::CreateContext();
		//ImGui_ImplSDL3_InitForOther(window);

		//imguiOverlay->show();

		// CREATE THE SCENE
		Ogre::Entity* ent_1 = scnMgr->createEntity("Cube_1", "cube.mesh");
		Ogre::SceneNode* node_1 = scnMgr->createSceneNode("Node_1");
		PhysicsObject* physObj_1 = static_cast<PhysicsObject*>(
			scnMgr->createMovableObject("PhysicsObject1", "PhysicsObject"));
		scnMgr->getRootSceneNode()->addChild(node_1);
		node_1->attachObject(ent_1);
		node_1->attachObject(physObj_1);

		node_1->pitch(Ogre::Radian(Ogre::Math::DegreesToRadians(90))); // TEMP

		node_1->setScale(0.4, 0.4, 0.4);
		physObj_1->setEntity(ent_1);
		node_1->setPosition(-25, 0, 0);
		ent_1->setMaterialName("Plain"); // Plain or Highlight
		PhysicsObjects.push_front(physObj_1);

		Ogre::Entity* ent_2 = scnMgr->createEntity("Cube_2", "cube.mesh");
		Ogre::SceneNode* node_2 = scnMgr->createSceneNode("Node_2");
		PhysicsObject* physObj_2 = static_cast<PhysicsObject*>(
			scnMgr->createMovableObject("PhysicsObject2", "PhysicsObject"));
		scnMgr->getRootSceneNode()->addChild(node_2);
		node_2->attachObject(ent_2);
		node_2->attachObject(physObj_2);

		node_2->pitch(Ogre::Radian(Ogre::Math::DegreesToRadians(90))); // TEMP

		node_2->setScale(0.4, 0.4, 0.4); 
		physObj_2->setEntity(ent_2);
		node_2->setPosition(25, 0, 0);
		ent_2->setMaterialName("Plain"); // Plain or Highlight
		PhysicsObjects.push_front(physObj_2);
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
			//	Ogre::ManualObject* man = scnMgr->createManualObject("test");
			//	man->begin("Examples/BeachStonesA", Ogre::RenderOperation::OT_TRIANGLE_LIST);

			//	man->position(-20, 20, 20);
			//	man->normal(0, 0, 1);
			//	man->textureCoord(0, 0);

			//	man->position(-20, -20, 20);
			//	man->normal(0, 0, 1);
			//	man->textureCoord(0, 1);

			//	man->position(20, -20, 20);
			//	man->normal(0, 0, 1);
			//	man->textureCoord(1, 1);

			//	man->position(20, 20, 20);
			//	man->normal(0, 0, 1);
			//	man->textureCoord(1, 0);

			//	man->quad(0, 1, 2, 3);

			//	man->end();
			//	scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(man);
			//}

			//{
			//	Ogre::ManualObject* manual = scnMgr->createManualObject("Quad");
			//	manual->begin("Examples/OgreLogo", Ogre::RenderOperation::OT_TRIANGLE_LIST);

			//	manual->position(5.0, 0.0, 0.0);
			//	manual->textureCoord(0, 1);
			//	manual->position(-5.0, 10.0, 0.0);
			//	manual->textureCoord(1, 0);
			//	manual->position(-5.0, 0.0, 0.0);
			//	manual->textureCoord(1, 1);
			//	manual->position(5.0, 10.0, 0.0);
			//	manual->textureCoord(0, 0);

			//	manual->index(0);
			//	manual->index(1);
			//	manual->index(2);
			//	manual->index(0);
			//	manual->index(3);
			//	manual->index(1);

			//	manual->end();
			//	manual->convertToMesh("Quad");

			//	Ogre::Entity* ent = scnMgr->createEntity("Quad");
			//	Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode("Node1");
			//	node->attachObject(ent);

			//}

			//{
			//	Ogre::Plane plane(Ogre::Vector3(0, 1, 0), -10);

			//	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, false, 1, 5, 20, Ogre::Vector3::UNIT_Z);

			//	Ogre::Entity* ent = scnMgr->createEntity("Name Of Plane", "plane");

			//	scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
			//	ent->setMaterialName("PlainWhiteX");
			//}

			//{
			//	Ogre::MeshManager::getSingleton().createPlane(
			//		"MyPlane",
			//		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			//		Ogre::Plane(Ogre::Vector3(0,1,0), -10), // Normal pointing up
			//		20, 20, 1, 1,                        // Width, Height, Segments
			//		false, 1, 5, 5,                         // Normals, TexCoords, U tile, V tile
			//		Ogre::Vector3::UNIT_Z                  // Up vector
			//	);

			//	// Create the material
			//	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(
			//		"PlaneMaterial",
			//		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
			//	);

			//	// Set the diffuse color (e.g., a nice Blue)
			//	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
			//	pass->setDiffuse(0.0, 0.5, 1.0, 1.0);
			//	pass->setAmbient(0.1, 0.1, 0.1);

			//	Ogre::Entity* planeEnt = scnMgr->createEntity("PlaneEntity", "MyPlane");
			//	planeEnt->setMaterialName("PlaneMaterial");

			//	Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
			//	node->attachObject(planeEnt);
			//	node->setPosition(Ogre::Vector3(0, 0, -30.0f));
			//}
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

			// 
			for (std::list<PhysicsObject*>::iterator obj = PhysicsObjects.begin(); obj != PhysicsObjects.end(); ++obj) {
				(*obj)->Move();
			}

			// Temporary: We ask if each object is colliding with every other object in the scene
			for (std::list<PhysicsObject*>::iterator i = PhysicsObjects.begin(); i != PhysicsObjects.end(); ++i) {
				for (std::list<PhysicsObject*>::iterator j = PhysicsObjects.begin(); j != PhysicsObjects.end(); ++j) {
					if (i != j)
					{
						(*i)->CheckCollision((*j));
					}
				}
			}
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