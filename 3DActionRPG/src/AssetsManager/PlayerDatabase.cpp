#include "PlayerDatabase.h"

#include <fstream>

#include "Util/CsvReader.h"



void PlayerDatabase::set_dst_file(const std::string& file_name)
{
	master_file_name_ = file_name;
}

void PlayerDatabase::load_master_data()
{
	CsvReader csv_reader;
	csv_reader.load(master_file_name_);
	master_parameter_.hp = csv_reader.geti(0, ColHp);
	master_parameter_.attack = csv_reader.geti(0, ColAttack);
	master_parameter_.total_gem = csv_reader.geti(0, ColTotalGem);
}

void PlayerDatabase::save_master_data()
{
	std::ofstream of{ master_file_name_ };
	std::string data;
	std::string hp_str = std::to_string(master_parameter_.hp);
	std::string attack_str = std::to_string(master_parameter_.attack);
	std::string total_gem_str = std::to_string(master_parameter_.total_gem);
	data = hp_str + "," + attack_str + "," + total_gem_str;
	of << data << std::endl;
	of.close();
}

//ゲーム開始時にプレイヤーのパラメータを初期化する
void PlayerDatabase::set_initial_parameter()
{
	//現在の強化状態でゲームを開始
	current_parameter_ = master_parameter_;
	//ただし、所持ジェム数はいったんゼロに
	current_parameter_.total_gem = 0;
}

void PlayerDatabase::add_hp(int rise_value)
{
	current_parameter_.hp += rise_value;
}

void PlayerDatabase::add_attack(int rise_value)
{
	current_parameter_.attack += rise_value;
}

void PlayerDatabase::add_gem(int rise_value)
{
	current_parameter_.total_gem += rise_value;
}

void PlayerDatabase::use_gem(int use_value)
{
	master_parameter_.total_gem -= use_value;
}

void PlayerDatabase::set_hp(int hp)
{
	current_parameter_.hp = hp;
}

const PlayerDatabase::Parameter& PlayerDatabase::get_current_parameter()
{
	return current_parameter_;
}

const PlayerDatabase::Parameter& PlayerDatabase::get_master_parameter()
{
	return master_parameter_;
}

void PlayerDatabase::add_possessed_jem(int rise_gem)
{
	master_parameter_.total_gem += rise_gem;
	save_master_data();
}

void PlayerDatabase::enhance_hp(int rise_value)
{
	master_parameter_.hp += rise_value;
}

void PlayerDatabase::enhance_attack(int rise_value)
{
	master_parameter_.attack += rise_value;
}
