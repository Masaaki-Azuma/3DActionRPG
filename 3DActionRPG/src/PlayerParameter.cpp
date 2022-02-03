#include "PlayerParameter.h"
#include <fstream>
#include "CsvReader.h"

void PlayerParameter::load()
{
	CsvReader csv_reader;
	csv_reader.load("Assets/Parameters/player_parameter.csv");
	parameter_.max_hp = csv_reader.geti(0, 0);
	parameter_.attack = csv_reader.geti(0, 1);
	parameter_.total_gem = csv_reader.geti(0, 2);
}

void PlayerParameter::save()
{
	std::ofstream of{ "Assets/Parameters/player_parameter.csv" };
	std::string data;
	data += std::to_string(parameter_.max_hp);
	data += ",";
	data += std::to_string(parameter_.attack);
	data += ",";
	data += std::to_string(parameter_.total_gem);
	of << data << std::endl;
}

void PlayerParameter::add_hp(int rise_value)
{
	parameter_.max_hp += rise_value;
}

void PlayerParameter::add_attack(int rise_value)
{
	parameter_.attack += rise_value;
}

void PlayerParameter::add_gem(int rise_value)
{
	parameter_.total_gem += rise_value;
}

const PlayerParameter::Parameter& PlayerParameter::get()
{
	return parameter_;
}
