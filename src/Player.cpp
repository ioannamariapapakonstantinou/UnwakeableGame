#include "player.h"
#include "graphics.h"
#include "game.h"

Player::Player(const Game& mygame)
	:GameObject(mygame)
{
	s_time = graphics::getGlobalTime();//the moment the player is created is the moment the playing mode starts
}

void Player::update()
{
	
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		pos_x -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		pos_x += speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		pos_y -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		pos_y += speed * graphics::getDeltaTime() / 10.0f;
	}
	
	if (pos_x < 0)pos_x = 0;
	if (pos_x > CANVAS_WIDTH)pos_x = CANVAS_WIDTH; 
	if (pos_y < 0)pos_y = 0;
	if (pos_y > CANVAS_WIDTH)pos_y = CANVAS_HEIGHT;
}

void Player::draw()
{

	graphics::Brush br;
	br.outline_opacity = 0.0f;
	
	graphics::setScale(height,height);

	br.fill_opacity = 1.0f; 
	br.texture = std::string(ASSET_PATH) + "robot.png";
	graphics::drawRect(pos_x, pos_y, 200, 200, br);
	graphics::resetPose();
	
	if (game.getDebugMode()) {
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;
		Disk hull = getCollisionHull();
		graphics::drawDisk (hull.cx, hull.cy, hull.radius,  br );
		
	}
}

void Player::init()
{
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x+70;
	disk.cy = pos_y-50;
	disk.radius = 30.0f;
	return disk;
	
	
	
}
