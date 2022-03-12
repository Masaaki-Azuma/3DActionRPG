#ifndef MESH_H_
#define MESH_H_

#include <string>

//3Dメッシュ管理クラス
class Mesh
{
public:
	static void load();
	static void clear();
private:
	static int load_a_mesh(const char* file_name);
	static void clear_a_mesh(int& handle);
public:
	static int stage_collider_handle;
	static int ground_handle;
	static int skybox_handle;

	static int player_handle;

	static int slime_handle;
	static int skeleton_handle;
	static int mage_handle;
	static int black_knight_handle;
	static int crack_handle;
	static int mimic_handle;
	static int jewel_handle;
};
#endif//!MESH_H_

