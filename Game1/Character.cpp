#include "stdafx.h"

void Character::LookTarget(Vector2 target, ObImage* img)
{
	Vector2 dir = target - col->GetWorldPos();
	float radian = Utility::DirToRadian(dir);

	//¿À¸¥ÂÊ
	if (-DIV8PI <= radian && radian <= DIV8PI)
	{
		dirState = Dir_R;
	}
	else if (DIV8PI <= radian && radian <= DIV4PI + DIV8PI)
	{
		dirState = Dir_RT;
	}
	else if (DIV4PI + DIV8PI <= radian && radian <= DIV8PI + DIV4PI * 2.0f)
	{
		dirState = Dir_T;
	}
	else if (DIV8PI + DIV4PI * 2.0f <= radian && radian <= DIV8PI + DIV4PI * 3.0f)
	{
		dirState = Dir_LT;
	}
	else if (-(DIV8PI + DIV4PI * 2.0f) >= radian && radian >= -(DIV8PI + DIV4PI * 3.0f))
	{
		dirState = Dir_LB;
	}
	else if (-(DIV8PI + DIV4PI) >= radian && radian >= -(DIV8PI + DIV4PI * 2.0f))
	{
		dirState = Dir_B;
	}
	else if (-(DIV8PI) >= radian && radian >= -(DIV8PI + DIV4PI))
	{
		dirState = Dir_RB;
	}
	else
	{
		dirState = Dir_L;
	}

	img->frame.y = frameY[dirState];
}

