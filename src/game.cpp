#include "game.h"
#include "graphics.h"
#include "config.h"
#include <iostream>
#include <list>



void Game::fire(float pos_x, float pos_y,float size)
{//.......creates a CollisionObject
	fire1 = new CollisionObject(*this, pos_x, pos_y, size);
	fires.push_back(fire1);
}


void Game::spawnClock()
{//....creates clocks 

	if (enemies.size()<=10) {
		
		enemies_initialized = false;
	}
	
}


void Game::checkClock()
{
	// check if the clocks are not in the window
	for (std::list<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ) {
		Enemy* clock = *e;
		if (clock && !clock->isActive()) {
			e = enemies.erase(e);
		}
		else
			++e;
	}
}

void Game::checkCollision()
{//....check if there is collision between player1's hammer and each clock
	
	if (!player1 || !clock) {
		return ;
	}

	Disk d1 = player1->getCollisionHull();
	for (std::list<Enemy*>::iterator e = enemies.begin(); e != enemies.end();){
	
		Enemy* clock = *e;

		Disk d2 = clock->getCollisionHull();

		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;

		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius) {
			player1->gainLife(gameMode);
			coins += 100 / (clock->getSize());// number of coins depend on size of the clock

			//....explosion
			graphics::playSound(std::string(ASSET_PATH) + "s.mp3", 1.0f, false);
			fire(d2.cx, d2.cy, d2.size);
			//..erase the clock
		
			e = enemies.erase(e);


		}
		else
			++e;
		
	}
		
}

bool Game::endGame()
{//....the game ends if player_life is equal to 1.0f or if time >= 3 minutes (=180000 milliseconds)
	e = graphics::getGlobalTime() - player1->s_time;
	
	if (player1->getRemainingLife() == 1.0f || e>=180000)
		return true;
	return false;
}

void Game::updateStartScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_E)) {//if E is pressed the status changes
		status = STATUS_PLAYING;
		gameMode = 0.01f;

	}else if (graphics::getKeyState(graphics::SCANCODE_H)) {//if H is pressed the status changes
		status = STATUS_PLAYING;
		gameMode = 0.001f;
	}
}

void Game::updateLevelScreen()
{
	if (!player_initialized1 && graphics::getGlobalTime() > 1000) {
		player1 = new Player(*this);
		player_initialized1 = true;
		
	}
	if (player1) {
		player1->update();
	}

	if (!enemies_initialized && graphics::getGlobalTime() > 1000) {
		for (int i = 0; i < 1; i++) {
			clock = new Enemy(*this);
			enemies.push_back(clock);
		}
		enemies_initialized = true;
	}
		
	checkClock();
	spawnClock();
	if (!enemies.empty()) {
		for (auto clock : enemies) {
			clock->update();

		}
	}
	

	checkCollision();
	if (!fires.empty()) {
		for (std::list<CollisionObject*>::iterator e = fires.begin(); e != fires.end();) {
			CollisionObject* fire1 = *e;
			fire1->update();
			if (!fire1->active) {
				e=fires.erase(e);
			}
			else
				++e;
		}

	}

	if (endGame()) {
			
		if (!enemies.empty()) {
			for (auto&& enemy : enemies) {
				delete enemy;
			}
			enemies.clear();
		}
		if (!fires.empty()) {
			for (auto&& fire1 : fires) {
				delete fire1;
			}
			fires.clear();
		}
		status = STATUS_END;	
	}
	
}

void Game::updateEndScreen()
{//.....if the user plays again we change the status, the player's life, the coins, enemies_initialised variable and the s_time

	if (graphics::getKeyState(graphics::SCANCODE_E) || graphics::getKeyState(graphics::SCANCODE_H)) {
		status = STATUS_PLAYING;
		player1-> setLife(0.0f);
		setCoins(0.0f);
		enemies_initialized = false;
		player1->s_time = graphics::getGlobalTime();
		
	}
	if (graphics::getKeyState(graphics::SCANCODE_E))
		setGameMode(0.01f);
	else if (graphics::getKeyState(graphics::SCANCODE_H))
		setGameMode(0.001f);
}

void Game::drawStartScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "T1.png";
	br.outline_opacity = 0.0f;
	//draw backgroung
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);
	//draw explosion images that cannot be seen now so later they appear faster
	br.texture = std::string(ASSET_PATH) + "2.png";
	graphics::drawRect(CANVAS_WIDTH / 2 - 150, CANVAS_HEIGHT / 2 - 100, 20, 20, br);
	br.texture = std::string(ASSET_PATH) + "4.png";
	graphics::drawRect(CANVAS_WIDTH / 2 - 150, CANVAS_HEIGHT / 2 - 100, 20, 20, br);
	br.texture = std::string(ASSET_PATH) + "robot.png";
	graphics::drawRect(CANVAS_WIDTH / 2-150, CANVAS_HEIGHT / 2-100 ,200, 200, br);
	graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2-100, 70, "Unwakeable", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT / 2 + 20, 50, "Help the robot destroy the alarm clocks and gain sleep energy", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2 + 80, 40, "For Easy mode: Press E ", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2 + 110, 40, "For Hard mode: Press H", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 160, CANVAS_HEIGHT / 2 + 150, 40, "Use A, W, S, D buttons to move", br);
}

