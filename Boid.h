#ifndef BOID_H
#define BOID_H

#include "GL\vec.h"
class Boid
{
private:
	const float border = .95f;
	float maxSpeed;
public:
	vec3 velocity;
	vec3 position;
	Boid(vec3 p = vec3(0), float max = 0, vec3 v = vec3(0.005, 0 ,0));

	void accelerate();
};
#endif // !BOID_H

