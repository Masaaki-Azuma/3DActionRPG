#include "EnemySpawner.h"

#include "IWorld.h"
#include "Actor/Enemy/Slime.h"
#include "Actor/Enemy/Skeleton.h"
//#include "Actor/Enemy/Mage.h"
#include "Actor/Enemy/Mimic.h"
#include "Actor/Enemy/BlackKnight.h"

EnemySpawner::EnemySpawner(IWorld& world):
	world_{world}
{
}

void EnemySpawner::spawn(const std::string& enemy)
{
	//敵名に応じた敵グループを生成
	if (enemy == "Slime") {
		world_.add_actor(std::make_shared<Slime>(&world_, Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
		world_.add_actor(std::make_shared<Slime>(&world_, Vector3{ -500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
		world_.add_actor(std::make_shared<Slime>(&world_, Vector3{ 500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
	}
	else if (enemy == "Skeleton") {
		//world_.add_actor(new Skeleton{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
		world_.add_actor(std::make_shared<Skeleton>(&world_, Vector3{ -500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
		world_.add_actor(std::make_shared<Skeleton>(&world_, Vector3{ 500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
	}
	else if (enemy == "Mage") {
		//world_.add_actor(std::make_shared<Skeleton>(&world_, Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
	}
	else if (enemy == "Mimic") {
		world_.add_actor(std::make_shared<Mimic>(&world_, Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
		world_.add_actor(std::make_shared<Slime>(&world_, Vector3{ -500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
		world_.add_actor(std::make_shared<Slime>(&world_, Vector3{ 500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
	}
	else if (enemy == "BlackKnight") {
		world_.add_actor(std::make_shared<BlackKnight>(&world_, Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f }));
	}
	else if (enemy == "None") {
		/*処理の都合上、スタート地点を表すNoneが渡されることはないので、バグである*/
	}
}
