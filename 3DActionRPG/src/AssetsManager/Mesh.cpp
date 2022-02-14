#include "Mesh.h"

#include <cassert>
#include <DxLib.h>


int Mesh::test_handle{ -1 };
int Mesh::slime_handle{ -1 };
int Mesh::chest_handle{ -1 };
int Mesh::black_knight_handle{ -1 };
int Mesh::stage_handle{ -1 };
int Mesh::skybox{ -1 };

void Mesh::load()
{
	test_handle = load_a_mesh("Assets/Mesh/Player/Swordman.mv1");
	slime_handle = load_a_mesh("Assets/Mesh/Slime/Slime.mv1");
	chest_handle = load_a_mesh("Assets/Mesh/Chest/Chest.mv1");
	black_knight_handle = load_a_mesh("Assets/Mesh/BlackKnight/BlackKnight.mv1");

	stage_handle = load_a_mesh("Assets/Mesh/Stage/stage_collider.mv1");
	skybox = load_a_mesh("Assets/Mesh/Stage/skybox.mv1");
}

void Mesh::clear()
{
	clear_a_mesh(test_handle);
	clear_a_mesh(stage_handle);
	clear_a_mesh(skybox);
	clear_a_mesh(chest_handle);
	clear_a_mesh(black_knight_handle);
}

int Mesh::load_a_mesh(const char* file_name)
{
	int handle = MV1LoadModel(file_name);
	assert(handle != -1 && "メッシュのロードに失敗しました");
	return handle;
}

void Mesh::clear_a_mesh(int handle)
{
	MV1DeleteModel(handle);
}
