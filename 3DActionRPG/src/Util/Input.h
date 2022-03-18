#ifndef INPUT_H_
#define INPUT_H_

class Input
{
public:
	static void init();
	static void update();
	static bool get_button(int button_id);
	static bool get_button_down(int button_id);
	static bool get_button_up(int button_id);

private:
	static int prev_state;
	static int current_state;

};
#endif//!INPUT_H_

