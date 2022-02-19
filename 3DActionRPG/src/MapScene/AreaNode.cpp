#include "AreaNode.h"
#include <unordered_map>
#include <DxLib.h>
#include "AssetsManager/Image.h"

std::unordered_map<std::string, int> enemy_image_table{
	{"Slime", Texture_icon_slime},
	{"Skeleton", Texture_icon_skeleton},
	{"Mage", Texture_icon_mage},
	{"Mimic", Texture_icon_mimic},
	{"BlackKnight", Texture_icon_blackKnight},
};

AreaNode::AreaNode(const Vector3& position, const std::string& enemy):
	position_{ position },
	enemy_{ enemy }{
}

void AreaNode::draw()
{
	//エリア円表示
	Image::draw_rota_graph(Texture_mapArea, position_.x, position_.y);
	//敵が存在するときはモンスター表示
	if(enemy_image_table.count(enemy_) != 0) Image::draw_rota_graph(enemy_image_table[enemy_], position_.x, position_.y);
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

