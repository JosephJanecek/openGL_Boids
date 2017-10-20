#ifndef BOID_H
#define BOID_H

#include "GL\vec.h"
#include "GL\mat.h"
class Boid
{
private:
	const float border = .95f;
	float maxSpeed;
public:
	vec3 velocity;
	vec3 position;
	Boid(vec3 p, vec3 v, float max = 0.005);

	//Moves the boid by it's velocity
	void accelerate();
	//Returns a RotateZ matrix in the direction of the velocity
	mat4 direction();
};
#endif // !BOID_H

