#include "GaugeActor.h"
#include "AssetsManager/PlayerDatabase.h"

GaugeActor::GaugeActor(float left, float top, int width, int height, int frame_id, int bar_id, int barBG_id):
	gauge_{left, top, width, height, frame_id, bar_id, barBG_id},
	p_DB_{PlayerDatabase::GetInstance()}
{
	gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
	gauge_.set_edge_width(3);
}

void GaugeActor::draw_gui() const
{
	gauge_.draw_gui(static_cast<float>(p_DB_.get_current_parameter().hp));
}
