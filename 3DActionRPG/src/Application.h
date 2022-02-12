#ifndef APPLICATION_H_
#define APPLICATION_H_

class Game;

class Application {
public:
    Application() = default;
    ‾Application() = default;
    static const float CurrentFPS();
    void Run();

public:
    Application(const Application&) = delete;
    Application& operator=(const Application) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

private:
    void Draw();

private:
    static float currentFPS_;
    Game* game_{ nullptr };
    int skipCount_{ 0 }; // 何回連続でフレームスキップしたか
};

#endif