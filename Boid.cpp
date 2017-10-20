#include "Boid.h"

Boid::Boid(vec3 p, vec3 v, float max)
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

mat4 Boid::direction()
{
	mat4 dir;
	double theta = atan(velocity.y / velocity.x);

	//This check is so that we get angles in the second and third quadrant when we should
	if (velocity.x < 0)
		theta += 3.14159;
	
	//This builds a rotate matrix that will rotate the boid along the z-axis
	dir[0][0] = dir[1][1] = cos(theta);
	dir[1][0] = sin(theta);
	dir[0][1] = -dir[1][0];

	return dir;
}