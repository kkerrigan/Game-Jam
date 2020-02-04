#include "physicsShapes.h"

cAABBTerrianBroadPhase::cAABBTerrianBroadPhase() : id(0), center(glm::vec3(0.0f)), halfLength(0.0f) {}

cAABBTerrianBroadPhase::cAABBTerrianBroadPhase(glm::vec3 center, float halfLength) : id(0), center(center), halfLength(halfLength) {
	this->id = this->generateId(center);
}

glm::vec3 cAABBTerrianBroadPhase::getMinXYZ() {
	return this->center - glm::vec3(this->halfLength);
}

float cAABBTerrianBroadPhase::getSideLength() {
	return this->halfLength * 2;
}

glm::vec3 cAABBTerrianBroadPhase::getCenterFromPosition(glm::vec3 pos, float halfLength) {
	
	pos.x = (floor((pos.x + halfLength) / ((float)halfLength * 2))) * halfLength * 2;
	pos.y = (floor((pos.y + halfLength) / ((float)halfLength * 2))) * halfLength * 2;
	pos.z = (floor((pos.z + halfLength) / ((float)halfLength * 2))) * halfLength * 2;
	
	return pos;
}

cAABBTerrianBroadPhase::id_type cAABBTerrianBroadPhase::generateId(glm::vec3 center) {
	
	unsigned long long theABS_X = (unsigned long long(fabs(center.x)));
	unsigned long long theABS_Y = (unsigned long long(fabs(center.y)));
	unsigned long long theABS_Z = (unsigned long long(fabs(center.z)));

	// Add the "sign" digit:
	// If +ve, then the sign is 0, eg: 193 would be:  000193   (000 193)
	// If -ve, then the sign is 1, eg: -193 would be: 100193   (100 193)
	if (center.x < 0.0f) { theABS_X += 100000; }	// Sets 6th digit to 1
	if (center.y < 0.0f) { theABS_Y += 100000; }	// Sets 6th digit to 1
	if (center.z < 0.0f) { theABS_Z += 100000; }	// Sets 6th digit to 1

	unsigned long long theID =
		theABS_Z
		+ (theABS_Y * 1000000)				// Shift the y to the 7th digit
		+ (theABS_X * 1000000 * 1000000);	// Shift the x to the 13th

	return theID;
}

cAABBTerrianBroadPhase::id_type cAABBTerrianBroadPhase::generateIdFromPosition(glm::vec3 pos, float halfLength) {
	
	pos.x = (floor((pos.x + halfLength) / ((float)halfLength * 2))) * halfLength * 2;
	pos.y = (floor((pos.y + halfLength) / ((float)halfLength * 2))) * halfLength * 2;
	pos.z = (floor((pos.z + halfLength) / ((float)halfLength * 2))) * halfLength * 2;
	
	return cAABBTerrianBroadPhase::generateId(pos);
}