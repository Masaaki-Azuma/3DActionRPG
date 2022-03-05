#include "FadeInAnimation.h"

#include "AssetsManager/Image.h"

enum //アニメーション状態
{
    Wait,
    Running,
    End
};

const float Div{ 10 };

FadeInAnimation::FadeInAnimation(int texture_id, const Vector3& initial_pos, float distance):
    texture_{texture_id},
    position_{initial_pos},
    initial_pos_{initial_pos},
    distance_{distance},
    alpha_{0},
    state_{Wait}
{
}

void FadeInAnimation::start()
{
    state_ = Running;
}

void FadeInAnimation::reset()
{
    state_ = Wait;
}

void FadeInAnimation::update(float delta_time)
{
    if (is_running()) {
        position_.y -= distance_ / Div;
        alpha_ += 255 / Div;
        if (position_.y < initial_pos_.y + distance_) {
            position_.y = initial_pos_.y + distance_;
            state_ = End;
        }
    }
}

void FadeInAnimation::draw() const
{
    if (!is_waiting()) {
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
        Image::draw_rota_graph(texture_, position_.x, position_.y);
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha_);
    }
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
