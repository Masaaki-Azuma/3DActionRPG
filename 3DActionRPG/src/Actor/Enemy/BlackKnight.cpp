#include "BlackKnight.h"

#include <cassert>

#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "BattleScene/IWorld.h"
#include "Actor/CrackGenerator.h"

#include "AssetsManager/Image.h"

enum
{
	Motion_Attack01 = 0,
	Motion_Attack02 = 1,
	Motion_Attack03 = 2,
	Motion_Die = 3,
	Motion_Dizzy = 4,
	Motion_Damage = 5,
	Motion_Idle = 6,
	Motion_Defence = 7,
	Motion_Guard = 8,
	Motion_Run = 10,
	Motion_WalkBackward = 15,
	Motion_WalkForward = 16
};


const float DetectionRadius{ 800.0f }; //プレイヤーを検知する範囲半径
const float AttackRadius{ 350.0f };  //プレイヤーを攻撃する範囲半径
const float EscapeRadius{ 200.0f };
const float MoveSpeed{ 200.0f };

//TODO:コンボガードを入れれば、ゲームバランスがちょうどよくなりそう
BlackKnight::BlackKnight(IWorld* world, const Vector3& position, const Vector3& rotation):
	Enemy{ world, position, rotation }
{
	assert(DetectionRadius >= AttackRadius && "プレイヤー感知半径が不正です");

	name_ = "BlackKnight";
	move_speed_ = MoveSpeed;
	collider_ = Sphere{ 200.0f, Vector3{0.0f, 20.0f, 0.0f} };
	motion_ = Motion_Idle;
	parameter_ = e_DB_.get_parameter(name_);

	//メッシュ姿勢初期化
	mesh_ = SkinningMesh{ Mesh::black_knight_handle, 20.0f };
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.set_scale(Vector3{ 1.0f, 1.0f, 1.0f });
}

void BlackKnight::react_player_attack(Actor& other)
{
	if (other.tag() == "PlayerAttackTag") {
		flinch_count_++;
		//プレイヤーの攻撃力分ダメージを受ける
		take_damage(PlayerDatabase::GetInstance().get_current_parameter().attack);
		//体力ゼロで死亡
		if (parameter_.hp <= 0) {
			//当たり判定を無効化
			enable_collider_ = false;
			//死亡状態に遷移
			change_state(State::Dead, Motion_Die, false);
			mesh_.change_anim(motion_, motion_loop_, motion_interruption);
			return;
		}

		if (flinch_count_ >= parameter_.max_flinch_count) {
			//ひるみ回数をリセット
			flinch_count_ = 0;
			//プレイヤー方向へ向き直る
			make_approach();
			//反撃
			change_state(StateBK::Slash, Motion_Attack01, false, true);
			mesh_.change_anim(motion_, motion_loop_, motion_interruption);
		}
		else {
			//ダメージ状態に
			change_state(State::Damage, Motion_Damage, false, true);
			mesh_.change_anim(motion_, motion_loop_, motion_interruption);
		}
	}
}

void BlackKnight::update_state(float delta_time)
{
	switch (state_) {
	case StateBK::Move:   move(delta_time);   break;
	case StateBK::Attack: attack(delta_time); break;
	case StateBK::Tackle: tackle(delta_time); break;
	case StateBK::Slash:  slash(delta_time);  break;
	case StateBK::Damage: damage(delta_time); break;
	case StateBK::Dead:   dead(delta_time);   break;
	case StateBK::Crack:  crack(delta_time);  break;
	}
}

