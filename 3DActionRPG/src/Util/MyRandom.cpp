#include "MyRandom.h"

std::random_device MyRandom::rd;//非決定的乱数生成器
std::mt19937 MyRandom::mt;//決定的乱数生成器、高速

void MyRandom::Init()
{
	mt.seed(rd());
}

int MyRandom::Range(int min, int max)
{
	std::uniform_int_distribution<> uid(min, max);
	return uid(mt);
}

float MyRandom::Range(float min, float max)
{
	std::uniform_real_distribution<float> urd(min, max);
	return urd(mt);
}

bool MyRandom::Probability(float probability)
{
	std::bernoulli_distribution bd(probability);
	return bd(mt);
}