void Game::drawLevelScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "SK.png";
	br.outline_opacity = 0.0f;
	float offset = CANVAS_HEIGHT* sinf(graphics::getGlobalTime()/1000.0f)/4;
	
	//draw backgroung
	graphics::drawRect(CANVAS_WIDTH / 2 , CANVAS_HEIGHT / 2 + offset, CANVAS_WIDTH, CANVAS_WIDTH, br);

	//draw player

	if (player1)
		player1->draw();

	
	//............draw clocks............
	
	if (!enemies.empty()){
		for (std::list<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e) {
			Enemy* clock = *e;
			clock->draw();
				
		}
		
	}
	//.............draw fire.........

	if (!fires.empty()) {
		for (auto e = fires.begin(); e != fires.end(); ++e) {
			CollisionObject* fire1 = *e;
			fire1->draw();
		}
	}
	
	//ui/ info layer
//.............debug mode info


	if ( debug_mode && player1) {
		char info[40];
		sprintf_s(info, "(%6.2f, %6.2f, %2.2f)", player1->getPosX(), player1->getPosY(), player1->getRemainingLife());
		graphics::drawText(100, 50, 40, info, br);
	}
	
//................
	
//........drawing coins/ cloud.....................
	float player_life = player1 ? player1->getRemainingLife() : 0.0f;
	
	
	br.texture = std::string(ASSET_PATH) + "CLOUD.png";
	br.outline_opacity = 0.0f;
	
	graphics::drawRect(790, 30, 130, 150, br);


	br.outline_opacity = 0.0f;
	br.fill_color[0] = 0.4f;
	br.fill_color[1] = 0.1f;
	br.fill_color[2] = 1.0f;
	
	char info1[40];
	sprintf_s(info1, "%6.0f",coins);
	graphics::drawText(790, 45, 30, info1, br);
	
	graphics::Brush br1;
	br1.outline_opacity = 0.0f;
	br1.texture = std::string(ASSET_PATH) + "coin.png";
	graphics::drawRect(CANVAS_WIDTH - 220, 30, 40, 40, br1);

	



//.....draw time (in seconds).............................

	char info2[40];
	sprintf_s(info2, "%2.0f seconds passed", e / 1000);
	graphics::drawText(50, 50, 40, info2, br);
	graphics::Brush br8;
	br8.texture = std::string(ASSET_PATH) + "CLOUD.png";
	br8.outline_opacity = 0.0f;
	//draw backgroung
	graphics::drawRect(150, 30, 300, 200, br8);
	

//..................draw sleep energy.........................

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 0.89f;
	br.fill_color[1] = 0.15f;
	br.fill_color[2] = 0.21f;
	
	br.texture = "";
	br.fill_secondary_color[0] = 0.89f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.15f;
	br.fill_secondary_color[2] = 0.21f * (1.0f-player_life) + player_life * 0.33f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f-player_life) * 120 / 2), 30, player_life * 120, 20, br);
	
	br.outline_opacity = 1.0f;
	br.gradient = false;

	br.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 20, br);	
}

void Game::drawEndScreen()
{
	
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	
	
	if (player1->getRemainingLife() == 1.0f && e < 180000) {
		br.texture = std::string(ASSET_PATH) + "m.png";
		graphics::drawText(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT / 2 - 100, 40, "Congrats! Keep sleeping!", br);
	}
	else {
		br.texture = std::string(ASSET_PATH) + "E1.png";
		graphics::drawText(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT / 2 - 120, 40, "Sorry! You need to wake up!", br);
	}
	graphics::drawRect(CANVAS_WIDTH / 2 - 170, CANVAS_HEIGHT / 2 - 100, 200, 200, br);
	char info2[80];
	sprintf_s(info2, "In%6.0f seconds you gathered%6.0f coins !", e/1000, coins);
	graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 + 20, 40, info2, br);

	graphics::drawText(CANVAS_WIDTH / 2 - 180, CANVAS_HEIGHT / 2 + 50, 40, "Press E to play again (easy mode)", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 180, CANVAS_HEIGHT / 2 + 80, 40, "Press H to play again (hard mode)", br);
	
}

void Game::update()
{
	if (status == STATUS_START) {
		updateStartScreen();
	}
	else if (status == STATUS_PLAYING) {
		updateLevelScreen();
	}
	else if(status == STATUS_END) {
		updateEndScreen();
	}
}

void Game::draw()
{
	if (status == STATUS_START) {
		drawStartScreen();
	}
	else if (status == STATUS_PLAYING) {
		drawLevelScreen();
	}
	else  if (status == STATUS_END) {
		drawEndScreen();
	}
}

void Game::init()
{
	graphics::setFont(std::string(ASSET_PATH) + "alphabetized cassette tapes.ttf");
	graphics::playMusic(std::string(ASSET_PATH) + "background.mp3", 1.0f, true, 4000);
}

Game::Game()
{
}

Game::~Game()
{
	if (player1) {
		delete player1;
	}
}
