#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h";
#include "SDL3/SDL_main.h";
#include <cstdio>
#include <wtypes.h>
#include <OgreRenderSystem.h>
#include <Ogre.h>
#include "OgreRoot.h"
#include <chrono>
#include "OgreUtil.h"
#include <filesystem>

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

#include "Helper.h"

#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720

static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;
Ogre::Root* root;
Ogre::SceneManager* scnMgr;
Ogre::OverlaySystem* mOverlaySystem;
Ogre::ImGuiOverlay* imguiOverlay;
Ogre::RaySceneQuery* mRaySceneQuery;
PhysicsObjectFactory* factory;
std::unique_ptr<SelectedObject> selectedObject;

std::chrono::steady_clock::time_point TimePoint;
std::list<PhysicsObject*> PhysicsObjects;

namespace fs = std::filesystem;

int SetupOgre(Ogre::String windowHandleStr, unsigned int windowWidth, unsigned int windowHeight) {
	mOverlaySystem = new Ogre::OverlaySystem();

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
		auto path = fs::current_path() / "Media";
		std::cout << "asdfasdfawefASDFAWEFASDFAS \n\n\n\n\nCurrent Path: " << path.string() << std::endl;
		std::cout << (std::string(buffer) + "\\Samples\\Media\\materials\\scripts") << std::endl;
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path.string(), "FileSystem", "General");
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

	// Setup ImGui Overlay
	scnMgr->addRenderQueueListener(mOverlaySystem);
	imguiOverlay = new Ogre::ImGuiOverlay();
	Ogre::OverlayManager* overlayManager = Ogre::OverlayManager::getSingletonPtr();
	float vpScale = 1.0f;
	if (overlayManager)
	{
		overlayManager->addOverlay(imguiOverlay);
		vpScale = overlayManager->getPixelRatio();
	}

	ImGui::CreateContext();
	ImGui::GetIO().FontGlobalScale = std::round(vpScale); // default font does not work with fractional scaling
	imguiOverlay->setZOrder(300);
	imguiOverlay->show();

	ImGui_ImplSDL3_InitForOther(window);

	// Stuff to select an object
	mRaySceneQuery = scnMgr->createRayQuery(Ogre::Ray());
	mRaySceneQuery->setQueryTypeMask(ENTITY);
	mRaySceneQuery->setSortByDistance(true);
	selectedObject = std::unique_ptr<SelectedObject>(new SelectedObject());

	// Setup Standard Scene Objects
	scnMgr->setAmbientLight(Ogre::ColourValue::ColourValue(1.0, 1.0, 1.0));

	Ogre::Light* light = scnMgr->createLight("MainLight");
	Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setPosition(0, 5, -10);
	light->setCastShadows(true);
	light->setDiffuseColour(Ogre::ColourValue(1.0, 1.0, 1.0)); // Bright white
	light->setSpecularColour(Ogre::ColourValue(0.5, 0.5, 0.5)); // Shine color

	Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(0, 0, 200);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	Ogre::Camera* cam = scnMgr->createCamera("myCam");
	cam->setAspectRatio(Ogre::Real(windowWidth) / Ogre::Real(windowHeight));
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(false);
	camNode->attachObject(cam);

	Ogre::Viewport* vp = ogreWin->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue::ColourValue(0.1, 0.1, 0.1));
}

