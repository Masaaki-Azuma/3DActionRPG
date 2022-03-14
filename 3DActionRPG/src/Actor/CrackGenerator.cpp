#include "CrackGenerator.h"

#include "BattleScene/IWorld.h"
#include "Actor/CrackAttack.h"
#include "AssetsManager/Sound.h"

CrackGenerator::CrackGenerator(IWorld& world, const Vector3& position, const Vector3& direction):
	world_{world},
	direction_{direction.Normalize()}
{
	position_ = position;
	enable_collider_ = false;
}

void CrackGenerator::update(float delta_time)
{
	timer_.update(delta_time);
	generate_crack();
}

void CrackGenerator::generate_crack()
{
	static const int NumCrack = 6;
	static const float CrackTimeInterval = 0.25f;
	static const float CrackDistance = 250.0f;
	//一定時間おきに地割れ攻撃を生成
	for (int i = 0; i < NumCrack; ++i) {
		if (timer_.has_excessed(CrackTimeInterval * i)) {
			Sound::GetInstance().play_SE(SE_Crack);
			Vector3 crack_pos = position_ + direction_ * CrackDistance * static_cast<float>(i);
			world_.add_actor(new CrackAttack{&world_, crack_pos});
		}
	}

	//最後の地割れ攻撃を生成後消滅
	if (timer_.has_elapsed((NumCrack - 1) * CrackTimeInterval)) {
		die();
	}
}
	
