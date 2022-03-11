#include "Mesh.h"

#include <cassert>
#include <DxLib.h>

int Mesh::stage_collider_handle{ -1 };
int Mesh::ground_handle{ -1 };
int Mesh::skybox_handle{ -1 };

int Mesh::player_handle{ -1 };

int Mesh::slime_handle{ -1 };
int Mesh::skeleton_handle{ -1 };
int Mesh::mage_handle{ -1 };
int Mesh::mimic_handle{ -1 };
int Mesh::jewel_handle{ -1 };
int Mesh::black_knight_handle{ -1 };

void Mesh::load()
{
	//HACK:loadを繰り返すと正常な読み込みができなくなるので、応急処置的に全削除
	DxLib::MV1InitModel();

	stage_collider_handle = load_a_mesh("Assets/Mesh/Stage/stage_collider.mv1");
	ground_handle = load_a_mesh("Assets/Mesh/Stage/StaticObject/ground_objects.mv1");
	skybox_handle = load_a_mesh("Assets/Mesh/Stage/skybox.mv1");

	player_handle = load_a_mesh("Assets/Mesh/Player/Swordman.mv1");

	slime_handle = load_a_mesh("Assets/Mesh/Slime/Slime.mv1");
	skeleton_handle = load_a_mesh("Assets/Mesh/Skeleton/Skeleton.mv1");
	mage_handle = load_a_mesh("Assets/Mesh/Mage/Mage.mv1");
	mimic_handle = load_a_mesh("Assets/Mesh/Mimic/Mimic.mv1");
	jewel_handle = load_a_mesh("Assets/Mesh/Mimic/Jewel.mv1");
	black_knight_handle = load_a_mesh("Assets/Mesh/BlackKnight/BlackKnight.mv1");
}

void Mesh::clear()
{
	clear_a_mesh(stage_collider_handle);
	clear_a_mesh(ground_handle);
	clear_a_mesh(skybox_handle);

	clear_a_mesh(player_handle);

	clear_a_mesh(slime_handle);
	clear_a_mesh(skeleton_handle);
	clear_a_mesh(mage_handle);
	clear_a_mesh(mimic_handle);
	clear_a_mesh(jewel_handle);
	clear_a_mesh(black_knight_handle);
}

int Mesh::load_a_mesh(const char* file_name)
{
	int handle = DxLib::MV1LoadModel(file_name);
	assert(handle != -1 && "メッシュのロードに失敗しました");
	return handle;
}

void Mesh::clear_a_mesh(int& handle)
{
	DxLib::MV1DeleteModel(handle);
	handle = -1;
}
