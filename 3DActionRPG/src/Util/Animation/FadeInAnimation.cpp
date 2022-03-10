#include "FadeInAnimation.h"

#include "AssetsManager/Image.h"
#include "AssetsManager/Font.h"

enum //アニメーション状態
{
    Wait,
    Running,
    End
};

enum //描画対象
{
    Mode_Texture,
    Mode_Text,
};

FadeInAnimation::FadeInAnimation(int texture_id, const Vector3& initial_pos, float second, float distance) :
    state_{Wait},
    mode_{Mode_Texture},
    texture_{ texture_id },
    position_{ initial_pos },
    prev_position_{ initial_pos },
    initial_pos_{ initial_pos },
    second_{ second },
    distance_{ distance }
{
    reset();
}

FadeInAnimation::FadeInAnimation(const std::string& text, int font_handle, int color, const Vector3& initial_pos, float sec, float distance) :
    state_{ Wait },
    mode_{ Mode_Text },
    text_{ text },
    font_handle_{ font_handle },
    font_color_{ color },
    position_{ initial_pos },
    prev_position_{ initial_pos },
    initial_pos_{ initial_pos },
    second_{ sec },
    distance_{ distance }
{
    reset();
}

void FadeInAnimation::start()
{
    state_ = Running;
}

void FadeInAnimation::reset()
{
    alpha_ = 0;
    position_ = initial_pos_;
    state_ = Wait;
}

void FadeInAnimation::update(float delta_time)
{
    prev_position_ = position_;

    if (is_running()) {
        position_.y += distance_ / second_ * delta_time;
        alpha_ += 255 / second_ * delta_time;
        if (has_reached_target() && is_completely_opaque()) {
            position_ = target_pos();
            state_ = End;
        }
    }
}

void FadeInAnimation::draw() const
{
    if (is_waiting()) return;

    //不透明度の影響を有効化
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

    switch (mode_) {
    case Mode_Texture: Image::draw_rota_graph(texture_, position_.x, position_.y);         break;
    case Mode_Text:    Font::draw_centered(position_.y, text_, font_color_, font_handle_); break;
    }
    //不透明度の影響を無効化
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha_);
}

bool FadeInAnimation::is_waiting() const
{
    return state_ == Wait;
}

bool FadeInAnimation::is_running() const
{
    return state_ == Running;
}

bool FadeInAnimation::is_end() const
{
    return state_ == End;
}

bool FadeInAnimation::has_reached_target() const
{
    bool has_reached =
        (prev_position_.y < target_pos().y) && (target_pos().y < position_.y) ||
        (position_.y < target_pos().y) && (target_pos().y < prev_position_.y);
    bool is_immovable = distance_ == 0.0f;

    return has_reached || is_immovable;
}

bool FadeInAnimation::is_completely_opaque() const
{
    return alpha_ >= 255;
}

Vector3 FadeInAnimation::target_pos() const
{
    Vector3 target = initial_pos_;
    target.y += distance_;
    return target;
}
