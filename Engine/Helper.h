#pragma once

enum QueryType {
	ENTITY = 1 << 0,
	PHYSICS_OBJECT = 1 << 1
};

class SelectedObject {
public:
	SelectedObject() {}
	SelectedObject(const std::string& name, int type) : name(name), selected(false), position{ 0.0f, 0.0f, 0.0f } {}

	bool selected = false;
	std::string name = "";
	float position[3] = { 0.0f, 0.0f, 0.0f };
	std::string ogMaterial = "";

	void ResetLastObjectMaterial(Ogre::SceneManager* scnMgr) {
		auto movableObject = scnMgr->getMovableObject(this->name, "PhysicsObject");
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(movableObject->getParentNode());
		Ogre::Entity* ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
		ent->setMaterialName(this->ogMaterial);
	}

	void UnSelect(Ogre::SceneManager* scnMgr) {

		ResetLastObjectMaterial(scnMgr);

		selected = false;
		name = "";
		position[0] = 0.0f;
		position[1] = 0.0f;
		position[2] = 0.0f;
	}
};