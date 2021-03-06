#include "MapManager.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <DxLib.h>


#include "AreaNode.h"
#include "NullNode.h"
#include "Util/MyRandom.h"
#include "Util/PadInput.h"
#include "Util/CsvReader.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"
#include "AssetsManager/PlayerDatabase.h"

static std::shared_ptr<NullNode> null_node{ std::make_shared<NullNode>()}; //Nullノード
static const int MaxDepth{ 6 };                              //マップ上のノードの列数
static const int NodeNumList[MaxDepth]{ 1, 3, 4, 3, 2, 1 };  //深さごとのノード数
static const float AreaHorizontalInterval{ 300.0f };         //エリア間の幅
static const float AreaVerticalInterval{ 200.0f };           //エリア間の高さ
static const Vector3 StartPosition{ 200.0f, 600.0f };        //スタートノードの位置
static const int RoadOffset{ 50 };                           //エリア間の道描画用のオフセット
static const int NumEnemySpecies{ 5 };                       //敵の種類数

void MapManager::update(float delta_time)
{
	//エリア更新
	update_areas(delta_time);
	//エリア選択
	if (!is_picked()) pick_area();
	//遭遇テキスト更新
	encount_text_.update(delta_time);
	//エリア公開後敵名表示
	if (is_appeared() && encount_text_.is_wait()) {
		if (current_area_node_->enemy() == "Chest") {
			Sound::GetInstance().play_SE(SE_Heal);
			get_portion();
			enter_map();
			return;
		}

		encount_text_ = SlideInAnimation{ "VS　" + current_area_node_->enemy(), Font::japanese_font_120_edge, 120, DxLib::GetColor(207, 205, 175), 40.0f, 300.0f };
		encount_text_.start();
		Sound::GetInstance().play_SE(SE_Encount);
	}
	
}

void MapManager::draw()
{
	//背景描画
	Image::GetInstance().draw_graph(Texture_background_oldmap);
	//全エリアを描画
	draw_areas();
	//カーソルを描画
	draw_cursor();
	//現在位置描画
	 draw_player();
	//遭遇モンスター名表示
	encount_text_.draw();
	if (current_area_node_->enemy() != "Null") return;
	//操作誘導テキストを描画
	draw_instruction();
}


void MapManager::generate()
{
	//全ノードを個別に生成
	generate_nodes();
	//生成したノードをつなげる
	link_nodes();
	//スタート地点を現在地に設定
	set_start_node();
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
	for (int row = 0; row < table.rows(); ++row) {
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
	//敵生成用のランダム値を100％基準で生成
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
	PadInput& input = PadInput::GetInstance();
	const int num_next_node = prev_area_node_->next().size();
	//決定ボタンでエリアを決定
	if (input.GetButtonDown(XINPUT_BUTTON_B)) {
		if (prev_area_node_->next().empty()) return;
		Sound::GetInstance().play_SE(SE_Select);
		//選択エリアを現在地に更新
		current_area_node_ = prev_area_node_->next().at(area_index_);
		//シルエットを公開
		current_area_node_->appear();
		//選択済みに
		is_picked_ = true;
	}
	//上下ボタンでエリアを選択
	if (input.GetButtonDown(XINPUT_BUTTON_DPAD_UP)) {
		Sound::GetInstance().play_SE(SE_CursorMove);
		area_index_ = (area_index_ + num_next_node - 1) % num_next_node;
	}
	if (input.GetButtonDown(XINPUT_BUTTON_DPAD_DOWN)) {
		Sound::GetInstance().play_SE(SE_CursorMove);
		area_index_ = (area_index_ + 1) % num_next_node;
	}
}

bool MapManager::is_picked()
{
	return is_picked_;
}

bool MapManager::is_appeared() const
{
	return current_area_node_->is_appeared();
}

bool MapManager::is_final_area()
{
	//次のエリアリストが空なら最終エリアと判断
	return prev_area_node_->next().empty();
}

bool MapManager::is_end()
{
	return is_picked() && is_appeared() && encount_text_.is_end();
}

//マップシーンに入った時の処理
void MapManager::enter_map()
{
	//エリア選択履歴を更新
	prev_area_node_ = current_area_node_;
	//選択ノードを空に
	current_area_node_ = null_node;
	//選択状態を初期化
	is_picked_ = false;
	area_index_ = 0;
	//遭遇テキストをリセット
	encount_text_.reset();
}

std::string& MapManager::selected_enemy()
{
	//現在エリアにいる敵の名前を取得
	return current_area_node_->enemy();
}

void MapManager::update_areas(float delta_time)
{
	//浅いエリアから上から順に描画
	for (auto& depth : node_list_) {
		for (auto& node : depth) {
			//エリア本体を描画
			node->update(delta_time);
		}
	}
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
	CsvReader node_relation{ "Assets/MapData/node_relation.csv" };
	int current_row = 0;

	//各ノードが繋がる次のノードを登録
	while (current_row < node_relation.rows()) {
		/*  "node", <nodeの横位置>, <nodeの縦位置>  */
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

void MapManager::set_start_node()
{
	//最初のノードを現在地に設定
	current_area_node_ = node_list_[0][0];
	prev_area_node_ = current_area_node_;
}

void MapManager::calc_possibility_per_depth(int depth, const CsvReader& table)
{
	//累積確率
	int possibility_accumulation = 0;
	for (int row = 0; row < table.rows(); ++row) {
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

void MapManager::draw_areas()
{
	//浅いエリアから上から順に描画
	for (auto& depth : node_list_) {
		for (auto& node : depth) {
			Vector3 pos = node->position();
			//次のエリアリスト
			AreaNode::NextNodeList list = node->next();
			//エリアをつなぐ道の描画
			for (auto& np : list) {
				static const int road_color = DxLib::GetColor(79, 58, 29);
				DxLib::DrawLineAA(pos.x, pos.y + RoadOffset, np->position().x, np->position().y + RoadOffset, road_color, 8);
			}
			//エリア本体を描画
			node->draw();
		}
	}
}

void MapManager::draw_player()
{
	Vector3 position = prev_area_node_->position();
	Image::GetInstance().draw_rota_graph(Texture_icon_player, position.x, position.y);
}

void MapManager::draw_cursor()
{
	//選択箇所をアイコンで示す
	if (prev_area_node_->next().empty()) return; //ゴールなら描画の必要なし
	Vector3 pos_next_area = prev_area_node_->next().at(area_index_)->position();
	Image::GetInstance().draw_rota_graph(Texture_cursor, pos_next_area.x, pos_next_area.y - 60.0f, 0.6f);
}

void MapManager::draw_instruction()
{
	Font::draw(1000, 100, "次に進むエリアを選んでください", DxLib::GetColor(0, 0, 0), Font::japanese_font_35);
}

void MapManager::get_portion()
{
	PlayerDatabase& p_DB = PlayerDatabase::GetInstance();
	//回復
	p_DB.add_hp(500);
}
