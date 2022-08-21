#pragma once

enum DirState
{
	Dir_L,
	Dir_R,
	Dir_T,
	Dir_B,
	Dir_LT,
	Dir_LB,
	Dir_RT,
	Dir_RB,
	Dir_N
};

class Character
{
protected:
	ObRect* col;
	Vector2 moveDir;

	int dirState;
	int frameY[8];

	Vector2 target;

public:
	void LookTarget(Vector2 target, ObImage* img);

	void SetTarget(Vector2 target)
	{
		this->target = target;
	}
	Vector2 GetPos()
	{
		return col->GetWorldPos();
	}
};

