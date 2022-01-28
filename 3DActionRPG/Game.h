#ifndef GAME_H_
#define GAME_H_



class SceneManager;
class GameManager;
class PlayerParameter;

class Game
{
public:
	Game();
	‾Game() = default;
	void init();
	void update();
	void draw();  //dxlibを使う場合、constにできない
	void end();

private:
	SceneManager& sm;
	GameManager& gm;
	PlayerParameter& player_paremeter;
};
#endif//!GAME_H_
