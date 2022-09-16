#ifndef RANDOM_H
#define RANDOM_H

class Random {
public:
	Random() {
		return;
	}
	static float RandomRangef(float min = 0.0f, float max = 1.0f) 
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
	static int RandomRangei(int min = 0, int max = 2) 
	{
		return min + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (max - min)));
	}
};
#endif