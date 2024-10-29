#pragma once
#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Environment
{
public:
	Environment();
	~Environment();
	
	void init();
	void calculate_curve();
	void fill_curve();
public:
	float tau = 0.5;
	int num_points = 200;
	std::vector<glm::vec3> control_points_pos;
	std::vector<glm::vec3> curve_points_pos;
	std::vector<glm::vec3> box_points;
};