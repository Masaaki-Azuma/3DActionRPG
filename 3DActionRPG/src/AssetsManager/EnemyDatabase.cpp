#include "EnemyDatabase.h"

#include <cassert>

enum //csvデータの各列の内容
{
    ColName = 0,
    ColHp,
    ColAttack,
    ColMaxFlinchCount,
    ColDropGem,
};
void EnemyDatabase::load(const std::string& file_name)
{
    parameters_.load(file_name);
}

const EnemyDatabase::EnemyParameter EnemyDatabase::get_parameter(const std::string& enemy_name) const
{
    EnemyParameter param;
    for (int r = 0; r < parameters_.rows(); ++r) {
        if (parameters_.get(r, ColName) == enemy_name) {
            param.hp = parameters_.geti(r, ColHp);
            param.attack = parameters_.geti(r, ColAttack);
            param.max_flinch_count = parameters_.geti(r, ColMaxFlinchCount);
            param.drop_gem = parameters_.geti(r, ColDropGem);
            break;
        }
    }
    return param;
}

int EnemyDatabase::get_hp(const std::string& enemy) const
{
    for (int r = 0; r < parameters_.rows(); ++r) {
        if (parameters_.get(r, ColName) == enemy) {
            return parameters_.geti(r, ColHp);
        }
    }

    assert(!"敵の名前が不正です");
    return -1;
}

int EnemyDatabase::get_attack(const std::string& enemy) const
{
    for (int r = 0; r < parameters_.rows(); ++r) {
        if ((parameters_.get(r, ColName) + "Attack") == enemy) {
            return parameters_.geti(r, ColAttack);
        }
    }

    assert(!"敵の名前が不正です");
    return -1;
}

int EnemyDatabase::get_drop_gem(const std::string& enemy) const
{
    for (int r = 0; r < parameters_.rows(); ++r) {
        if (parameters_.get(r, ColName) == enemy) {
            return parameters_.geti(r, ColDropGem);
        }
    }

    assert(!"敵の名前が不正です");
    return -1;
}


