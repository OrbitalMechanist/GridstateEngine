#include "gamemaster/Component.h"

Component::~Component() {}

Transform& Component::getParentTransform() {
	return parent.transform;
}