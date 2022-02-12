#include "Stopwatch.h"

LONGLONG Stopwatch::Frequency() const {
    return frequency_.QuadPart;
}
bool Stopwatch::IsHighResolution() const {
    return isHighResolution_;
}

Stopwatch::Stopwatch() {
    isHighResolution_ = QueryPerformanceFrequency(&frequency_);
}

bool Stopwatch::IsRunning() const {
    return isRunning_;
}
LONGLONG Stopwatch::ElapsedMilliseconds() {
    return 1000LL * ElapsedTicks() / frequency_.QuadPart;
}
LONGLONG Stopwatch::ElapsedTicks() {
    if (isRunning_) UpdateElapsedTicks();
    return elapsedTicks_;
}

LONGLONG Stopwatch::GetTimestamp() {
    LARGE_INTEGER now{ 0LL };
    QueryPerformanceCounter(&now);
    return now.QuadPart;
}
Stopwatch Stopwatch::StartNew() {
    Stopwatch newStopwatch;
    newStopwatch.Start();
    return newStopwatch;
}
void Stopwatch::Reset() {
    isRunning_ = false;
    start_ = { 0LL };
}
void Stopwatch::Restart() {
    Reset();
    Start();
}
void Stopwatch::Start() {
    isRunning_ = true;
    start_.QuadPart = GetTimestamp();
}
void Stopwatch::Stop() {
    isRunning_ = false;
}

void Stopwatch::UpdateElapsedTicks() {
    elapsedTicks_ = (GetTimestamp() - start_.QuadPart);
}