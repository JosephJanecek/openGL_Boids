#include "Boid.h"

Boid::Boid(vec3 p, float max, vec3 v)
{
	velocity = v;
	position = p;
	maxSpeed = max;
}

void Boid::accelerate()
{
	if (position.x >= border || position.x <= (-1 * border))
		velocity.x *= -1;
	if (position.y >= border || position.y <= (-1 * border))
		velocity.y *= -1;

	position.x += velocity.x;
	position.y += velocity.y;
}