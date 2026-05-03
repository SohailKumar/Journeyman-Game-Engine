#include "Ogre.h"
#include "OgreUtil.h"
#include "PhysicsObject.h"
#include "Helper.h"

void OgreUtil::CreateCube(Ogre::SceneManager* scnMgr) {
	Ogre::SceneNode* node = scnMgr->createSceneNode("Node_Cube_" + std::to_string(cubeCount));

	Ogre::Entity* ent = scnMgr->createEntity("Cube_" + std::to_string(cubeCount), "cube.mesh");
	ent->setQueryFlags(ENTITY);
	PhysicsObject* physObj = static_cast<PhysicsObject*>(
		scnMgr->createMovableObject("PhysicsObject_Cube_" + std::to_string(cubeCount), "PhysicsObject"));
	physObj->setQueryFlags(PHYSICS_OBJECT);

	scnMgr->getRootSceneNode()->addChild(node);
	node->attachObject(ent);
	node->attachObject(physObj);

	node->setScale(0.3, 0.3, 0.3);
	physObj->setEntity(ent);
	node->setPosition(0, 0, -5);
	ent->setMaterialName("BasicShadedPlastic");

	cubeCount++;
}

void OgreUtil::CreateSphere(Ogre::SceneManager* scnMgr) {
	Ogre::SceneNode* node = scnMgr->createSceneNode("Node_Sphere_" + std::to_string(sphereCount));

	Ogre::Entity* ent = scnMgr->createEntity("Sphere_" + std::to_string(sphereCount), "sphere.mesh");
	ent->setQueryFlags(ENTITY);
	PhysicsObject* physObj = static_cast<PhysicsObject*>(
		scnMgr->createMovableObject("PhysicsObject_Sphere_" + std::to_string(sphereCount), "PhysicsObject"));
	physObj->setQueryFlags(PHYSICS_OBJECT);

	scnMgr->getRootSceneNode()->addChild(node);
	node->attachObject(ent);
	node->attachObject(physObj);

	node->setScale(0.1, 0.1, 0.1);
	physObj->setEntity(ent);
	node->setPosition(0, 2, -10);
	ent->setMaterialName("BasicShadedPlastic");

	sphereCount++;
}

void OgreUtil::CreateOgre(Ogre::SceneManager* scnMgr) {
	Ogre::SceneNode* node = scnMgr->createSceneNode("Node_Ogre_" + std::to_string(ogreCount));

	Ogre::Entity* ent = scnMgr->createEntity("Ogre_" + std::to_string(ogreCount), "ogrehead.mesh");
	ent->setQueryFlags(ENTITY);
	PhysicsObject* physObj = static_cast<PhysicsObject*>(
		scnMgr->createMovableObject("PhysicsObject_Ogre_" + std::to_string(ogreCount), "PhysicsObject"));
	physObj->setQueryFlags(PHYSICS_OBJECT);

	scnMgr->getRootSceneNode()->addChild(node);
	node->attachObject(ent);
	node->attachObject(physObj);

	node->setScale(0.1, 0.1, 0.1);
	physObj->setEntity(ent);
	node->setPosition(0, 0, -5);
	//ent->setMaterialName("BasicShading");

	ogreCount++;
}

//int main(int argc, char* argv[])
//{
//    // NOTE: OGRE3D comes prepackaged with a utility class called OgreBites, which provides a ton of wrapper functions for SDL2
//    // NOTE: This 
//
//    OgreBites::ApplicationContext ctx("OgreTutorialApp");
//    ctx.initApp();
//}
//    // get a pointer to the already created root
//    Ogre::Root* root = ctx.getRoot();
//    Ogre::SceneManager* scnMgr = root->createSceneManager();
//
//    // register our scene with the RTSS
//    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
//    shadergen->addSceneManager(scnMgr);
//
//    // without light we would just get a black screen    
//    Ogre::Light* light = scnMgr->createLight("MainLight");
//    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
//    lightNode->setPosition(0, 10, 15);
//    lightNode->attachObject(light);
//
//    // also need to tell where we are
//    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
//    camNode->setPosition(0, 0, 15);
//    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
//
//    // create the camera
//    Ogre::Camera* cam = scnMgr->createCamera("myCam");
//    cam->setNearClipDistance(5); // specific to this sample
//    cam->setAutoAspectRatio(false);
//    camNode->attachObject(cam);
//
//    // and tell it to render into the main window
//    ctx.getRenderWindow()->addViewport(cam);
//
//    // finally something to render
//    // 1. Create the ManualObject
//    Ogre::ManualObject* cube = scnMgr->createManualObject("UserCube");
//
//    // 2. Start defining the mesh using a material
//    cube->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
//
//    // Define the 8 vertices of a cube
//    // Front face
//    cube->position(-0.5, 0.5, 0.5); // 0
//    cube->position(0.5, 0.5, 0.5); // 1
//    cube->position(0.5, -0.5, 0.5); // 2
//    cube->position(-0.5, -0.5, 0.5); // 3
//
//    // Back face
//    cube->position(-0.5, 0.5, -0.5); // 4
//    cube->position(0.5, 0.5, -0.5); // 5
//    cube->position(0.5, -0.5, -0.5); // 6
//    cube->position(-0.5, -0.5, -0.5); // 7
//
//    // 3. Define the triangles (Indices)
//    // Front
//    cube->triangle(0, 3, 2);
//    cube->triangle(2, 1, 0);
//    // Right
//    cube->triangle(1, 2, 6);
//    cube->triangle(6, 5, 1);
//    // Back
//    cube->triangle(5, 6, 7);
//    cube->triangle(7, 4, 5);
//    // Left
//    cube->triangle(4, 7, 3);
//    cube->triangle(3, 0, 4);
//    // Top
//    cube->triangle(4, 0, 1);
//    cube->triangle(1, 5, 4);
//    // Bottom
//    cube->triangle(3, 7, 6);
//    cube->triangle(6, 2, 3);
//
//    cube->end();
//
//    // 4. Attach it to a SceneNode to see it
//    Ogre::SceneNode* cubeNode = scnMgr->getRootSceneNode()->createChildSceneNode();
//    cubeNode->attachObject(cube);
//
//    // register for input events
//    KeyHandler keyHandler;
//    ctx.addInputListener(&keyHandler);
//
//    ctx.getRoot()->startRendering();
//    ctx.closeApp();
//
//    return 0;
//}
