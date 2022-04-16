#pragma once
#include "player.h"
#include "Enemy.h"
#include <list>
#include "collisionObject.h"

class Game {

	typedef enum {STATUS_START, STATUS_PLAYING, STATUS_END} status_t;
	status_t status = STATUS_START;

	Player * player1= nullptr;
	bool player_initialized1 = false;

	bool debug_mode = false;
	
	std::list <Enemy*> enemies;
	bool enemies_initialized = false;
	Enemy* clock = nullptr;

	std::list<CollisionObject*> fires;
	CollisionObject* fire1 = nullptr;
	float e = 0.1f;//how much time the player needed to complete the game...If time is equal or more than 3 minutes they fail

	float gameMode = 0;
	void fire(float pos_x, float pos_y, float size);
	void spawnClock();
	void checkClock();
	void checkCollision();
	bool endGame();
	float coins=0;
	
	void updateStartScreen();
	void updateLevelScreen();
	void updateEndScreen();

	void drawStartScreen();
	void drawLevelScreen();
	void drawEndScreen();

public:
	void setGameMode(float x) { gameMode = x; }
	float getGameMode() { return gameMode; }
	
	bool getDebugMode() const { return debug_mode; }
	void setDebugMode(bool d) { debug_mode = d; }
	void update();
	void draw();
	void init();
	void setCoins(float n) { coins = n; }
	Game();
	~Game();
};