#pragma once

enum class PlayerState
{
	IDLE,
	WALK,
	ROLL
};

class Player : public Character
{
private:
	ObImage*	walk;
	ObImage*	roll;
	PlayerState plState;
	
	float		rollTime;

public:
	void Idle();
	void Walk();
	void Roll();

	void Input();

public:
	Player();
	~Player();

	void Update();
	void Render();
};

