#include "collisionObject.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"



void CollisionObject::update()
{
	if (k<=6)//k = counter
		i = !i; 
	else
		active = false;//if all images are shown inactivate the explosion
}

void CollisionObject::draw()
{// explosion consists of 2 images named 2.png, 4.png that are shown 6 times in total
	
	graphics::Brush brush;
	brush.outline_opacity = 0.0f; 
	if (i)
		brush.texture = std::string(ASSET_PATH) + "2.png";
	else 
		brush.texture = std::string(ASSET_PATH) + "4.png";
	graphics::drawRect(pos_x, pos_y, size, size, brush);
	k ++;
}

void CollisionObject::init()
{
}

CollisionObject::CollisionObject(const Game& mygame,float x, float y,float s)
:GameObject(mygame){
	pos_x = x;//the explosion must have the same size and coordinates with the destroyed clock
	pos_y = y;
	size = s;
	init();
}

CollisionObject::~CollisionObject()
{
}
