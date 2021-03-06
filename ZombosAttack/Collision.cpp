#include <SFML\Graphics.hpp>
#include "Collision.h"
#include "KeyValues.h"

Collision::Collision(std::vector<Enemy*>* arr, std::vector<Bullet*>* bulletsArr, Player* player)
{
	this->enemyArray = arr;
	this->bullets = bulletsArr;
	p1 = player;	
}

bool Collision::CheckCol(sf::Sprite& s1, sf::Sprite& s2)
{
	bool result = false;

	sf::Vector2f direction;
	direction.x = s1.getPosition().x - s2.getPosition().x;
	direction.y = s1.getPosition().y - s2.getPosition().y;

	float distance = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

	float totalWidth = (s1.getGlobalBounds().width + s2.getGlobalBounds().width) / 2;

	if (distance < totalWidth)
	{
		//std::cout << "something happened!" << std::endl;
		result = true;
	}

	return result;
}

void Collision::update(int& totalScore, sf::RenderWindow& window)
{
	sf::Sprite enemy, player, bullet;

	for (int i = 0; i < enemyArray->size(); i++) //runs through every enemy
	{

		enemy = *(*enemyArray)[i];
		player = *p1;
		if (this->CheckCol(enemy, player))
		{
			playerAndEnemyCollision(i, window);
		}
		bool killed = false; // enemy is removed from the vector if killed, this bool prevents errors caused by this
		for (int j = 0; j < bullets->size() && !killed; j++) // checks all bullets against current enemy
		{
			bullet = *(*bullets)[j];
			if (this->CheckCol(enemy, bullet))
			{
				killed = bulletsAndEnemyCollision(i, j, totalScore);
			}
		} // !j < bullets->size()
	} // !i < enemyArray->size()
}

void Collision::playerAndEnemyCollision(int enemyIndex, sf::RenderWindow& window)
{
	p1->damageForN((*enemyArray)[enemyIndex]->getDamage());


	if (p1->getHealth() <= 0)
	{
		window.close();
		// call game over
	}
}

bool Collision::bulletsAndEnemyCollision(int &enemyIndex, int &bulletIndex, int& totalScore)
{
	bool killed = false;
	int points = (*enemyArray)[enemyIndex]->getBaseHealth();
	(*enemyArray)[enemyIndex]->damageForN(p1->getGun()->getDamage());// damages enemy for bullet damage
	delete (*bullets)[bulletIndex];
	bullets->erase(bullets->begin() + bulletIndex);
	--bulletIndex;
	
	if ((*enemyArray)[enemyIndex]->getHealth() <= 0) // kills enemy
	{
		totalScore += points * SCORE_MULTIPLIER;
		delete (*enemyArray)[enemyIndex];
		enemyArray->erase(enemyArray->begin() + enemyIndex);
		--enemyIndex;
		killed = true;
		
	}
	return killed;
}