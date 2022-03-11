#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
    reset();
}

void SimpleTimer::reset(float time)
{
    state_timer_ = time;
    prev_state_timer_ = time;
}

void SimpleTimer::update(float delta_time)
{
    prev_state_timer_ = state_timer_;
    state_timer_ += delta_time;
}

float SimpleTimer::elapsed_sec() const
{
    return state_timer_;
}

bool SimpleTimer::has_elapsed(float time) const
{
    return state_timer_ >= time;
}

bool SimpleTimer::has_excessed(float time) const
{
    return state_timer_ >= time && prev_state_timer_ < time;
}
