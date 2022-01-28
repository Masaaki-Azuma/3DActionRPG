#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <memory>
#include <vector>
#include "Singleton.h"

class Player;
class PlayerBullet;

class GameManager : public Singleton<GameManager>
{
public:
	std::shared_ptr<Player> player{ nullptr };
	std::vector<std::shared_ptr<PlayerBullet>> player_bullets;

	template<typename T, class T_if>
	void erase_remove_if(std::vector<T>& v, T_if if_condition)
	{
		v.erase(
			std::remove_if(v.begin(), v.end(), if_condition),
			v.end()
		);
	}

private:
	GameManager() = default;
	‾GameManager() = default;

private:
	friend class Singleton<GameManager>;
};

/*
1.外部からの生成を禁止
2.基底シングルトンにのみ公開
*/

#endif//!GAME_MANAGER_H_

