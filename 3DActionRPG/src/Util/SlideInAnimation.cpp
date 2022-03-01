#include "SlideInAnimation.h"

#include "Screen.h"
#include "AssetsManager/Image.h"

enum
{
    Wait,
    Animating,
    End
};

SlideInAnimation::SlideInAnimation():
    state_{Wait}
{
}

SlideInAnimation::SlideInAnimation(int texture_id, int image_width, int image_height, float slide_speed, float target):
    textue_id_{texture_id},
    width_{image_width},
    height_{image_height},
    speed_{slide_speed},
    target_{target},
    position_{Screen::Width / 2, -image_height},
    state_{Wait}
{
}

SlideInAnimation::SlideInAnimation(int texture_id, const Vector2& image_size, float slide_speed, float target):
    SlideInAnimation{textue_id_, static_cast<int>(image_size.x), static_cast<int>(image_size.y), slide_speed, target}
{
}

void SlideInAnimation::start()
{
    state_ = Animating;
}

void SlideInAnimation::update(float delta_time)
{
    if (is_running()) {
        position_.y += speed_;
        if (position_.y >= target_) {
            position_.y = target_;
            state_ = End;
        }
    }
}

void SlideInAnimation::draw() const
{
    if (is_wait()) return;
    Image::draw_rota_graph(textue_id_, position_.x, position_.y);
}

bool SlideInAnimation::is_wait() const
{
    return state_ == Wait;
}

bool SlideInAnimation::is_running() const
{
    return state_ == Animating;
}

bool SlideInAnimation::is_end() const
{
    return state_ == End;
}
