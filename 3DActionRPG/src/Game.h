#ifndef GAME_H_
#define GAME_H_

#include "SceneManager.h"

#include "World.h"

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
    //World world_;
};

#endif