void SetupUI() {
	namespace IG = ImGui;

	ImGuiViewport* viewport = IG::GetMainViewport();
	float panelWidth = 200.0f;
	ImGui::SetNextWindowPos(ImVec2(viewport->Size.x, 0),
		ImGuiCond_Always,
		ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(panelWidth, viewport->Size.y));
	ImGui::Begin("Details", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if (selectedObject->selected) {
		std::string selectedText = "Selected Object: \n\t" + selectedObject->name;
		ImGui::TextWrapped("%s", selectedText.c_str());
		ImGui::TextWrapped("Position: \n\tx=%.2f\n\ty=%.2f\n\tz=%.2f", selectedObject->position[0], selectedObject->position[1], selectedObject->position[2]);
	}
	ImGui::End();

	if (IG::BeginMainMenuBar())
	{
		if (IG::BeginMenu("New")) {
			if (IG::MenuItem("Cube")) {
				OgreUtil::CreateCube(scnMgr, factory);
			}
			if (IG::MenuItem("Sphere")) {
				OgreUtil::CreateSphere(scnMgr, factory);
			}
			IG::Separator();
			if (IG::MenuItem("Ogre")) {
				OgreUtil::CreateOgre(scnMgr, factory);
			}
			IG::EndMenu();
		}
		if (IG::MenuItem("ClearScene")) {
			if(selectedObject->selected)
				selectedObject->UnSelect(scnMgr);
			factory->ClearObjects(scnMgr);
		}
		IG::EndMainMenuBar();
	}

}

// Start function
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	// Create SDL Window
	SDL_SetAppMetadata("Renderer Test", "0.1", "journeymanengine");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	window = SDL_CreateWindow("Render Test", WINDOWWIDTH, WINDOWHEIGHT, SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	TimePoint = std::chrono::steady_clock::now();

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
		SetupOgre(windowHandleStr, WINDOWWIDTH, WINDOWHEIGHT);

		// CREATE THE SCENE

		factory->createObject(scnMgr, Ogre::Vector3(-30, 10, 0), false, Ogre::Vector3(0.1f));

//<<<<<<< HEAD
		factory->createObject(scnMgr, Ogre::Vector3(30, 10, 0), false, Ogre::Vector3(0.1f));
//=======
//		node_1->setScale(0.1, 0.1, 0.6);
//		physObj_1->setEntity(ent_1);
//		node_1->setPosition(-25, -30, -10);
//		ent_1->setMaterialName("Plain"); // Plain or Highlight
//		PhysicsObjects.push_front(physObj_1);
//>>>>>>> c367456 (creating objects works)

		factory->createObject(scnMgr, Ogre::Vector3(-20, 0, 0), false, Ogre::Vector3(0.1f));

		factory->createObject(scnMgr, Ogre::Vector3(20, 0, 0), false, Ogre::Vector3(0.1f));

		// My really terrible floor ceiling to deal with a pseudo collision detection
		for (int i = 0; i < 21; i++)
		{
			factory->createObject(scnMgr, Ogre::Vector3(-100 + i * 10 , -70, 0), true, Ogre::Vector3(0.1f, 0.1f, 0.1f));

			factory->createObject(scnMgr, Ogre::Vector3(-100 + i * 10, 70, 0), true, Ogre::Vector3(0.1f, 0.1f, 0.1f));
		}

		// My really terrible floor ceiling to deal with a pseudo collision detection
		for (int i = 0; i < 13; i++)
		{
			factory->createObject(scnMgr, Ogre::Vector3(-100, 60 - 10 * i, 0), true, Ogre::Vector3(0.1f, 0.1f, 0.1f));

			factory->createObject(scnMgr, Ogre::Vector3(100, 60 - 10 * i, 0), true, Ogre::Vector3(0.1f, 0.1f, 0.1f));
		}

//<<<<<<< HEAD
//=======
//		node_2->setScale(0.2, 0.2, 0.2); 
//		physObj_2->setEntity(ent_2);
//		node_2->setPosition(0, -30, -10);
//		ent_2->setMaterialName("Plain"); // Plain or Highlight
//		PhysicsObjects.push_front(physObj_2);
//>>>>>>> c367456 (creating objects works)
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
	ImGui_ImplSDL3_ProcessEvent(event);
	ImGuiIO& io = ImGui::GetIO();

	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && !io.WantCaptureMouse) {

		try {
			float mouseX;
			float mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			Ogre::Ray ray = scnMgr->getCamera("myCam")->getCameraToViewportRay(mouseX / WINDOWWIDTH, mouseY / WINDOWHEIGHT);
			mRaySceneQuery->setRay(ray);
			Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
			for (auto& item : result) {
				//std::cout << "Hit Distance: " << item.distance << std::endl;
				if (item.movable) {
					//std::cout << "\tHit: " << item.movable->getName() << std::endl;
					if (selectedObject->selected) {
						selectedObject->ResetLastObjectMaterial(scnMgr);
					}
					selectedObject->selected = true;
					selectedObject->name = item.movable->getName();
					selectedObject->position[0] = item.movable->getParentSceneNode()->getPosition().x;
					selectedObject->position[1] = item.movable->getParentSceneNode()->getPosition().y;
					selectedObject->position[2] = item.movable->getParentSceneNode()->getPosition().z;

					// 1. Get the node back from the physics body
					Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(item.movable->getParentNode());

					// 2. Get the entity attached to that node
					// Entities are usually the first object attached (index 0)
					Ogre::Entity* ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
					selectedObject->ogMaterial = ent->getSubEntity(0)->getMaterialName();
					ent->setMaterialName("Highlight");

					return SDL_APP_CONTINUE;
				}
			}
			if (selectedObject->selected)
				selectedObject->UnSelect(scnMgr);
		}
		catch (Ogre::Exception& e) {
			std::cerr << "...................................................\n";
			std::cerr << "Ogre Exception: " << e.getFullDescription() << std::endl;
		}
	}

	return SDL_APP_CONTINUE;
}

//Update function
SDL_AppResult SDL_AppIterate(void *appstate) 
{
	try {
		if (root) {

			if (selectedObject->selected) {
				auto movableObject = scnMgr->getMovableObject(selectedObject->name, "PhysicsObject");
				selectedObject->position[0] = movableObject->getParentSceneNode()->getPosition().x;
				selectedObject->position[1] = movableObject->getParentSceneNode()->getPosition().y;
				selectedObject->position[2] = movableObject->getParentSceneNode()->getPosition().z;
			}

			ImGui_ImplSDL3_NewFrame();
			Ogre::ImGuiOverlay::NewFrame();

			SetupUI();

			root->renderOneFrame();

			std::chrono::steady_clock::time_point TimeCurrent = std::chrono::steady_clock::now();
			float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(TimeCurrent - TimePoint).count() * 0.000001f;
			TimePoint = TimeCurrent;

			factory->Update(deltaTime);
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
	root->shutdown();
	delete factory;
	delete root;

	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}