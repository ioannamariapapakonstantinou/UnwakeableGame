#pragma once
#include "gameobject.h"
#include "graphics.h"

class Enemy :public GameObject, public Collidable{

	float pos_x, pos_y;
	float speed;
	float size;
	float rotation;
	graphics::Brush brush;
	bool active = true;
	
public:
	
	void update() override;
	void draw()override;
	void init() override ;
    bool isActive() { return active; }
	Enemy(const class Game & mygame);
	float getPosY() { return pos_y; }
	float getPosX() { return pos_x; }
	float getSize() { return size; }
	~Enemy();
	Disk getCollisionHull() const override;
};