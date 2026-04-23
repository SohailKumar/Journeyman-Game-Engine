#include "PhysicsObject.h"
#include <glm/glm.hpp>

// SAT implementation taken from Chris Ericson's textbook "Real Time Collision Detection"

PhysicsObject::PhysicsObject(const Ogre::String & name) : Ogre::MovableObject(name)
{

}

PhysicsObject::~PhysicsObject() {}

const Ogre::String& PhysicsObject::getMovableType(void) const
{
    return PHYSICS_OBJECT_TYPE;
}

const Ogre::AxisAlignedBox& PhysicsObject::getBoundingBox(void) const
{
    return mEntity->getBoundingBox();
}

Ogre::Real PhysicsObject::getBoundingRadius(void) const
{
    return mEntity->getBoundingRadius();
}

// TODO: figure out what this method is supposed to do
void PhysicsObject::_updateRenderQueue(Ogre::RenderQueue* queue)
{

}

// TODO: figure out what this method is supposed to do
void PhysicsObject::visitRenderables(Ogre::Renderable::Visitor* visitor,
    bool debugRenderables)
{

}

void PhysicsObject::Move()
{
    getParentNode()->yaw(Ogre::Radian(Ogre::Math::DegreesToRadians(0.005))); // TEMP
}

bool PhysicsObject::CheckCollision(PhysicsObject* other)
{
    // A simple AABB check to see if they are even close to each other
    if (mEntity->getWorldBoundingBox().intersects(other->mEntity->getWorldBoundingBox()))
    {
		if (SAT(other))
		{
			mEntity->setMaterialName("Highlight"); // TEMP
			return true;
		}
    }

    mEntity->setMaterialName("Plain"); // TEMP
    return false;
}

// Calculates precision collisions, and should be used in tandem with another cheaper detection to prevent 
bool PhysicsObject::SAT(PhysicsObject* const a_pOther)
{
	Ogre::Real ra, rb;
	Ogre::Matrix4 R, AbsR;

	// TODO: move the u "localAxis" calculations into PhysicsObject, calculate during movement
	Ogre::Quaternion qa = getParentSceneNode()->_getDerivedOrientation();
	Ogre::Quaternion qb = a_pOther->getParentSceneNode()->_getDerivedOrientation();

	Ogre::Vector3 ua[3] = {
		qa * Ogre::Vector3::UNIT_X,
		qa * Ogre::Vector3::UNIT_Y,
		qa * Ogre::Vector3::UNIT_Z
	};

	Ogre::Vector3 ub[3] = {
		qb * Ogre::Vector3::UNIT_X,
		qb * Ogre::Vector3::UNIT_Y,
		qb * Ogre::Vector3::UNIT_Z
	};

	// Pos. halfwidth extents of OBB along each axis -> Vector e
	Ogre::Vector3 ea = HalfSize;
	Ogre::Vector3 eb = a_pOther->HalfSize;

	// Compute rotation matrix expressing a_pOther in this rb's coordinate frame
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			R[i][j] = ua[i].dotProduct(ub[j]);
		}
	}

	// Compute the translation vector t, bring it into this rb's coordinate frame
	Ogre::Vector3 t = a_pOther->GetWorldPosition() - GetWorldPosition();
	t = Ogre::Vector3(t.dotProduct(ua[0]), t.dotProduct(ua[1]), t.dotProduct(ua[2]));


	// Adding in epsilon to counteract two parallel edges and cross product is near null
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AbsR[i][j] = glm::abs(R[i][j]) + DBL_EPSILON;
		}
	}

	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++)
	{
		ra = ea[i];
		rb = eb[0] * AbsR[i][0] + eb[1] * AbsR[i][1] + eb[2] * AbsR[i][2];

		if (glm::abs(t[i]) > ra + rb) return false;
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++)
	{
		rb = eb[i];
		ra = ea[0] * AbsR[0][i] + ea[1] * AbsR[1][i] + ea[2] * AbsR[2][i];

		if (glm::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return false;
	}

	// Test axis L = A0 x B0
	ra = ea[1] * AbsR[2][0] + ea[2] * AbsR[1][0];
	rb = eb[1] * AbsR[0][2] + eb[2] * AbsR[0][1];
	if (glm::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

	// Test axis L = A0 x B1
	ra = ea[1] * AbsR[2][1] + ea[2] * AbsR[1][1];
	rb = eb[0] * AbsR[0][2] + eb[2] * AbsR[0][0];
	if (glm::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

	// Test axis L = A0 x B2
	ra = ea[1] * AbsR[2][2] + ea[2] * AbsR[1][2];
	rb = eb[0] * AbsR[0][1] + eb[1] * AbsR[0][0];
	if (glm::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

	// Test axis L = A1 x B0
	ra = ea[0] * AbsR[2][0] + ea[2] * AbsR[0][0];
	rb = eb[1] * AbsR[1][2] + eb[2] * AbsR[1][1];
	if (glm::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

	// Test axis L = A1 x B1
	ra = ea[0] * AbsR[2][1] + ea[2] * AbsR[0][1];
	rb = eb[0] * AbsR[1][2] + eb[2] * AbsR[1][0];
	if (glm::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

	// Test axis L = A1 x B2
	ra = ea[0] * AbsR[2][2] + ea[2] * AbsR[0][2];
	rb = eb[0] * AbsR[1][1] + eb[1] * AbsR[1][0];
	if (glm::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

	// Test axis L = A2 x B0
	ra = ea[0] * AbsR[1][0] + ea[1] * AbsR[0][0];
	rb = eb[1] * AbsR[2][2] + eb[2] * AbsR[2][1];
	if (glm::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

	// Test axis L = A2 x B1
	ra = ea[0] * AbsR[1][1] + ea[1] * AbsR[0][1];
	rb = eb[0] * AbsR[2][2] + eb[2] * AbsR[2][0];
	if (glm::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

	// Test axis L = A2 x B2
	ra = ea[0] * AbsR[1][2] + ea[1] * AbsR[0][2];
	rb = eb[0] * AbsR[2][1] + eb[1] * AbsR[2][0];
	if (glm::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

	// Could not find a seperating plane between them
	return true; 
}


