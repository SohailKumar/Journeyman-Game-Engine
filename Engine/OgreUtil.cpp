#include "Ogre.h"
#include "OgreUtil.h"
#include "PhysicsObject.h"
#include "Helper.h"

void OgreUtil::CreateCube(Ogre::SceneManager* scnMgr, PhysicsObjectFactory* factory) {
	factory->createObject(scnMgr, Ogre::Vector3(30, 10, 0), false, Ogre::Vector3(0.1f), "cube.mesh");
}

void OgreUtil::CreateSphere(Ogre::SceneManager* scnMgr, PhysicsObjectFactory* factory) {
	factory->createObject(scnMgr, Ogre::Vector3(20, 10, 0), false, Ogre::Vector3(0.1f), "sphere.mesh");
}

void OgreUtil::CreateOgre(Ogre::SceneManager* scnMgr, PhysicsObjectFactory* factory) {
	factory->createObject(scnMgr, Ogre::Vector3(20, 10, 0), false, Ogre::Vector3(0.3f), "ogrehead.mesh");
}