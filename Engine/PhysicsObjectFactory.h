#pragma once
#include "PhysicsObject.h"

// NOTE: it is reccommended to use a factory to create custom MovableObjects instead of...
// ... initialing them in main. This was taking from one example

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
};