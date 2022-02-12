#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <Windows.h>

class Stopwatch {
public:
    LONGLONG Frequency() const;
    bool IsHighResolution() const;

    Stopwatch();

    bool IsRunning() const;
    LONGLONG ElapsedMilliseconds();
    LONGLONG ElapsedTicks();

    static LONGLONG GetTimestamp();
    static Stopwatch StartNew();
    void Reset();
    void Restart();
    void Start();
    void Stop();

public:
    Stopwatch(const Stopwatch&) = delete;
    Stopwatch& operator=(const Stopwatch) = delete;
    Stopwatch(Stopwatch&&) = default;
    Stopwatch& operator=(Stopwatch&&) = default;

private:
    void UpdateElapsedTicks();

private:
    bool isHighResolution_{ false };
    bool isRunning_{ false };
    LONGLONG elapsedTicks_{ 0 };
    LARGE_INTEGER frequency_{ 0 };
    LARGE_INTEGER start_{ 0 };
};

#endif