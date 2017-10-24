#ifndef BOID_H
#define BOID_H

#include "GL\vec.h"
#include "GL\mat.h"
class Boid
{
private:
	const float borderX = .90f;
	const float borderY = .80f;
	unsigned int ID;
	float startSpeed;
	float maxSpeed;
public:
	vec3 velocity;
	vec3 position;
	Boid(vec3 p, vec3 v, unsigned id, float max = 0.005);
	bool operator==(const Boid &b)
	{
		return ID == b.ID;
	}
	bool operator!=(const Boid &b)
	{
		return ID != b.ID;
	}

	//Moves the boid by it's velocity
	void accelerate();
	//Returns a RotateZ matrix in the direction of the velocity
	mat4 direction();
};
#endif // !BOID_H

