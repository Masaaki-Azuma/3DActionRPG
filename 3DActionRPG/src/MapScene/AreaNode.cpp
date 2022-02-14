#include "AreaNode.h"
#include <unordered_map>
#include <DxLib.h>
#include "AssetsManager/Image.h"

std::unordered_map<std::string, int> enemy_image_table{
	{"Slime", Texture_test00},
	{"Mimic", Texture_test01},
	{"BlackKnight", Texture_test02},
	{"dragon", Texture_test03}
};

AreaNode::AreaNode(const Vector3& position, const std::string& enemy):
	position_{ position },
	enemy_{ enemy }{
}

void AreaNode::draw()
{
	DxLib::DrawRotaGraphF(position_.x, position_.y, 1.0f, 0.0f, Image::texture_handle(enemy_image_table[enemy_]), true);
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
