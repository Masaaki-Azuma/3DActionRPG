#include "Skeleton.h"

#include <cassert>

#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "BattleScene/IWorld.h"

enum
{
	Motion_Attack01 = 0,
	Motion_Attack02 = 1,
	Motion_Defense = 2,
	Motion_Guard = 3,
	Motion_Die = 4,
	Motion_Dizzy = 5,
	Motion_Damage = 6,
	Motion_Idle = 7,
	Motion_Run = 9,
	Motion_Taunting = 10,
	Motion_WalkForward= 16,
};

static const float DetectionRadius{ 600.0f }; //プレイヤーを検知する範囲半径
static const float AttackRadius{ 200.0f };     //プレイヤーを攻撃し始める範囲半径
static const float RunSpeed{ 380.0f };       //移動スピード
static const float WalkSpeed{ 150.0f };       //移動スピード

Skeleton::Skeleton(IWorld* world, const Vector3& position, const Vector3& rotation):
	Enemy{ world, position, rotation }
{
	assert(DetectionRadius >= AttackRadius && "プレイヤー感知半径が不正です");

	name_ = "Skeleton";
	move_speed_ = RunSpeed;
	state_ = StateSkeleton::Move;
	collider_ = Sphere{ 60.0f, Vector3{0.0f, 50.0f, 0.0f} };
	motion_ = Motion_Idle;
	parameter_ = e_DB_.get_parameter(name_);

	//メッシュ姿勢初期化
	mesh_ = SkinningMesh{ Mesh::GetInstance().mesh_handle(Mesh_Skeleton), 30.0f };
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);

	se_run_handle_ = DxLib::DuplicateSoundMem(sound_.sound_handle(SE_SkeletonRun));
}

Skeleton::‾Skeleton()
{
	DxLib::DeleteSoundMem(se_run_handle_);
}

void Skeleton::react_player_attack(Actor& other)
{
	if (other.tag() == "PlayerAttackTag") {
		DxLib::StopSoundMem(se_run_handle_);
		int damage_value  = PlayerDatabase::GetInstance().get_current_parameter().attack;
		if (!can_be_flinched() || state_ == StateSkeleton::Defense) {
			sound_.play_SE(SE_SkeletonGuard);
			//ダメージを半減
			damage_value /= 2;
			//ダメージ状態に
			change_state(StateSkeleton::Damage, Motion_Guard, false, true);
		}
		else {
			//ダメージ状態に
			change_state(State::Damage, Motion_Damage, false, true);
		}
		//ひるみ回数を追加
		flinch_count_++;
		//ダメージを受ける
		take_damage(damage_value);
		if (parameter_.hp <= 0) {
			//当たり判定を無効化
			enable_collider_ = false;
			//さらに死亡状態に遷移
			change_state(State::Dead, Motion_Die, false);
		}
		mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	}
}

void Skeleton::update_state(float delta_time)
{
	switch (state_) {
	case StateSkeleton::Move:   move(delta_time);   break;
	case StateSkeleton::Attack: attack(delta_time); break;
	case StateSkeleton::Damage: damage(delta_time); break;
	case StateSkeleton::Dead:   dead(delta_time);   break;
	case StateSkeleton::Defense: defense(delta_time); break;
	case StateSkeleton::Search: search(delta_time); break;
	}
}

void Skeleton::move(float delta_time)
{
	//プレイヤーを検索し、存在しなかったら棒立ち状態
	std::shared_ptr<Actor> player = find_player();
	if (!player) {
		change_motion(Motion_Idle);
		return;
	}
	//移動状態のモーション
	unsigned int motion = Motion_Idle;
	Vector3 velocity = Vector3::ZERO;

	//プレイヤーとの距離
	float distance = get_vec_to_player().Length();
	//距離が一定以下だと状態遷移
	if (distance <= AttackRadius) { //攻撃状態
		DxLib::StopSoundMem(se_run_handle_);
		change_state(State::Attack, Motion_Attack01, false);
		return;
	}
	else if (distance <= DetectionRadius) {  //移動状態
		velocity = make_approach();
		//走りモーションを設定
		motion = Motion_Run;
		//移動SE
		if (!DxLib::CheckSoundMem(se_run_handle_)) {
			DxLib::PlaySoundMem(se_run_handle_, DX_PLAYTYPE_LOOP);
		}
	}
	else if (has_excessed(5.0f)) {
		DxLib::StopSoundMem(se_run_handle_);
		velocity_ = make_approach().Normalize() * WalkSpeed;
		change_state(StateSkeleton::Search, Motion_WalkForward);
		return;
	}

	if(velocity == Vector3::ZERO) DxLib::StopSoundMem(se_run_handle_);
	velocity_ = velocity;
	position_ += velocity_ * delta_time;
	change_motion(motion);
}

