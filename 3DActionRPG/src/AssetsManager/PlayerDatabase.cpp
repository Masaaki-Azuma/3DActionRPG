#include "PlayerDatabase.h"

#include <fstream>

#include "Util/CsvReader.h"

enum
{
	ColHp = 0,
	ColAttack,
	ColTotalGem,
};
void PlayerDatabase::load(const std::string& file_name)
{
	CsvReader csv_reader;
	csv_reader.load(file_name);
	current_parameter_.hp = csv_reader.geti(0, ColHp);
	current_parameter_.attack = csv_reader.geti(0, ColAttack);
	current_parameter_.total_gem = csv_reader.geti(0, ColTotalGem);
}

void PlayerDatabase::save(const std::string& file_name)
{
	std::ofstream of{ file_name };
	std::string data;
	std::string hp_str = std::to_string(current_parameter_.hp);
	std::string attack_str = std::to_string(current_parameter_.attack);
	std::string total_gem_str = std::to_string(current_parameter_.total_gem);
	data = hp_str + "," + attack_str + "," + total_gem_str;
	of << data << std::endl;
	of.close();
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

const PlayerDatabase::Parameter& PlayerDatabase::get_current_parameter()
{
	return current_parameter_;
}
