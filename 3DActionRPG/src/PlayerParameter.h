#pragma once

#include "Singleton.h"

//プレイヤーに関するセーブデータ管理クラス
class PlayerParameter : public Singleton<PlayerParameter>
{
public:
	struct Parameter
	{
		int max_hp;
		int attack;
		int total_gem;
	};
public:
	void load();
	void save();
	void add_hp(int rise_value);
	void add_attack(int rise_value);
	void add_gem(int rise_value);
	const Parameter& get();

private:
	Parameter parameter_;

private:
	PlayerParameter() = default;
	‾PlayerParameter() = default;
private:
	friend class Singleton<PlayerParameter>;

};

