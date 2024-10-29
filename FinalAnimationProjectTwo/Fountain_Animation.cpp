#include "Fountain_Animation.h"



Fountain_Animation::Fountain_Animation()
{
}


Fountain_Animation::~Fountain_Animation()
{
}

void Fountain_Animation::init()
{
	mass = 0.05;

	particle_scale = { 0.15f, 0.15f, 0.15f };	
	particle_velocity.y = 20; //= rand()%10+5;
	particle_velocity.x = 0; //= rand()%10-10;
	particle_velocity.z = 0;//= rand()%10-10;
	std::random_device rP;
	std::mt19937 ran(rP());
	std::uniform_real_distribution<double> vPos(-.3, 0.3);
	float x = vPos(ran);
	float z = vPos(ran);
	position = { x, 4.55f, z };
	n = 0;
	lifespan = 1000;
	//frC = 0;
}
void Fountain_Animation::forces()
{
	std::random_device rF;
	std::mt19937 ran(rF());
	std::uniform_real_distribution<double> vForce(0.0, 5.0);
	gravity = -9.81 * mass; 
	friction = -1 *.02;
	
	drag = -(1/2) * 1 * 1 * 0.01;
	if (n < 2)
	{
		Force.y = drag * particle_velocity.y *mass; 
		Force.y += gravity;
		Force.x = drag*particle_velocity.x * mass;
		Force.z = drag*particle_velocity.z * mass;
		particle_acceleration.y = Force.y / mass;
	}
	if (n == 2)
	{
		Force.y = drag * particle_velocity.y * mass;
		Force.y += gravity + -1 * gravity;
		Force.x = friction * particle_velocity.x * mass;
		Force.z = friction * particle_velocity.z * mass;
		particle_acceleration.y = Force.y / mass;
	}
	if (count == 10)
	{
		wind = vForce(ran) * mass;//rand
		std::random_device vC;
		std::mt19937 ranC(vC());
		std::uniform_int_distribution<std::mt19937::result_type> vChance(0, 2);
		int chanceX = vChance(ranC);
		int chanceZ = vChance(ranC);
		if (chanceX == 0)
			Force.x += wind;
		else if (chanceX == 1)
			Force.x += -1 * wind;
	
		if (chanceZ == 0)
			Force.z += -1 * wind;
		else if (chanceZ == 1)
			Force.z += wind;
		count = 0;

	}
	particle_acceleration.z = Force.z / mass;
	particle_acceleration.x = Force.x / mass;
}

void Fountain_Animation::update(float delta_time)
{
	
	if (lifespan == 980)
	{
		std::random_device vV;
		std::mt19937 vel(vV());
		std::uniform_real_distribution<double> vVelo(0, 2);
		float xV = vVelo(vel);
		float zV = vVelo(vel);
		particle_velocity.x = xV; //= rand()%10-10;
		particle_velocity.z = zV;//= rand()%10-10;
		std::random_device vC;
		std::mt19937 vNeg(vC());
		std::uniform_int_distribution<std::mt19937::result_type> neg(0, 1);
		int chX = neg(vNeg);
		int chZ = neg(vNeg);
		if (chX == 1)
		{
			particle_velocity.x *= -1;
		}
		if (chZ == 1)
		{
			particle_velocity.z *= -1;
		}
		//n++;
	}	
	//std::cout << frC << std::endl;
	
	if (position.y <= 4.35 && sqrt(pow(position.x, 2) + pow(position.z, 2)) < 3)
	{
		position.y = 4.30;
		particle_velocity.y = 0;
		particle_acceleration.y = 0;
		n = 2;
	}
	forces();
	particle_velocity = particle_velocity + particle_acceleration * delta_time;
	position = position + particle_velocity *delta_time + particle_acceleration *delta_time;
	if (lifespan == 0 || position.y < 0 || position.y > 40)
	{
		reset();
	}
	lifespan -= 1;	
	count++;
	n = 0;
}

void Fountain_Animation::reset()
{
	init();
}

