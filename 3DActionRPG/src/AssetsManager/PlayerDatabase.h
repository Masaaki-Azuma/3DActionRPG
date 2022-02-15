#ifndef PLAYER_DATABASE_H_
#define PLAYER_DATABASE_H_

#include "Singleton.h"

#include <string>

//プレイヤーに関するセーブデータや進行データの管理クラス
class PlayerDatabase : public Singleton<PlayerDatabase>
{
public:
	struct Parameter
	{
		int hp;
		int attack;
		int total_gem;
	};
public:
	void load(const std::string& file_name);
	void save(const std::string& file_name);
	void add_hp(int rise_value);
	void add_attack(int rise_value);
	void add_gem(int rise_value);

	void set_hp(int hp);
	const Parameter& get_current_parameter();

private:
	Parameter current_parameter_{ -1, -1, -1 };

private:
	PlayerDatabase() = default;
	‾PlayerDatabase() = default;
private:
	friend class Singleton<PlayerDatabase>;

};
#endif//!PLAYER_DATABASE_H_

