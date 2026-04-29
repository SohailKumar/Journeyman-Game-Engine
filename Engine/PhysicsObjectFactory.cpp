#include "PhysicsObjectFactory.h"

PhysicsObjectFactory::PhysicsObjectFactory()
{
    index = 0;
}

PhysicsObjectFactory::~PhysicsObjectFactory()
{
}

void PhysicsObjectFactory::createObject(Ogre::SceneManager* scnMgr, Ogre::Vector3 position, bool isStatic, Ogre::Vector3 scale)
{
    const Ogre::String nameObj = "Object_%f" + std::to_string(index);
    const Ogre::String nameNode = "Node_%f" + std::to_string(index);
    const Ogre::String namePhys = "PhysObj_%f" + std::to_string(index);

    Ogre::Entity* ent = scnMgr->createEntity(nameObj, "cube.mesh");
    Ogre::SceneNode* node = scnMgr->createSceneNode(nameNode);
    PhysicsObject* physObj = static_cast<PhysicsObject*>(
        scnMgr->createMovableObject(namePhys, "PhysicsObject"));
    scnMgr->getRootSceneNode()->addChild(node);
    node->attachObject(ent);
    node->attachObject(physObj);

    node->setScale(scale);
    physObj->Initialize(ent, isStatic);
    node->setPosition(position);
    ent->setMaterialName("Plain"); // Plain or Highlight

    index += 1;

    PhysicsObjects.push_front(physObj);
}

void PhysicsObjectFactory::Update(float deltaTime)
{

    // We update the transform data of each object in the scene
    for (std::list<PhysicsObject*>::iterator obj = PhysicsObjects.begin(); obj != PhysicsObjects.end(); ++obj) {
        (*obj)->Update(deltaTime);
    }

    // We detect if each object is colliding with every other object in the scene
    for (std::list<PhysicsObject*>::iterator i = PhysicsObjects.begin(); i != PhysicsObjects.end(); ++i) {
        for (std::list<PhysicsObject*>::iterator j = PhysicsObjects.begin(); j != PhysicsObjects.end(); ++j) {
            if (i != j)
            {
                (*i)->CheckCollision((*j));
            }
        }
    }
}
