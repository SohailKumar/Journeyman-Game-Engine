#pragma once
#include "PhysicsObject.h"
#include "OgreRoot.h"

class PhysicsObjectFactory : public Ogre::MovableObjectFactory {
public:
    PhysicsObjectFactory();
    ~PhysicsObjectFactory();

    const Ogre::String FACTORY_TYPE_NAME = "PhysicsObject";

    const Ogre::String& getType(void) const override {
        return FACTORY_TYPE_NAME;
    }

    Ogre::MovableObject* createInstanceImpl(
        const Ogre::String& name,
        const Ogre::NameValuePairList* params = 0) override
    {
        return new PhysicsObject(name);
    }

    void createObject(Ogre::SceneManager* scnMgr, Ogre::Vector3 position, bool isStatic = false,
        Ogre::Vector3 scale = Ogre::Vector3(1.0f), const Ogre::String& mesh = "cube.mesh");

    void Update(float deltaTime);

    void ClearObjects(Ogre::SceneManager* scnMgr)
    {
        for(PhysicsObject *obj : PhysicsObjects)
        {
            Ogre::SceneNode* parentNode = obj->getParentSceneNode();

            if (parentNode) {
                // Detach the object from the node
                parentNode->detachObject(obj);
            }

            scnMgr->destroyMovableObject(obj);

            if (parentNode) {
                scnMgr->destroySceneNode(parentNode);
            }
		}
        PhysicsObjects.clear();

    }

private:
    int index;

    std::list<PhysicsObject*> PhysicsObjects;
};