#pragma once

enum QueryType {
	ENTITY = 1 << 0,
	PHYSICS_OBJECT = 1 << 1
};

class SelectedObject {
	public:
	SelectedObject() : name("") {}
	SelectedObject(const std::string& name, int type) : name(name) {}

	std::string name;
	
	//Ogre::;
};