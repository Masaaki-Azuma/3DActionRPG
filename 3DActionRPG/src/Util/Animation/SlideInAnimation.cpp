#include "SlideInAnimation.h"

#include "Screen.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"

enum //アニメーション状態
{
    Wait,
    Animating,
    End
};

enum //表示モード
{
    Mode_Texture,
    Mode_Text,
};

SlideInAnimation::SlideInAnimation():
    state_{Wait}
{
}

SlideInAnimation::SlideInAnimation(int texture_id, int image_width, int image_height, float slide_speed, float target):
    state_{Wait},
    mode_{Mode_Texture},
    textue_id_{texture_id},
    width_{image_width},
    height_{image_height},
    speed_{slide_speed},
    target_{target},
    position_{Screen::Width / 2, -image_height}
{
    reset();
}

SlideInAnimation::SlideInAnimation(int texture_id, const Vector2& image_size, float slide_speed, float target):
    SlideInAnimation{textue_id_, static_cast<int>(image_size.x), static_cast<int>(image_size.y), slide_speed, target}
{
}

SlideInAnimation::SlideInAnimation(const std::string& text, int font_handle, int font_size, unsigned int color, float slide_speed, float target) :
    state_{ Wait },
    mode_{ Mode_Text },
    text_{ text },
    font_handle_{ font_handle },
    font_size_{ font_size },
    color_{ color },
    speed_{ slide_speed },
    target_{ target }
{
    position_.y = static_cast<float>(-font_size_ * 2);
}

void SlideInAnimation::start()
{
    state_ = Animating;
}

void SlideInAnimation::reset()
{
    state_ = Wait;
    position_.y = static_cast<float>(-font_size_ * 2);
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

    switch (mode_) {
    case Mode_Text: Font::draw_centered(position_.y, text_ , color_, font_handle_); break;
    case Mode_Texture: Image::draw_rota_graph(textue_id_, position_.x, position_.y); break;
    }
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
