#include "EnemyDatabase.h"

enum //csvデータの各列の内容
{
    ColName = 0,
    ColHp,
    ColAttack,
    ColMaxStunCount,
};
void EnemyDatabase::load(const std::string& file_name)
{
    parameters_.load(file_name);
}

const EnemyDatabase::EnemyParameter EnemyDatabase::get_parameter(const std::string& enemy_name) const
{
    EnemyParameter param;
    for (int r = 0; r < parameters_.rows(); ++r) {
        if ((parameters_.get(r, ColName) + "Attack") == enemy_name) {
            param.hp = parameters_.geti(r, ColHp);
            param.attack = parameters_.geti(r, ColAttack);
            param.max_stun_count = parameters_.geti(r, ColMaxStunCount);
            break;
        }
    }
    return param;
}
