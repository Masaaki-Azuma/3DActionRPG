#include "AreaNode.h"

#include <unordered_map>
#include <DxLib.h>

#include "AssetsManager/Image.h"
#include "AssetsManager/EnemyDatabase.h"
#include "Util/Vector2.h"

AreaNode::AreaNode(const Vector3& position, const std::string& enemy):
	position_{ position },
	enemy_{ enemy },
	e_DB_{EnemyDatabase::GetInstance()}
{
	//アニメーションを設定
	icon_anim_ = AppearAnimation{ Vector2{position.x, position.y}, e_DB_.enemy_icon_table(enemy), e_DB_.enemy_silhouette_table(enemy), Vector2{128, 128}, 2.0f };
}

void AreaNode::update(float delta_time)
{
	icon_anim_.update(delta_time);
}

void AreaNode::draw() const
{
	//エリア円表示
	Image::draw_rota_graph(Texture_mapArea, position_.x, position_.y);
	//敵が存在するときはモンスター表示
	if (e_DB_.enemy_icon_table(enemy_) != -1) {
		icon_anim_.draw();
	}
}

void AreaNode::appear()
{
	icon_anim_.appear();
}

bool AreaNode::is_appeared() const
{
	return icon_anim_.is_end();
}

AreaNode::NextNodeList& AreaNode::next()
{
	return next_;
}

void AreaNode::add_next(AreaNode::PtrNode next)
{
	next_.emplace_back(next);
}

const Vector3& AreaNode::position() const
{
	return position_;
}

std::string& AreaNode::enemy()
{
	return enemy_;
}