void BlackKnight::move(float delta_time)
{
	//プレイヤーが存在しなかったら棒立ち状態
	Actor* player = find_player();
	if (!player) {
		change_state(State::Move, Motion_Idle);
		return;
	}

	unsigned int motion = Motion_Idle;
	Vector3 velocity = Vector3::ZERO;

	//プレイヤーとの距離
	float distance = get_vec_to_player().Length();
	
	//距離により状態
	if (distance <= EscapeRadius) {
		velocity = make_distance();
		motion = Motion_WalkBackward;
	}
	else if (distance <= AttackRadius) {
		//攻撃状態に遷移
		change_state(State::Attack, Motion_Attack02, false);
		return;
	}
	else if (distance <= DetectionRadius) {  //移動状態
		velocity = make_approach();
		motion = Motion_WalkForward;
		//一定時間移動状態が続いたらタックル
		if (has_elapsed(3.0f)) {
			change_state(StateBK::Tackle, Motion_Attack03, false);
			return;
		}
	}
	else {
		if (has_elapsed(1.0f)) {
			//プレイヤーへ向き直る
			make_approach();
			//地割れ攻撃
			change_state(StateBK::Crack, Motion_Attack02, false);
			return;
		}
	}

	velocity_ = velocity;
	position_ += velocity_ * delta_time;
	change_motion(motion);
}

void BlackKnight::attack(float delta_time)
{
	if (can_generate_attack(1.0f)) {
		sound_.play_SE(SE_BlackKnightSwingDown);
		Vector3 pos_attack = position_ + forward() * 300.0f;
		generate_attack(Sphere{ 150.0f, pos_attack }, name_ + "Attack", 0.5f);

	}
	if (is_motion_end()) {
		flinch_count_ = 0;
		change_state(State::Move, Motion_Idle);
	}
}

void BlackKnight::slash(float delta_time)
{
	if (can_generate_attack(0.6f)) {
		sound_.play_SE(SE_BlackKnightSlash);
		Vector3 pos_attack = position_ + forward() * 300.0f;
		generate_attack(Sphere{ 150.0f, pos_attack }, name_ + "Attack", 0.2f);

	}
	if (is_motion_end()) {
		flinch_count_ = 0;
		change_state(State::Move, Motion_Idle);
	}
}

void BlackKnight::tackle(float delta_time)
{
	if (can_generate_attack(0.6f)) {
		sound_.play_SE(SE_BlackKnightTackle);
		Vector3 pos_attack = position_ + forward() * 500.0f;
		generate_attack(Sphere{ 250.0f, pos_attack }, name_ + "Attack", 0.3f);
	}

	if (is_motion_end()) {
		change_state(State::Move, Motion_Idle);
	}
}

void BlackKnight::damage(float delta_time)
{
	if (is_motion_end()) {
		make_approach();
		change_state(StateBK::Slash, Motion_Attack01, false);
	}
}

void BlackKnight::crack(float delta_time)
{
	if (has_excessed(1.0f)) {
		sound_.play_SE(SE_BlackKnightSwingDown);
		world_->add_actor(new CrackGenerator{ *world_, position(), forward() });
	}

	if (is_motion_end()) {
		change_state(StateBK::Move, Motion_Idle);
	}
}


void BlackKnight::draw_debug() const
{
	//ForDebug:
	static const int blue = DxLib::GetColor(0, 0, 255);
	static const int yellow = DxLib::GetColor(255, 255, 0);
	static const int red = DxLib::GetColor(255, 0, 0);
	DrawSphere3D(DxConverter::GetVECTOR(position_), EscapeRadius, 4, blue, blue, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), DetectionRadius, 4, yellow, yellow, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), AttackRadius, 4, red, red, false);
	DxLib::DrawFormatString(0, 20, DxLib::GetColor(255, 255, 255), "blackKnight_hp:%d", parameter_.hp);
	DxLib::DrawFormatString(0, 40, DxLib::GetColor(255, 255, 255), "blackKnight_flince:%d", flinch_count_);

	Vector3 hp_gauge_pos = position_ + Vector3{ 0.0f,400.0f, 0.0f };
	//DxLib::DrawBillboard3D(DxConverter::GetVECTOR(hp_gauge_pos), 0.5f, 0.5f, 200.0f, 0.0f, Image::texture_handle(Texture_GaugeBarGreen), true);

}