void Skeleton::attack(float delta_time)
{
	switch (motion_) {
	case Motion_Attack01:
		if (can_generate_attack(0.2f)) {
			sound_.play_SE(SE_SkeletonAttack);
			//攻撃判定を生成
			generate_attack(Sphere{ 100.0f, position_ + forward() * 150.0f }, "SkeletonAttack", 0.3f);
		}
		break;
	case Motion_Attack02:
		if (can_generate_attack(0.4f)) {
			sound_.play_SE(SE_SkeletonAttack);
			generate_attack(Sphere{ 100.0f, position_ + forward() * 180.0f }, "SkeletonAttack", 0.3f);
		}
		break;
	}

	//最大2コンボまで攻撃
	if (is_motion_end()) {
		flinch_count_ = 0;
		switch (motion_) {
		case Motion_Attack01: change_state(State::Attack, Motion_Attack02);         break;
		case Motion_Attack02: change_state(StateSkeleton::Defense, Motion_Defense); break;
		}
	}
}

void Skeleton::damage(float delta_time)
{
	if (is_motion_end()) {
		if (flinch_count_ >= 7) change_state(State::Attack, Motion_Attack01);
		else if(can_be_flinched()) change_state(State::Move, Motion_Idle);
		else                  change_state(StateSkeleton::Defense, Motion_Defense);
	}
}

void Skeleton::defense(float delta_time)
{
	if (has_elapsed(1.0f)) {
		//連続ひるみ回数をリセット
		flinch_count_ = 0;
		change_state(State::Move, Motion_Idle);
	}
}

void Skeleton::search(float delta_time)
{
	//プレイヤーを見つけるまで歩いて探索
	position_ += velocity_ * delta_time;

	//プレイヤーを検索し、存在しなかったら棒立ち状態
	std::shared_ptr<Actor> player = find_player();
	if (!player) {
		change_motion(Motion_Idle);
		return;
	}

	//一定時間後、向きを変えて再び探索状態へ
	if (has_elapsed(3.0f)) {
		velocity_ = make_approach().Normalize() * WalkSpeed;
		change_state(StateSkeleton::Search, Motion_WalkForward);
	}

	//プレイヤーが近くに来たら走って接近
	float distance = get_vec_to_player().Length();
	if (distance <= DetectionRadius) {
		change_state(State::Move, Motion_Run);
	}
}

//ダメージを受けたらひるむか？
bool Skeleton::can_be_flinched() const
{
	return flinch_count_ < parameter_.max_flinch_count;
}

void Skeleton::draw_debug() const
{
	static const int yellow = DxLib::GetColor(255, 255, 0);
	static const int red = DxLib::GetColor(255, 0, 0);
	DrawSphere3D(DxConverter::GetVECTOR(position_), DetectionRadius, 4, yellow, yellow, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), AttackRadius, 4, red, red, false);
	DxLib::DrawFormatString(0, 20, DxLib::GetColor(255, 255, 255), "skeleton_hp:%d", parameter_.hp);
	DxLib::DrawFormatString(0, 40, DxLib::GetColor(255, 255, 255), "flinch_count: %d", flinch_count_);
	DxLib::DrawFormatString(0, 60, DxLib::GetColor(255, 255, 255), "flinch_count: %d", sound_.check_SE_playing(SE_SkeletonRun));
}
