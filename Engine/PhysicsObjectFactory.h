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
        Ogre::Vector3 scale = Ogre::Vector3(1.0f));

    void Update(float deltaTime);

    void ClearObjects()
    {
        PhysicsObjects.clear();
    }

private:
    int index;

    std::list<PhysicsObject*> PhysicsObjects;
};