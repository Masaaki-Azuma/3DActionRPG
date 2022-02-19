#include "MapManager.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <DxLib.h>

#include "AreaNode.h"
#include "Util/MyRandom.h"
#include "Util/Input.h"
#include "Util/CsvReader.h"
#include "AssetsManager/Image.h"


const int MaxDepth{ 6 };                              //マップ上のノードの列数
const float AreaHorizontalInterval{ 300.0f };         //エリア間の幅
const float AreaVerticalInterval{ 200.0f };           //エリア間の高さ
const Vector3 StartPosition{ 200.0f, 600.0f };          //スタートノードの位置
const int NodeNumList[MaxDepth]{ 1, 3, 4, 3, 2, 1 };  //深さごとのノード数
const int NumEnemySpecies{ 5 }; //敵の種類数

//ForDebug:関数staticを一時的にグローバルに
static int area_index = 0;

void MapManager::update(float delta_time)
{
	pick_area();
}

void MapManager::draw()
{
	//背景描画
	Image::draw_graph(Texture_background_oldmap);

	//浅いノードから上から順に描画
	for (auto& depth : node_list_) {
		for (auto& node : depth) {
			node->draw();
			//ForDebug:ノードの繋がりの確認
			Vector3 pos = node->position();
			AreaNode::NextNodeList list = node->next();
			for (auto& np : list) {
				DxLib::DrawLineAA(pos.x, pos.y, np->position().x, np->position().y, GetColor(0, 255, 0));
			}
		}
	}
	//ForDebug:選択エリアを単純描画
	DxLib::DrawFormatString(0, 0, GetColor(255, 255, 255), "current_selected_index: %d", area_index);
	DxLib::DrawCircleAA(current_area_node_->position().x, current_area_node_->position().y, 20, 8, GetColor(0, 255, 0));
	DxLib::DrawCircleAA(prev_area_node_->position().x, prev_area_node_->position().y, 20, 8, GetColor(255, 0, 0));
}


void MapManager::generate()
{
	//全ノードを個別に生成
	generate_nodes();
	//生成したノードをつなげる
	link_nodes();

	//ForDebug:最初のノードを現在地に設定
	current_area_node_ = node_list_[0][0];
	prev_area_node_ = current_area_node_;
}

void MapManager::clear()
{
	node_list_.clear();
}

/*
* 
* 元データ                         　　　　変換後データ（同列の0でない直上の値+自分の確率を収納）
* Slime      , 70, 58, 20,  0,   0      　  Slime      ,  70,  58,  20,   0,   0
* Skeleton   , 30, 40, 50, 55,   0     　   Skeleton   , 100,  98,  70,  55,   0
* Mage       ,  0,  0, 25, 40,   0   →     Mage       ,   0,   0,  95,  95,   0
* Mimic      ,  0,  2,  5,  5,   0       　 Mimic      ,   0, 100, 100, 100,   0
* BlackKnight,  0,  0,  0,  0, 100        　BlackKnight,   0,   0,   0,   0, 100
*
*/

void MapManager::load_enemy_possibility_table(const std::string& difficulty)
{
	//元の確率表を読み込み
	CsvReader table{ "Assets/MapData/enemy_possibility_table.csv" };

	//全敵種族を変換後確率表に登録
	for (int row = 0; row < NumEnemySpecies; ++row) {
		std::pair<std::string, std::vector<int>> possibility;
		//種族名を取得
		possibility.first = table.get(row, 0);
		enemy_possibility_table_.emplace_back(possibility);
	}

	for (int depth = 1; depth < MaxDepth; ++depth) {
		//各エリア深さごとに累積確率を算出
		calc_possibility_per_depth(depth, table);
	}
}

const std::string MapManager::pick_enemy(int depth)
{
	//スタート地点ならNone
	if (depth == 0) return "None";

	float rand_num = MyRandom::Range(0.0f, 100.0f);
	std::string enemy_name = "None"; //スタート地点を表すNone
	//ある深さでの敵出現率とランダム値を順に比較
	for (size_t e_i = 0; e_i < enemy_possibility_table_.size(); ++e_i) {
		if (rand_num <= enemy_possibility_table_[e_i].second[depth - 1]) { //depth: 1〜Maxdepth-1 → 0〜Maxdepth - 2
			//出現する敵名を返す
			enemy_name = enemy_possibility_table_[e_i].first;
			break;
		}
	}
	return enemy_name;
}

