#ifndef ACTOR_H_
#define ACTOR_H_

#include <memory>
#include <string>

class Actor
{
public:
	Actor() {};
	virtual ‾Actor() {};
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void OnCollision(std::shared_ptr<Actor> other){}

public:
	std::string name;
	std::string tag;
	float x{ 0 };
	float y{ 0 };
	float z{ 0 };
	float vx{ 0 };
	float vy{ 0 };
	float vz{ 0 };
	bool is_dead{ false };
};
#endif//!ACTOR_H_

