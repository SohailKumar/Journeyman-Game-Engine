#pragma once
#include <OgreNode.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreMovableObject.h>

class PhysicsObject : public Ogre::MovableObject {

public:
    const Ogre::String PHYSICS_OBJECT_TYPE = "PhysicsObject";

    Ogre::Vector3 HalfSize;

    PhysicsObject(const Ogre::String& name);
    virtual ~PhysicsObject();

    const Ogre::String& getMovableType(void) const override;

    const Ogre::AxisAlignedBox& getBoundingBox(void) const override;
    Ogre::Real getBoundingRadius(void) const override;

    void _updateRenderQueue(Ogre::RenderQueue* queue) override;

    void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false) override;

    void Move();

    bool CheckCollision(PhysicsObject* other);

    void setEntity(Ogre::Entity* ent) 
    { 
        mEntity = ent; 
        HalfSize = ent->getBoundingBox().getHalfSize() * getParentSceneNode()->getScale();
    }

    Ogre::Vector3 GetWorldPosition() { return getParentSceneNode()->_getDerivedPosition(); }

    Ogre::Matrix4 GetWorldMatrix() { return getParentSceneNode()->_getFullTransform(); }

private:
    Ogre::Entity *mEntity;
    bool SAT(PhysicsObject* const a_pOther);
};