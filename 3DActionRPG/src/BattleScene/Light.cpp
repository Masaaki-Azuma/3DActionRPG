#include "Light.h"

#include <DxLib.h>

#include "Util/DxConverter.h"
#include "Util/Vector3.h"


Light::Light()
{
	//標準ライトを削除
	DxLib::SetLightEnable(FALSE);
}

void Light::add_directional_light(const Vector3& dir)
{
	int handle = DxLib::CreateDirLightHandle(DxConverter::GetVECTOR(dir));
	light_handle_.push_back(handle);
}

void Light::clear()
{
	DxLib::DeleteLightHandleAll();
	light_handle_.clear();
}
