#ifndef AREA_NODE_H_
#define AREA_NODE_H_

#include <vector>
#include <string>
#include <memory>
#include "Util/Vector3.h"


class AreaNode
{
public:
using PtrNode = std::shared_ptr<AreaNode>;
using NextNodeList = std::vector<PtrNode>;
public:
	AreaNode(const Vector3& position, const std::string& enemy);
	‾AreaNode() = default;
	//ノード描画
	void draw();
	NextNodeList& next();
	void add_next(PtrNode next);
	//ノードの描画位置を取得
	const Vector3& position() const;
	//ノードに配置された敵名を取得
	std::string& enemy();

private:
	NextNodeList next_;
	int node_depth_{ -1 };            //ノードの深さ
	Vector3 position_;                //ノードの描画位置
	std::string enemy_;               //配置される敵グループ名
	int enemy_image_id_{ -1 };        //ノード上に表示するテクスチャID
};
#endif//!AREA_NODE_H_