void MapManager::pick_area()
{
	//static int area_index{ 0 };
	const int num_next_node = prev_area_node_->next().size();
	if (Input::get_button_down(PAD_INPUT_1)) {
		if (prev_area_node_->next().empty()) return;
		current_area_node_ = prev_area_node_->next().at(area_index);
		area_index = 0;
		is_picked_ = true;
	}

	if (Input::get_button_down(PAD_INPUT_UP)) {
		area_index = (area_index + num_next_node - 1) % num_next_node;
	}
	if (Input::get_button_down(PAD_INPUT_DOWN)) {
		area_index = (area_index + 1) % num_next_node;
	}
}

bool MapManager::is_picked()
{
	return is_picked_;
}

bool MapManager::is_final_area()
{
	return prev_area_node_->next().empty();
}

void MapManager::make_node_old()
{
	prev_area_node_ = current_area_node_;
	is_picked_ = false;
}

std::string& MapManager::selected_enemy()
{
	return current_area_node_->enemy();
}

void MapManager::generate_nodes()
{
	//難易度に応じた確率表を読み込む
	load_enemy_possibility_table("");
	//ノードの深さ分のメモリを一度に確保
	node_list_.resize(MaxDepth);
	//全ノードを作成
	for (int depth = 0; depth < MaxDepth; ++depth) {
		//同じ深さにあるノードの数
		const int NodeNum = NodeNumList[depth];
		//深さが同じ全ノードを作成
		for (int i = 0; i < NodeNum; ++i) {
			//ノードの位置
			Vector3 position = StartPosition + Vector3{
				depth * AreaHorizontalInterval,                                                              //深さに応じた横位置
				((NodeNum + 1) % 2) * AreaVerticalInterval / 2 + (i - NodeNum / 2) * AreaVerticalInterval }; //ノード数が偶数の場合の縦オフセット + 同じ深さにおけるノードの順番による縦位置
			node_list_[depth].push_back(std::make_shared<AreaNode>(position, pick_enemy(depth)));
		}
	}
}

void MapManager::link_nodes()
{
	CsvReader node_relation{ "Assets/MapData/node_relation.csv" }; //node,<nodeの横位置>,<nodeの縦位置>
	int current_row = 0;

	//各ノードが繋がる次のノードを登録
	while (current_row < node_relation.rows()) {
		/*  "node", <nodeの横位置>, <nodeの縦位置>  */
		//ForDebug
		assert(node_relation.get(current_row, 0) == "node" && "意図しない行を読み込みました");
		//扱うノードの番号を取得
		int node_depth, node_index;
		node_depth = node_relation.geti(current_row, 1);
		node_index = node_relation.geti(current_row, 2);
		//次の行へ
		current_row++;

		/*  次につながるノードの縦位置, ....*/
		for (int col = 0; col < node_relation.columns(current_row); ++col) {
			//値が空なら、次のノードの追加が終了したのでループを抜ける
			if (node_relation.get(current_row, col) == "") break;

			int next_node_index = -1;
			next_node_index = node_relation.geti(current_row, col);
			node_list_[node_depth][node_index]->add_next(node_list_[node_depth + 1][next_node_index]);
		}

		//次の行へ
		current_row++;
	}

}

void MapManager::calc_possibility_per_depth(int depth, const CsvReader& table)
{
	//累積確率
	int possibility_accumulation = 0;
	for (int row = 0; row < NumEnemySpecies; ++row) {
		//ある敵の出現確率
		int possibility = table.geti(row, depth);
		//確率0のばあい0を入れる
		if (possibility == 0) {
			enemy_possibility_table_[row].second.emplace_back(0);
			continue;
		}
		//確率を累積
		possibility_accumulation += possibility;
		enemy_possibility_table_[row].second.emplace_back(possibility_accumulation);
	}
	assert(possibility_accumulation == 100 && "確率の合計が100％になりません。元データの数値を確認してください");
}
