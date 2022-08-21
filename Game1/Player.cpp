#include "stdafx.h"

Player::Player()
{
	col = new ObRect();
	col->SetWorldPos(Vector2(-500.0f, -200.0f));
	col->scale = Vector2(96.0f, 96.0f);
	col->isFilled = false;

	walk = new ObImage(L"Walk.png");
	walk->scale = Vector2(96.0f, 96.0f);
	walk->maxFrame = Int2(6, 8);
	walk->SetParentRT(*col);

	roll = new ObImage(L"Roll.png");
	roll->scale = Vector2(96.0f, 96.0f);
	roll->maxFrame = Int2(6, 8);
	roll->SetParentRT(*col);
	roll->visible = false;

	frameY[Dir_R] = 0;
	frameY[Dir_T] = 1;
	frameY[Dir_L] = 2;
	frameY[Dir_B] = 3;
	frameY[Dir_RB] = 4;
	frameY[Dir_LB] = 5;
	frameY[Dir_LT] = 6;
	frameY[Dir_RT] = 7;

	plState = PlayerState::IDLE;
}

Player::~Player()
{
	SafeDelete(col);
	SafeDelete(walk);
	SafeDelete(roll);
}

void Player::Update()
{
	switch (plState)
	{
	case PlayerState::IDLE:
		Idle();
		break;
	case PlayerState::WALK:
		Walk();
		break;
	case PlayerState::ROLL:
		Roll();
		break;
	}

	col->Update();
	walk->Update();
	roll->Update();
}

void Player::Render()
{
	col->Render();
	walk->Render();
	roll->Render();
}

void Player::Idle()
{	
	Input();
	LookTarget(INPUT->GetMouseWorldPos(), walk);
	
	//Idle->Walk
	if (moveDir != Vector2(0.0f, 0.0f))
	{
		plState = PlayerState::WALK;
		walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	}
}

void Player::Walk()
{
	Input();
	LookTarget(INPUT->GetMouseWorldPos(), walk);

	col->MoveWorldPos(moveDir * 200.0f * DELTA);

	//Walk -> Idle
	if (moveDir == Vector2(0.0f, 0.0f))
	{
		plState = PlayerState::IDLE;
		walk->ChangeAnim(ANIMSTATE::STOP, 0.1f);
		walk->frame.x = 0;
	}

	//Walk -> Roll
	if (INPUT->KeyPress(VK_SPACE))
	{
		plState = PlayerState::ROLL;
		roll->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		walk->visible = false;
		roll->visible = true;
		rollTime = 0.0f;
	}
}

void Player::Roll()
{
	rollTime += DELTA;

	Input();
	LookTarget(col->GetWorldPos() + moveDir, roll);

	col->MoveWorldPos(moveDir * 500.0f *
		cosf(rollTime / 0.6f * DIV2PI) * DELTA);
	//			0 ~ 1		   0 ~ 90

	//Roll -> Walk
	if (rollTime > 0.6f)
	{
		plState = PlayerState::WALK;
		walk->visible = true;
		roll->visible = false;
	}
}

void Player::Input()
{
	moveDir = Vector2(0.0f, 0.0f);

	if (INPUT->KeyPress('S'))
	{
		moveDir.y = -1.0f;
	}
	else if (INPUT->KeyPress('W'))
	{
		moveDir.y = 1.0f;
	}

	if (INPUT->KeyPress('A'))
	{
		moveDir.x = -1.0f;
	}
	else if (INPUT->KeyPress('D'))
	{
		moveDir.x = 1.0f;
	}
	
	moveDir.Normalize();
}


