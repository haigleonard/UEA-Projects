#include "CollisionDetection.h"



CollisionDetection::CollisionDetection()
{
}


CollisionDetection::~CollisionDetection()
{
}

bool CollisionDetection::Collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float area) {
	x1 -= (area / 3);
	x2 -= (area / 3);

	w1 = area;
	h1 = area;

	if (x1 < x2 + w2 &&
		x1 + w1 > x2 &&
		y1 < y2 + h2 &&
		h1 + y1 > y2) {
		return true;
	}
	else {
		return false;
	}
}

