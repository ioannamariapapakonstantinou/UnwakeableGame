#pragma once
#include "gameobject.h"
#include "graphics.h"
#include "config.h"
#include <string>

class CollisionObject :public GameObject{

	graphics::Brush brush;
	int k = 0;
	float pos_x ;
	float pos_y ; 
	float size ;
	bool i = false;

public:
	bool active = true;
	void update() override;
	void draw()override;
	void init() override;

	CollisionObject(const class Game& mygame, float pos_x, float pos_y, float size);
	
	~CollisionObject();
	
};