#include "ControllUI.h"
#include "AssetsManager/Image.h"
#include "Util/PadInput.h"
#include "BattleScene/IWorld.h"

ControllUI::ControllUI(IWorld& world, const Vector3& position):
	image_{Image::GetInstance()},
	input_{PadInput::GetInstance()}
{
	world_ = &world;
	position_ = position;
}

void ControllUI::update(float delta_time)
{
	
}

void ControllUI::draw() const
{
	if(!world_->is_battle_end()) draw_spark();
	image_.draw_rota_graph(Texture_ControllerUI, position_.x, position_.y);

}

void ControllUI::draw_spark() const
{
	//NOTE:メンバでなければconst関数でなくても使えるようだ
	if (input_.GetButton(XINPUT_BUTTON_B)) {
		Vector3 spark_pos = position() + Vector3{ 50.0f, 0.0f };
		image_.draw_rota_graph(Texture_Spark, spark_pos.x, spark_pos.y);
	}
	if (input_.GetButton(XINPUT_BUTTON_A)) {
		Vector3 spark_pos = position() + Vector3{ 0.0f, 50.0f };
		image_.draw_rota_graph(Texture_Spark, spark_pos.x, spark_pos.y);
	}
}
