#include "Boid.h"

Boid::Boid(vec3 p, vec3 v, unsigned id, float max)
{
	ID = id;
	velocity = v;
	position = p;
	maxSpeed = max;
	startSpeed = sqrt(v.x*v.x + v.y*v.y);
}

void Boid::accelerate()
{
	float a = .0001f;
	float speed = 0;

	//X boundaries
	if (position.x >= borderX)
		velocity.x -= a;
	else if (position.x <= (-1 * borderX))
		velocity.x += a;
	else
	{
		speed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
		if (speed != startSpeed)
		{
			//Make vector in the same direction with desired magnatude
			velocity = (velocity / speed) * startSpeed;
		}
	}

	//Y boundaries
	if (position.y >= borderY)
		velocity.y -= a;
	else if (position.y <= (-1 * borderY))
		velocity.y += a;
	else
	{
		speed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
		if (speed != startSpeed)
		{
			velocity = (velocity / speed) * startSpeed;
		}
	}
	 /*
	speed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
	if (speed > startSpeed)
	{
		velocity = (velocity / speed) * startSpeed;
	}*/

	position += velocity;
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