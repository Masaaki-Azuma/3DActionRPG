#ifndef PLAYER_DATABASE_H_
#define PLAYER_DATABASE_H_

#include "Singleton.h"
#include <string>

//プレイヤーに関するセーブデータ管理クラス
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
	void save();
	void add_hp(int rise_value);
	void add_attack(int rise_value);
	void add_gem(int rise_value);
	const Parameter& get_current_parameter();

private:
	Parameter current_parameter_;

private:
	PlayerDatabase() = default;
	‾PlayerDatabase() = default;
private:
	friend class Singleton<PlayerDatabase>;

};
#endif//!PLAYER_DATABASE_H_

