#ifndef MESH_H_
#define MESH_H_

#include "Singleton.h"

#include <vector>
#include <string>

enum
{
	Mesh_StageCollider,
	Mesh_StageMesh,
	Mesh_Skybox,

	Mesh_Player,

	Mesh_Slime,
	Mesh_Skeleton,
	Mesh_Mage,
	Mesh_BlackKnight,
	Mesh_Crack,
	Mesh_Mimic,
	Mesh_Jewel,
};

//3Dメッシュ管理クラス
class Mesh : public Singleton<Mesh>
{
public:
	void load(const std::string& scene);
	void clear();
	int mesh_handle(int mesh_id) const;
private:
	int load_a_mesh(const char* file_name, int mesh_id, const std::string& scene, std::vector<std::string> use_scenes);

private:
	Mesh() = default;
	‾Mesh() = default;

	friend class Singleton<Mesh>;
};
#endif//!MESH_H_

