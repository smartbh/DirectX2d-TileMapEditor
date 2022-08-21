#pragma once

enum class MonsterState
{
	IDLE,
	LOOK = 500,
	MOVE = 300,
	ATTACK = 100
};

class Monster : public Character
{
private:
	ObImage*		img;
	MonsterState	monsterState;
	ObCircle*		range[3];
	float			distance;

	bool			scaleSwitching;

public:
	Monster();
	~Monster();

	void Update();
	void Render();

	void Idle();
	void Look();
	void Move();
	void Attack();
};

