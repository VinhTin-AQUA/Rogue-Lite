#pragma once
#ifndef Enemies_h
#define Enemies_h

#include <vector>
#include <SDL.h>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "Explosion.h"
#include "TextureManager.h"
#include "Sound.h"

const unsigned int NUM_OF_ENEMIES = 20;
using namespace std;

class Enemies
{
private:
	vector<Enemy*> _Enemies;
	vector<Explosion*> _ExplosionDestroy;
	Sound* sound;
public:
	Enemies();
	~Enemies();

	vector<Enemy*> GetEnemies();
	int GetSize();
	void SetHealthEnemy(int i);
	int GetHealthEnemy(int i);
	Enemy* GetEnemy(int i);

	void DrawEnemies();
	void DrawBulletsEnemies();
	void Update(Player* player, bool AlivePlayer);

	void DeleteEnemy(int i);
	void DeleteBulletEnemy(Enemy* enemy, int i);
	void DoBulletEnemies();
	void DrawExplosion();

	// reset enemies
	void Reset();
};

#endif
