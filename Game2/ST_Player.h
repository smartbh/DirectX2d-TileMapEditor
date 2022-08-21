#pragma once
#define MAX 30
class ST_Player
{
private:
	ObImage*		LR;
	ObImage*		boost;
	
	Vector2			dir;

	bool			isBoosting;
	float			LRkey;

	int				level;

public:
	ObRect* col;
	ST_playerBullet bullet[MAX];

public:
	ST_Player();
	~ST_Player();

	void Update();
	void Render();

};

