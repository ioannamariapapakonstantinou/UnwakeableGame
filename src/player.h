#pragma once
#include "gameobject.h"
#include "config.h"

class Player :public GameObject, public Collidable {
	float speed = 10.0f;
	float pos_x=CANVAS_WIDTH/2, pos_y=CANVAS_HEIGHT/2;
	float height = 1.0f;
	float life = 0.0f;
public:
	float s_time;
	Player(const class Game& mygame);
	void update() override;
 	void draw() override;
	void init() override;
	Disk getCollisionHull() const override;
	float getPosY() { return pos_y; }
	float getPosX() { return pos_x; }
	float getRemainingLife() const { return life; }
	void gainLife(float amount) { life = std::min <float>(1.0f, life + amount); }
	void setLife(float n) { life = n; }
	
};