#ifndef MAP_MANAGER_H_
#define MAP_MANAGER_H_

#include "Singleton.h"
#include <string>
#include <vector>
#include <memory>   //shared_ptr
#include <utility>  //pair
#include "Util/Vector3.h"

class AreaNode;

class MapManager : public Singleton<MapManager>
{
public:
	//更新
	void update(float delta_time);
	//マップを描画
	void draw();
	//マップをランダム生成
	void generate();
	//マップを削除
	void clear();
	//敵の生成確率表を読み込む
	void load_enemy_possibility_table(const std::string& difficulty);
	//エリアの深さに応じて敵をランダム生成
	const std::string pick_enemy(int depth);

	void change_area_state(int index, std::string group_name);
	void change_player_area(int index);

	//エリアを選択
	void pick_area();
	//選択エリアが決定されたか？
	bool is_picked();
	//最終エリアをクリア後か？
	bool is_final_area();

	//前回選択したエリアを過去のエリアへ変更
	//HACK:これはバトルシーンから戻ってきたときに呼び出される関数であるので、適切な名前に変更せよ
	void make_node_old();
private:
	//ノードを生成
	void generate_nodes();
	//ノードを連結
	void link_nodes();

private:
	using PtrNode = std::shared_ptr<AreaNode>;
	using NodeInADepth = std::vector<PtrNode>;
	using NodeList = std::vector<NodeInADepth>;


	int change_area_index_{ -1 };
	PtrNode current_area_node_{ nullptr };
	PtrNode prev_area_node_{ nullptr };
	NodeList node_list_;  //[ノードの深さ][ノードの上からの順番]で表されるノードリスト
	std::vector<std::pair<std::string, int>> enemy_possibility_table_;
	bool is_picked_{ false };

private:
	MapManager() = default;
	‾MapManager() = default;
private:
	friend class Singleton<MapManager>;
};
#endif//!MAP_MANAGER_H_

