#include "Mesh.h"

#include <cassert>
#include <DxLib.h>


int Mesh::test_handle{ -1 };
int Mesh::slime_handle{ -1 };
int Mesh::skeleton_handle{ -1 };
int Mesh::mimic_handle{ -1 };
int Mesh::black_knight_handle{ -1 };
int Mesh::stage_handle{ -1 };
int Mesh::skybox{ -1 };

void Mesh::load()
{
	MV1InitModel();
	test_handle = load_a_mesh("Assets/Mesh/Player/Swordman.mv1");
	//test_handle = load_a_mesh("Assets/Mesh/Player/Swordman_sword.mv1");
	slime_handle = load_a_mesh("Assets/Mesh/Slime/Slime.mv1");
	skeleton_handle = load_a_mesh("Assets/Mesh/Skeleton/Skeleton.mv1");
	mimic_handle = load_a_mesh("Assets/Mesh/Mimic/Mimic.mv1");
	black_knight_handle = load_a_mesh("Assets/Mesh/BlackKnight/BlackKnight.mv1");

	stage_handle = load_a_mesh("Assets/Mesh/Stage/stage_collider.mv1");
	skybox = load_a_mesh("Assets/Mesh/Stage/skybox.mv1");
}

void Mesh::clear()
{
	clear_a_mesh(test_handle);
	clear_a_mesh(slime_handle);
	clear_a_mesh(skeleton_handle);
	clear_a_mesh(mimic_handle);
	clear_a_mesh(black_knight_handle);
	clear_a_mesh(stage_handle);
	clear_a_mesh(skybox);
}

int Mesh::load_a_mesh(const char* file_name)
{
	int handle = MV1LoadModel(file_name);
	assert(handle != -1 && "メッシュのロードに失敗しました");
	return handle;
}

void Mesh::clear_a_mesh(int& handle)
{
	MV1DeleteModel(handle);
	handle = -1;
}
