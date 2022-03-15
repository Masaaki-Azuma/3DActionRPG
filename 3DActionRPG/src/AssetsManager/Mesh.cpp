#include "Mesh.h"

#include <cassert>
#include <DxLib.h>

static std::vector<int> mesh_id_list(256, -1);

void Mesh::load(const std::string& scene)
{
	//HACK:loadを繰り返すと正常な読み込みができなくなるので、応急処置的に全削除
	DxLib::MV1InitModel();

	load_a_mesh("Assets/Mesh/Stage/stage_collider.mv1", Mesh_StageCollider, scene, {"BattleScene"});
	load_a_mesh("Assets/Mesh/Stage/StaticObject/ground_objects.mv1", Mesh_StageMesh, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/Stage/skybox.mv1", Mesh_Skybox, scene, { "BattleScene" });

	load_a_mesh("Assets/Mesh/Player/Swordman.mv1", Mesh_Player, scene, { "BattleScene" });

	load_a_mesh("Assets/Mesh/Slime/Slime.mv1", Mesh_Slime, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/Skeleton/Skeleton.mv1", Mesh_Skeleton, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/Mage/Mage.mv1", Mesh_Mage, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/Mimic/Mimic.mv1", Mesh_Mimic, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/Mimic/Jewel.mv1", Mesh_Jewel, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/BlackKnight/BlackKnight.mv1", Mesh_BlackKnight, scene, { "BattleScene" });
	load_a_mesh("Assets/Mesh/BlackKnight/Crack.mv1", Mesh_Crack, scene, { "BattleScene" });
}

void Mesh::clear()
{
	for (auto& handle : mesh_id_list) {
		DxLib::MV1DeleteModel(handle);
		handle = -1;
	}
}

int Mesh::mesh_handle(int mesh_id) const
{
	return mesh_id_list[mesh_id];
}

int Mesh::load_a_mesh(const char* file_name, int mesh_id, const std::string& scene, std::vector<std::string> use_scenes)
{
	/*ロードが必要なシーンか判定*/
	auto scene_itr = std::find(use_scenes.begin(), use_scenes.end(), scene);
	if (scene_itr == use_scenes.end()) return -1; //読み込むシーンではない

	if (mesh_id_list[mesh_id] != -1) return mesh_id_list[mesh_id]; //既に読み込み済みなのでパス

	/*実際のロード処理*/
	int handle = DxLib::MV1LoadModel(file_name);
	//画像が正しく読み込まれていなければ停止
	assert(handle != -1 && "メッシュのロードに失敗しました");
	//ハンドル配列メモリが不足したら2倍に拡張
	if (mesh_id >= static_cast<int>(mesh_id_list.size())) {
		mesh_id_list.resize(mesh_id_list.size() * 2);
	}
	//画像IDをインデックスとして画像ハンドルを格納
	mesh_id_list[mesh_id] = handle;
	return handle;
}

