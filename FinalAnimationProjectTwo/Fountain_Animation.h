#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
//#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <random> 

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

class Fountain_Animation
{
public:
	Fountain_Animation();
	~Fountain_Animation();

	void init();
	void update(float delta_time);
	void reset();
	void forces();

public:

	//Here are the attributes of each particle
	glm::vec3 particle_scale;
	glm::vec3 position; 
	glm::vec3 particle_velocity;
	glm::vec3 particle_acceleration;
	glm::vec3 Force;
	float gravity;
	float drag;
	float wind;
	float friction;
	int lifespan;
	float start;
	float mass;
	int n;
	float velocity;
	float acceleration;
	int count;
	bool turnOn;
};

