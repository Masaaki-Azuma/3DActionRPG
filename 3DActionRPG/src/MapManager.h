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
	void draw();
	void generate();
	void clear();
	void load_enemy_possibility_table(const std::string& difficulty);
	const std::string pick_enemy(int depth);
	void change_area_state(int index, std::string group_name);
	void change_player_area(int index);

private:
	using PtrNode = std::shared_ptr<AreaNode>;
	using NodeInADepth = std::vector<PtrNode>;
	using NodeList = std::vector<NodeInADepth>;
	int change_area_index_{ -1 };
	AreaNode* current_area_node_{ nullptr };
	AreaNode* prev_area_node_{ nullptr };
	NodeList node_list_;  //[ノードの深さ][ノードの上からの順番]で表されるノードリスト
	std::vector<std::pair<std::string, int>> enemy_possibility_table_;

private:
	MapManager() = default;
	‾MapManager() = default;
private:
	friend class Singleton<MapManager>;
};
#endif//!MAP_MANAGER_H_

