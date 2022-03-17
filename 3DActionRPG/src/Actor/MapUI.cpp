#include "MapUI.h"
#include "AssetsManager/Image.h"
#include "Util/Matrix4x4.h"
#include "BattleScene/IWorld.h"

//HACK:半径が変わったらどうする？
static const float StageRadius{ 1600.0f };
static const float MapImageRadius{ 150.0f };
static const float ScaleRatio{ MapImageRadius / StageRadius };

MapUI::MapUI(IWorld& world, const Vector3& position)
{
	world_ = &world;
	position_ = position;
}

void MapUI::update(float delta_time)
{
}

void MapUI::draw() const
{
	std::shared_ptr<Actor> player = world_->find_actor("Player");
	std::vector<std::shared_ptr<Actor>> enemies = world_->find_actor_with_tag("EnemyTag");
	std::shared_ptr<Actor> camera = world_->camera();
	float angleY = camera->rotation().y;

	Image& image = Image::GetInstance();
	//マップ円描画
	image.draw_rota_graph(Texture_CircleMap, position().x, position().y);
	//敵描画
	Vector3 player_pos = player->position();
	for (auto& enemy : enemies) {
		Vector3 enemy_pos = enemy->position();
		Vector3 icon_pos = calc_icon_pos(enemy_pos, player_pos, angleY);
		if (is_in_cirlce(icon_pos)) {
			image.draw_rota_graph(Texture_MapIconRed, icon_pos.x, icon_pos.y);
		}
	}
	//プレイヤー描画
	image.draw_rota_graph(Texture_MapIconGreen, position().x, position().y);
}

Vector3 MapUI::calc_icon_pos(const Vector3& target_pos, const Vector3& center_pos, float angleY) const
{
	//プレイヤーからの相対座標
	Vector3 diff_vec = target_pos - center_pos;
	//マップ用にスケーリング
	diff_vec = diff_vec * ScaleRatio;
	//カメラに対応した回転
	diff_vec = diff_vec * Matrix4x4::rotateY(-angleY);
	//UI上での描画位置
	Vector3 icon_pos = position() + Vector3{ diff_vec.x, -diff_vec.z };
	return icon_pos;
}

bool MapUI::is_in_cirlce(const Vector3& icon_pos) const
{
	float length = (icon_pos - position()).Length();
	return length <= MapImageRadius;
}

