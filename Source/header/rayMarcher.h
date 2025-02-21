#ifndef RAYMARCHER_H
#define RAYMARCHER_H

class RayMarcher
{
public:
	RayMarcher() = default;



private:
	float FOV = 1.0;
	int maxSteps = 256;
	float maxDist = 500;
	float epsilon = 0.001;
};

#endif
