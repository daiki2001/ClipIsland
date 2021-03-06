#include "RVector.h"

bool RV3Colider::ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos)
{
	//平面と球の中心との距離を求める
	float dist = dot(sphere.center, plane.normal) - plane.distance;
	//距離の絶対値が半径より大きければあたってない
	if (fabsf(dist) > sphere.rad) { return false; }
	//あたっている場合は疑似交点を計算
	*coliPos = -dist * plane.normal + sphere.center;
	//衝突
	return true;
}

const RVector3 Rv3Ease::lerp(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t) + end * t;
}

const RVector3 Rv3Ease::InQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t * t) + end * pow(t, 2);
}

const RVector3 Rv3Ease::OutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (1.0f - (1.0f - t) * (1.0f - t))) + end * (1.0f - (1.0f - t) * (1.0f - t));
}

const RVector3 Rv3Ease::InOutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return t < 0.5f ? (start * (1.0f - t)) + end * (t * t) : start * (1.0f - t) + end * (1 - pow(-2 * t + 2, 2) / 2);
}
