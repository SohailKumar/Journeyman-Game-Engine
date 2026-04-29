#pragma once
#include <OgreNode.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreMovableObject.h>

class PhysicsObject : public Ogre::MovableObject {

public:

    /// Forcemode is shamelessly taken from Unity, allowing forces to affect the object's velocity differently
    enum ForceMode {
        Force,          // A constant force applied using the object's mass
        Acceleration,   // A constant force applied ignoring the object's mass
        Impulse,        // An instant force applied using the object's mass
        VelocityChange  // An instant force applied ignoring the object's mass
    };

    /// <summary>
    /// 
    /// </summary>
    const float AIR_DRAG = 0.95f;
    
    const Ogre::String PHYSICS_OBJECT_TYPE = "PhysicsObject";
    const Ogre::Vector3 GRAVITY_CONSTANT = Ogre::Vector3(0.0f, -0.02f, 0.0f);

    Ogre::Vector3 HalfSize;

    PhysicsObject(const Ogre::String& name);
    virtual ~PhysicsObject();

    const Ogre::String& getMovableType(void) const override;
    const Ogre::AxisAlignedBox& getBoundingBox(void) const override;
    Ogre::Real getBoundingRadius(void) const override;
    void _updateRenderQueue(Ogre::RenderQueue* queue) override;
    void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false) override;

    /// <summary>
    /// Applies a physical force to the object this frame, which affects the object's acceleration
    /// </summary>
    /// <param name="force"></param>
    void ApplyForce(Ogre::Vector3 force, ForceMode mode = Force);

    /// <summary>
    /// Moves the object in the scene based on it's velocity and acceleration
    /// </summary>
    void Update(float deltaTime);

    /// <summary>
    /// Checks for collisions with another object, and applying a force to itself based on that collision
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    bool CheckCollision(PhysicsObject* other);

    /// <summary>
    /// SetEntity is an easier way to deal with referencing the entity attached to the same node
    /// </summary>
    /// <param name="ent"></param>
    /// <param name="isStatic"></param>
    void Initialize(Ogre::Entity* ent, bool isStatic = false)
    { 
        IsStatic = isStatic;
        mEntity = ent; 
        HalfSize = ent->getBoundingBox().getHalfSize() * getParentSceneNode()->getScale();
    }

    Ogre::Vector3 GetWorldPosition() { return getParentSceneNode()->_getDerivedPosition(); }
    Ogre::Matrix4 GetWorldMatrix() { return getParentSceneNode()->_getFullTransform(); }

private:
    bool IsStatic;
    float Mass;

    Ogre::Vector3 Velocity;
    Ogre::Vector3 Constant;
    Ogre::Vector3 Signal;

    Ogre::Entity *mEntity;
    bool SAT(PhysicsObject* const a_pOther);
};