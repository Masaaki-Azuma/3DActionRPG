#ifndef GAME_H_
#define GAME_H_


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
    World world_;
    //Player player_;
    //Camera camera_;
};

#endif