#ifndef ENEMY_SPAWNER_H_
#define ENEMY_SPAWNER_H_

#include <string>

class IWorld;

class EnemySpawner
{
public:
	EnemySpawner(IWorld& world);
	void spawn(const std::string& enemy);

private:
	IWorld& world_;
};
#endif//!ENEMY_SPAWNER_H_

