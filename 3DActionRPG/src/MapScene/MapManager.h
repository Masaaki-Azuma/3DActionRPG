#ifndef MAP_MANAGER_H_
#define MAP_MANAGER_H_

#include "Singleton.h"

#include <string>
#include <vector>
#include <memory>   //shared_ptr
#include <utility>  //pair

#include "Util/Vector3.h"

class AreaNode;
class CsvReader;

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

	//エリアを選択
	void pick_area();
	//選択エリアが決定されたか？
	bool is_picked();
	//最終エリアをクリア後か？
	bool is_final_area();

	//HACK:これはバトルシーンから戻ってきたときに呼び出される関数であるので、適切な名前に変更せよ
	//前回選択したエリアを過去のエリアへ変更
	void enter_map();
	//選択エリアの敵の名前を取得
	std::string& selected_enemy();

private:
	//エリアの更新
	void update_areas(float delta_time);
	//ノードを生成
	void generate_nodes();
	//ノードを連結
	void link_nodes();
	//スタートエリアを設定
	void set_start_node();
	//エリア深さごとの累積確率を算出
	void calc_possibility_per_depth(int depth, const CsvReader& table);
	//全エリアを描画
	void draw_areas();
	//選択箇所をカーソルで示す
	void draw_cursor();

private:
	//エリアノードのスマートポインタ
	using PtrNode = std::shared_ptr<AreaNode>;
	//同一深さにあるノード配列
	using NodesInADepth = std::vector<PtrNode>;
	//全ノード
	using NodeList = std::vector<NodesInADepth>;

	//選択ノード
	PtrNode current_area_node_{ nullptr };
	//前回選択ノード
	PtrNode prev_area_node_{ nullptr };
	//全ノード配列
	NodeList node_list_;  //[ノードの深さ][ノードの上からの順番]で表されるノードリスト
	//敵と各深さのエリアでの出現率表
	std::vector<std::pair<std::string, std::vector<int>>> enemy_possibility_table_; //<"敵の名前", "ある敵の出現率配列">
	bool is_picked_{ false };

	//選択中のエリア番号
	int area_index_;

private:
	MapManager() = default;
	‾MapManager() = default;
private:
	friend class Singleton<MapManager>;
};
#endif//!MAP_MANAGER_H_

