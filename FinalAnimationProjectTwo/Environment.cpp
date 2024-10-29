#include "Environment.h"

Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::init() 
{
	this->control_points_pos = {
		{ 7.5, 1.0, 0.0 },
		{ 0.0, 1.0, 7.5 },
		{ -7.5, 1.0, 0.0 },
		{ 0.0, 1.0, -7.5 }
	};
	calculate_curve();
	fill_curve();
	this->box_points = {
		{20.0, 0.0, 0.0},
		{0.0, 0.0, 20.0},
		{-20.0, 0.0, 0.0},
		{0.0, 0.0, -20.0}
	};
}

void Environment::calculate_curve()
{
	this->curve_points_pos = {
		{ 7.0, 0.0, 0.0 },
		{ 0.0, 0.0, 7.0 },
		{ -7.0, 0.0, 0.0 },
		{ 0.0, 0.0, -7.0 }
	};
		for (int n = 0; n < 4; n++)
		{
			curve_points_pos.erase(curve_points_pos.begin());
		}
		curve_points_pos.insert(curve_points_pos.begin(), control_points_pos[0]);
		int i = 1;
		for (int n = 0; n < 4; n++)
		{
			//std::cout << control_points_pos[n].x << std::endl;
			for (float u = 0.005; u < 1; u += .005)
			{
				int p[4];
				p[1] = n;
				p[2] = p[1] + 1;
				p[3] = p[2] + 1;
				p[0] = p[1] - 1;
				if (p[0] < 0) { p[0] = 3; }
				if (p[3] > 3) { p[3] = 0; }
				if (p[2] > 3)
				{
					p[2] = 0;
					p[3] = 1;
				}
				//curve_CR(p, u, i);
				float uSquared = u * u;
				float uCubed = u * uSquared;

				float q0 = -tau * uCubed + 2 * tau * uSquared - tau * u;//-uCubed + 2.0 * uSquared - u; 
				float q1 = (2 - tau) * uCubed + (tau - 3) * uSquared + 1;//3.0 * uCubed - 5 * uSquared + 2; 
				float q2 = (tau - 2) * uCubed + (3 - 2 * tau) * uSquared + u * tau; //-3.0 * uCubed + 4 * uSquared + u;  
				float q3 = uCubed * tau - uSquared * tau; //uCubed + uSquared; 
				float nx = q0 * control_points_pos[p[0]].x + q1 * control_points_pos[p[1]].x + q2 * control_points_pos[p[2]].x + q3 * control_points_pos[p[3]].x;
				float ny = q0 * control_points_pos[p[0]].y + q1 * control_points_pos[p[1]].y + q2 * control_points_pos[p[2]].y + q3 * control_points_pos[p[3]].y;
				float nz = q0 * control_points_pos[p[0]].z + q1 * control_points_pos[p[1]].z + q2 * control_points_pos[p[2]].z + q3 * control_points_pos[p[3]].z;
				glm::vec3 vnew = glm::vec3(nx, ny, nz);
				curve_points_pos.insert(curve_points_pos.begin() + i, vnew);
				i++;
			}
			curve_points_pos.pop_back();
			curve_points_pos.push_back(control_points_pos[n + 1]);
		}
		curve_points_pos.pop_back();
		curve_points_pos.push_back(curve_points_pos[0]);
}
void Environment::fill_curve()
{

}