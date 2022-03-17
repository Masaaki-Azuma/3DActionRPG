#include "ControllUI.h"
#include "AssetsManager/Image.h"
#include "Util/PadInput.h"

ControllUI::ControllUI(const Vector3& position)
{
	position_ = position;
}

void ControllUI::update(float delta_time)
{
}

void ControllUI::draw() const
{
	Image& image = Image::GetInstance();
	PadInput& input = PadInput::GetInstance();
	//NOTE:メンバでなければconst関数でなくても使えるようだ
	if (input.GetButton(XINPUT_BUTTON_B)) {
		Vector3 spark_pos = position() + Vector3{ 50.0f, 0.0f };
		image.draw_rota_graph(Texture_Spark, spark_pos.x, spark_pos.y);
	}
	if (input.GetButton(XINPUT_BUTTON_A)) {
		Vector3 spark_pos = position() + Vector3{ 0.0f, 50.0f };
		image.draw_rota_graph(Texture_Spark, spark_pos.x, spark_pos.y);
	}
	image.draw_rota_graph(Texture_ControllerUI, position_.x, position_.y);

}
