#ifndef GAME_H_
#define GAME_H_

#include "SceneManager.h"

class Game {
public:
    Game();
    ‾Game();
    void Init();
    void Update();
    void Draw();
    void End();

private:
    SceneManager scene_manager_;
};

#endif