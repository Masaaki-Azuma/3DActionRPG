#include "EnemyDatabase.h"

#include <cassert>

#include "AssetsManager/Image.h"

enum //csvデータの各列の内容
{
    ColName = 0,
    ColHp,
    ColAttack,
    ColMaxFlinchCount,
    ColDropGem,
};

//HACK:この二つ統合できない？
const std::unordered_map<std::string, int> EnemyIconTable{
    {"Slime", Texture_icon_slime},
    {"Skeleton", Texture_icon_skeleton},
    {"Mage", Texture_icon_mage},
    {"Mimic", Texture_icon_mimic},
    {"BlackKnight", Texture_icon_blackKnight},
};

const std::unordered_map<std::string, int> EnemySilhouetteTable{

    {"Slime", Texture_silhouette_slime},
    {"Skeleton", Texture_silhouette_skeleton},
    {"Mage", Texture_silhouette_mage},
    {"Mimic", Texture_silhouette_mimic},
    {"BlackKnight", Texture_silhouette_blackKnight},
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

//存在しなければ-1
int EnemyDatabase::enemy_icon_table(const std::string& enemy) const
{
    if (EnemyIconTable.count(enemy) == 0) return -1;

    return EnemyIconTable.at(enemy);
}

//存在しなければ-1
int EnemyDatabase::enemy_silhouette_table(const std::string& enemy) const
{
    if (EnemySilhouetteTable.count(enemy) == 0) return -1;

    return EnemySilhouetteTable.at(enemy);
}


