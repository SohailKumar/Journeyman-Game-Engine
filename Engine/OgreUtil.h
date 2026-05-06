#include "PhysicsObjectFactory.h"
namespace OgreUtil {
	static int cubeCount = 0;
	static int sphereCount = 0;
	static int ogreCount = 0;
	void CreateCube(Ogre::SceneManager* scnMgr, PhysicsObjectFactory* factory);
	void CreateSphere(Ogre::SceneManager* scnMgr, PhysicsObjectFactory* factory);
	void CreateOgre(Ogre::SceneManager* scnMgr, PhysicsObjectFactory* factory);
}