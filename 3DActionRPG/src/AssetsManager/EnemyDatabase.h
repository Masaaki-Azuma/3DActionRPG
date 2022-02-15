#ifndef ENEMY_DATABASE_H_
#define ENEMY_DATABASE_H_

#include "Singleton.h"

#include <string>
#include <unordered_map>

#include "Util/CsvReader.h"


//敵のパラメーター等データを管理するクラス
class EnemyDatabase : public Singleton<EnemyDatabase>
{
public:
	struct EnemyParameter
	{
		int hp{ -1 };             //体力
		int attack{ -1 };         //攻撃力
		int max_stun_count{ -1 }; //被弾時に連続で硬直する最大回数
		int drop_gem{ -1 };       //討伐時落とすジェム
	};
public:
	void load(const std::string& file_name);
	const EnemyParameter get_parameter(const std::string& enemy_name) const;
	int get_hp(const std::string& enemy) const;
	int get_attack(const std::string& enemy) const;
	int get_drop_gem(const std::string& enemy) const;

private:
	//std::unordered_map<std::string, EnemyParameter> parameters_;
	CsvReader parameters_;

private:
	EnemyDatabase() = default;
	‾EnemyDatabase() = default;
private:
	friend class Singleton<EnemyDatabase>;
};
#endif//!ENEMY_DATABASE_H_

