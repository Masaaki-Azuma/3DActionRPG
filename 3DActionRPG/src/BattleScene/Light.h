#ifndef LIGHT_H_
#define LIGHT_H_

#include <vector>

class Vector3;

class Light
{
public:
	Light();
	//ディレクショナルライトの追加（上限3個？）
	void add_directional_light(const Vector3& dir);
	//ライトをすべて削除
	void clear();

private:
	std::vector<int> light_handle_;
};
#endif//!LIGHT_H_

