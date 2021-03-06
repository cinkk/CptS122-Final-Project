#include "GameLoop.h"



GameLoop::GameLoop()
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::runLoop(int & playerScore)
{
	// initialized window
	sf::RenderWindow window(sf::VideoMode(1489, 644), "Andy VS.Zombies III!", sf::Style::Close);
	//gamestart
	buffer_gamestart.loadFromFile("gamestart.wav");
	sound_gamestart.setBuffer(buffer_gamestart);
	sound_gamestart.play();
	// for loading textures
	sf::Texture texture, player_t;
	//Setup Background
	texture.loadFromFile("Background.png");
	sf::Sprite background;
	background.setTexture(texture);
	//Setup Player
	player_t.loadFromFile("AndySun.png");
	Player p1(PLAYER_SPEED, PLAYER_HEALTH);
	p1.attachGun("Lasercannon.png", sf::Vector2f(0, 78), FIRE_DELAY, BULLET_SPEED, BULLET_DAMAGE, &window);
	p1.setTexture(player_t); //custom setTexture, sets origin as well.
	p1.setScale(PLAYER_SCALE, PLAYER_SCALE);
	p1.setPosition(background.getGlobalBounds().width / 2, background.getGlobalBounds().height / 2);

	// Setup Enemy Spawner
	EnemySpawner spawner("Zombie1.png", "Zombie2.png", SPAWN_ACCELERATION, ENEMY_SPAWN_DELAY, &p1, background);
	// Setup Collision Detecter
	Collision collider(&spawner.getEnemies(), &p1.getGun()->getBullets(), &p1);
	//Setup Controller
	Controller p1Controller(&p1);

	//setup backgroung music

	sf::Music music;
	music.setVolume(80);
	music.openFromFile("BeethovenVirus.wav"); // green tier
	music.play();
	music.setLoop(true);


	while (window.isOpen())
	{
		//Setup Text 
		sf::Font font;
		font.loadFromFile("zombie.ttf"); //Grabs font
		std::ostringstream sscore, shp;
		sf::Color orange(255, 165, 0, 255), cream(255, 215, 0, 255);
		sscore << playerScore;
		shp << p1.getHealth();
		sf::Text score, hp;

		hp.setFont(font);
		hp.setCharacterSize(100);
		if (p1.getHealth() < PLAYER_HEALTH * 0.1 && hp.getColor() != sf::Color::Red) // red tier
		{
			hp.setColor(sf::Color::Red);
		}
		else if (p1.getHealth() < PLAYER_HEALTH * 0.3 && hp.getColor() != orange) // 
		{
			hp.setColor(orange);
		}
		else if (p1.getHealth() < PLAYER_HEALTH * 0.6 && hp.getColor() != cream)
		{
			hp.setColor(cream);
		}
		else
		{
			hp.setColor(sf::Color::Green);
		}
		hp.setPosition(1100, 0);
		hp.setString(shp.str());

		score.setFont(font);
		score.setCharacterSize(100);
		score.setColor(sf::Color::Magenta);
		score.setPosition(150, 0);
		score.setString(sscore.str());

		sf::Event event;
		while (window.pollEvent(event))
		{
			p1Controller.runEvent(event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(background);
		p1.update(window); //draws gun, bullts, updates player, gun
		p1.draw(window); //custom player draw function
		spawner.update(background, window); // runs spawner suite
		collider.update(playerScore, window); // checks all possible collisions
		window.draw(score);
		window.draw(hp);
		window.display();
	}
}
