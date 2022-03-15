#include "Enemy.h"

#include "Util/MyMath.h"
#include "Util/DxConverter.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/Image.h"
#include "BattleScene/IWorld.h"
#include "Actor/AttackCollider.h"

Enemy::Enemy(IWorld* world, const Vector3& position, const Vector3& rotation)
{
	world_ = world;
	tag_ = "EnemyTag";
	position_ = position;
	rotation_ = rotation;
}

Enemy::‾Enemy()
{
	//HACK:モデルハンドルの複製場所はSkinningMeshコンストラクタだがわかりづらくないか？
	mesh_.clear();
}

void Enemy::update(float delta_time)
{
	//
	motion_interruption = false;

	//現在の状態を更新
	update_state(delta_time);
	//状態タイマーを更新
	update_state_timer(delta_time);
	//壁との押し出し処理
	react_wall();

	//メッシュの更新
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.update(delta_time);
}

void Enemy::draw() const
{
	//メッシュ描画
	mesh_.draw();
	//残り体力描画
	draw_hp_gauge();

	//collider().draw();
	//draw_debug();
}

void Enemy::react(Actor& other)
{
	react_player_attack(other);
	if (other.tag() == "EnemyTag" || other.tag() == "PlayerTag") {
		collide_actor(other);
	}
}

void Enemy::react_player_attack(Actor& other){}

void Enemy::change_state(unsigned int state, unsigned int motion, bool loop, bool is_interruptive)
{
	state_ = state;
	motion_ = motion;
	motion_loop_ = loop;
	motion_interruption = is_interruptive;
	state_timer_ = 0.0f;
	has_attacked_ = false;
}

void Enemy::change_motion(unsigned int motion, bool loop)
{
	motion_ = motion;
	motion_loop_ = loop;
}

void Enemy::update_state_timer(float delta_time)
{
	prev_state_timer_ = state_timer_;
	state_timer_ += delta_time;
}

void Enemy::dead(float delta_time)
{
	if (is_motion_end()) {
		//敵討伐数を加算
		world_->add_basterd(name_);
		die();
	}
}

void Enemy::draw_hp_gauge() const
{
	static const float GaugeHeight = 10.0f;
	static const float GaugeWidth = 100.0f;
	Vector3 pos = collider().center + Vector3{ 0.0f, collider().radius * 2, 0.0f };
	
	float current_hp_rate = static_cast<float>(parameter_.hp) / e_DB_.get_parameter(name_).hp;
	DxLib::SetUseZBuffer3D(FALSE);
	DxLib::DrawModiBillboard3D(DxConverter::GetVECTOR(pos),
		-GaugeWidth / 2,  GaugeHeight / 2,
		-GaugeWidth / 2 + GaugeWidth * current_hp_rate,  GaugeHeight / 2,
		-GaugeWidth / 2 + GaugeWidth * current_hp_rate, -GaugeHeight / 2,
		-GaugeWidth / 2, -GaugeHeight / 2,
		Image::texture_handle(Texture_GaugeBarRed), TRUE);
	DxLib::SetUseZBuffer3D(TRUE);
}

std::shared_ptr<Actor> Enemy::find_player()
{
	return world_->find_actor("Player");
}

Vector3 Enemy::get_vec_to_player()
{
	std::shared_ptr<Actor> player = find_player();
	if (!player) return Vector3::ZERO;
	Vector3 vec = player->position() - position();
	vec.y = 0.0f;
	return vec;
}

Vector3 Enemy::make_distance()
{
	Vector3 direction = get_vec_to_player();
	Vector3 velocity = -direction.Normalize() * move_speed_;
	rotation_.y = degree_forward(direction);
	return velocity;
}

float Enemy::degree_forward(const Vector3& forward)
{
	return Vector3::SignedAngleY(Vector3::FORWARD, forward) * MyMath::Rad2Deg;
}

Vector3 Enemy::make_approach()
{
	Vector3 direction = get_vec_to_player();
	Vector3 velocity = direction.Normalize() * move_speed_;
	rotation_.y = degree_forward(direction);
	return velocity;
}

void Enemy::generate_attack(const Sphere& collider, const std::string& name, float lifespan, float delay)
{
	world_->add_actor(std::make_shared<AttackCollider>(world_, collider, "EnemyAttackTag", name, "EnemyTag", lifespan, delay));
	has_attacked_ = true;
}

void Enemy::take_damage(int damage)
{
	parameter_.hp = (std::max)(parameter_.hp - damage, 0);
	sound_.play_SE(SE_MonsterDamage);
}

bool Enemy::is_motion_end() const
{
	return state_timer_ >= mesh_.anim_total_sec();
}

bool Enemy::can_generate_attack(float time) const
{
	//モーション開始から指定時間が過ぎている && まだ攻撃判定を生成していない
	return state_timer_ >= time && !has_attacked_;
}

bool Enemy::has_elapsed(float time)
{
	return state_timer_ >= time;
}

bool Enemy::has_excessed(float time)
{
	return state_timer_ >= time && prev_state_timer_ < time;
}

