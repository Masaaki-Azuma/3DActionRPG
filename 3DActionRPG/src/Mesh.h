#pragma once

#include <string>

//3Dメッシュ管理クラス
class Mesh
{
public:
	static void load();
	static void clear();
private:
	static int load_a_mesh(const char* file_name);
	static void clear_a_mesh(int handle);
public:
	static int test_handle;
	static int stage_handle;
	static int slime_handle;
	static int skybox;
};

