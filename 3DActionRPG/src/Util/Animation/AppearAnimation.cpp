#include "AppearAnimation.h"

#include <DxLib.h>

#include "AssetsManager/Image.h"
#include "Util/Vector2.h"

enum //状態
{
    Hide,
    Appearing,
    Appeared,
};

AppearAnimation::AppearAnimation(float x, float y, int color_id, int shilhouette_id, int image_width, int image_height, float appear_speed):
    x_{x},
    y_{ y },
    color_id_{color_id},
    shilhouette_id_{shilhouette_id},
    state_{Hide},
    width_{image_width},
    height_{image_height},
    appear_speed_{appear_speed}
{
}

AppearAnimation::AppearAnimation(const Vector2& position, int color_id, int silhouette_id, const Vector2& image_size, float appear_speed):
    AppearAnimation{ position.x, position.y, color_id, silhouette_id, static_cast<int>(image_size.x), static_cast<int>(image_size.y), appear_speed }
{
}

void AppearAnimation::appear()
{
    state_ = Appearing;
}

void AppearAnimation::reset()
{
    state_ = Hide;
    appear_height_ = 0.0f;
}

void AppearAnimation::update(float delta_time)
{
    //徐々に公開部分を増やす
    if (state_ == Appearing) {
        appear_height_ += appear_speed_;
        if (appear_height_ >= height_) {
            appear_height_ = static_cast<float>(height_);
            state_ = Appeared;
        }
    }
}

void AppearAnimation::draw() const
{
    //色画像
    //Image::draw_rota_graph(color_id_, x_, y_);

    DxLib::DrawRectRotaGraphF(x_, y_ - (height_ - appear_height_) / 2, 0, 0, width_, static_cast<int>(appear_height_), 1.0, 0.0, Image::texture_handle(color_id_), true);

    //シルエット画像
    DxLib::DrawRectRotaGraphF(x_, y_ + appear_height_ / 2, 0, static_cast<int>(appear_height_), width_, height_, 1.0, 0.0, Image::texture_handle(shilhouette_id_), true);
}

bool AppearAnimation::is_end() const
{
    return state_ == Appeared;
}
