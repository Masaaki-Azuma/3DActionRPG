#ifndef MY_RANDOM_H_
#define MY_RANDOM_H_

#include <random>

class MyRandom
{
public:
	static void Init();
	//最小値以上最大値以下のintを返す
	static int Range(int min, int max);
	//最小値以上最大値以下のintを返す
	static float Range(float min, float max);
	//指定確率(1以下の確率)でtrueを返す
	static bool Probability(float probability);

private:
	static std::random_device rd;//非決定的乱数生成器
	static std::mt19937 mt;//決定的乱数生成器、高速

private:
	MyRandom() = default;
	‾MyRandom() = default;
};
#endif//!MY_RANDOM_H_
