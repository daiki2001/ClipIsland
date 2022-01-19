#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

Collision::Collision(RVector3 minValue, RVector3 maxValue, RVector3 actorPos)
{
	min = minValue;
	max = maxValue;
	oldPos = actorPos;
}

void Collision::Update(const RVector3 &actorPosition)
{
	RVector3 diff = actorPosition - oldPos;
	min += diff;
	max += diff;
	oldPos = actorPosition;
}

//true@Õ“Ë
bool intersectAABB(const Collision &box1, const Collision &box2)
{
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;
	return true;
}
