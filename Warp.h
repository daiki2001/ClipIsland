#pragma once
#include "RVector.h"

class Warp
{
public:
	Warp();
	~Warp();

	void SetWarp(const RVector3 pos, const RVector3& forwordVec);

public:
	RVector3 pos;
	RVector3 forwordVec;
private:
	bool isWarp;
};
