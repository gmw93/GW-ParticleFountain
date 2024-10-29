#include "Renderer.h"

Camera* Renderer::m_camera = new Camera();

Lighting* Renderer::m_lightings = new Lighting();

//Fountain_Animation* Renderer::m_fountain_animation = new Fountain_Animation();  
//Environment* Renderer::m_environment = new Environment();

nanogui::Screen* Renderer::m_nanogui_screen = nullptr;

bool Renderer::keys[1024];

Renderer::Renderer()
{
}


Renderer::~Renderer()
{	
}

void Renderer::nanogui_init(GLFWwindow* window)
{
	m_nanogui_screen = new nanogui::Screen();
	m_nanogui_screen->initialize(window, true);

	glViewport(0, 0, m_camera->width, m_camera->height);

	//glfwSwapInterval(0);
	//glfwSwapBuffers(window);


	m_nanogui_screen->setVisible(true);
	m_nanogui_screen->performLayout();

	glfwSetCursorPosCallback(window,
		[](GLFWwindow *window, double x, double y) {
		m_nanogui_screen->cursorPosCallbackEvent(x, y);
	}
	);

	glfwSetMouseButtonCallback(window,
		[](GLFWwindow *, int button, int action, int modifiers) {
		m_nanogui_screen->mouseButtonCallbackEvent(button, action, modifiers);
	}
	);

	glfwSetKeyCallback(window,
		[](GLFWwindow *window, int key, int scancode, int action, int mods) {
		//screen->keyCallbackEvent(key, scancode, action, mods);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				keys[key] = true;
			else if (action == GLFW_RELEASE)
				keys[key] = false;
		}
	}
	);

	glfwSetCharCallback(window,
		[](GLFWwindow *, unsigned int codepoint) {
		m_nanogui_screen->charCallbackEvent(codepoint);
	}
	);

	glfwSetDropCallback(window,
		[](GLFWwindow *, int count, const char **filenames) {
		m_nanogui_screen->dropCallbackEvent(count, filenames);
	}
	);

	glfwSetScrollCallback(window,
		[](GLFWwindow *, double x, double y) {
		m_nanogui_screen->scrollCallbackEvent(x, y);
		//m_camera->ProcessMouseScroll(y);
	}
	);

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow *, int width, int height) {
		m_nanogui_screen->resizeCallbackEvent(width, height);
	}
	);

}

void Renderer::init()
{
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

#if defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_camera->init();
	//m_environment->init();

	this->m_window = glfwCreateWindow(m_camera->width, m_camera->height, "Animated Fountain", nullptr, nullptr);
	glfwMakeContextCurrent(this->m_window);

	glewExperimental = GL_TRUE;
	glewInit();

	m_lightings->init();	
	nanogui_init(this->m_window);
}

void Renderer::display(GLFWwindow* window)
{
	Shader m_shader = Shader("./shader/basic.vert", "./shader/basic.frag");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::string s[4] = { "objs/grass.jpg", "objs/BlueEdge.jpg", "objs/BlueEdgeTwo.jpg", "objs/water.jpg" }; 
	//texture code attributes
	//for(int i = 0; 0 < 4; i++) 
	//{
	//	m_texture[i].init(s); 
	//}
	// Main frame while loop
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		if (is_scene_reset) {
			scene_reset();
			is_scene_reset = false;
		}

		camera_move();

		m_shader.use();
		setup_uniform_values(m_shader);
		//Texture Added
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture); 

		glUniform1i(glGetUniformLocation(m_shader.program, "ourTexture"), 0);

		draw_scene(m_shader);
		

		m_nanogui_screen->drawWidgets();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return;
}

void Renderer::run()
{
	init();
	display(this->m_window);
}

void Renderer::load_models()
{
	obj_list.clear();
	
	Object fountain_cube("./objs/Cube.obj");
	fountain_cube.obj_color = glm::vec4(1.0, 1.0, 0.0, 1.0);
	fountain_cube.obj_name = "fountain_cubes";
	
	Object fountain_sphere("./objs/sphere.obj");
	fountain_sphere.obj_color = glm::vec4(1.0, 1.0, 0.0, 1.0);
	fountain_sphere.obj_name = "fountain_spheres";

	Object plane_object("./objs/plane.obj");
	plane_object.obj_color = glm::vec4(0.5, 0.5, 0.5, 1.0);
	plane_object.obj_name = "plane";

	Object bowl_object("./objs/pool.obj");
	bowl_object.obj_color = glm::vec4(0.5, 0.5, 0.5, 0.0);
	bowl_object.obj_name = "fountain_base";
	
	Object particle_object("./objs/sphere.obj"); 
	//particle_object.m_render_type = RENDER_POINTS;
	particle_object.obj_name = "particle"; 
	
	for (float i = 20; i >= -20; i-=.5)
	{
		for (float j = 20; j >= -20; j-=.5)
		{
			if (j > 7 || i > 7 || j < -7 || i < -7)
			{
			std::vector<glm::vec3> points = {
				{i, 0.0f, j},
				{i, 0.25f, j},
				{i, 0.75f, j} 
			};
			Object grass_object(points); 
			grass_object.m_render_type = RENDER_LINES;
			///grass_object.obj_mat = glm::scale(glm::mat4(1.0f), glm::vec3(.5, .5, .5));
			grass_object.obj_color = glm::vec4(0.13, .55, 0.13, 1.0);
			grass_object.obj_name = "grass";
			bind_vaovbo(grass_object);
			grass_list.push_back(grass_object);
			}
			
		}
	}

	bind_vaovbo(fountain_cube);
	bind_vaovbo(fountain_sphere);
	bind_vaovbo(plane_object);
	bind_vaovbo(bowl_object);
	bind_vaovbo(particle_object);
	
	// Here we only load one model
	obj_list.push_back(fountain_cube);
	obj_list.push_back(fountain_sphere);
	obj_list.push_back(plane_object);
	obj_list.push_back(bowl_object);
	obj_list.push_back(particle_object);
}

void Renderer::draw_scene(Shader& shader)
{
	// Set up some basic parameters
	glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	glm::mat4 world_identity_obj_mat = glm::mat4(1.0f);
	//draw_axis(shader, world_identity_obj_mat);
	draw_plane(shader);
	//draw_environment(shader);
	draw_fountain(shader, m_fountain_animation); 
}

void Renderer::camera_move()
{
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	// Camera controls
	if (keys[GLFW_KEY_W])
		m_camera->process_keyboard(FORWARD, delta_time);
	if (keys[GLFW_KEY_S])
		m_camera->process_keyboard(BACKWARD, delta_time);
	if (keys[GLFW_KEY_A])
		m_camera->process_keyboard(LEFT, delta_time);
	if (keys[GLFW_KEY_D])
		m_camera->process_keyboard(RIGHT, delta_time);
	if (keys[GLFW_KEY_Q])
		m_camera->process_keyboard(UP, delta_time);
	if (keys[GLFW_KEY_E])
		m_camera->process_keyboard(DOWN, delta_time);
	if (keys[GLFW_KEY_I])
		m_camera->process_keyboard(ROTATE_X_UP, delta_time);
	if (keys[GLFW_KEY_K])
		m_camera->process_keyboard(ROTATE_X_DOWN, delta_time);
	if (keys[GLFW_KEY_J])
		m_camera->process_keyboard(ROTATE_Y_UP, delta_time);
	if (keys[GLFW_KEY_L])
		m_camera->process_keyboard(ROTATE_Y_DOWN, delta_time);
	if (keys[GLFW_KEY_U])
		m_camera->process_keyboard(ROTATE_Z_UP, delta_time);
	if (keys[GLFW_KEY_O])
		m_camera->process_keyboard(ROTATE_Z_DOWN, delta_time);

}

void Renderer::draw_object(Shader& shader, Object& object)
{
	glBindVertexArray(object.vao);

	glUniform3f(glGetUniformLocation(shader.program, "m_object.object_color"), object.obj_color[0], object.obj_color[1], object.obj_color[2]);
	glUniform1f(glGetUniformLocation(shader.program, "m_object.shininess"), object.shininess);

	if (object.m_render_type == RENDER_TRIANGLES)
	{
		if (object.m_obj_type == OBJ_POINTS)
		{
			std::cout << "Error: Cannot render triangles if input obj type is point\n";
			return;
		}
		if (object.m_obj_type == OBJ_TRIANGLES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object.vao_vertices.size());
		}
	}

	if (object.m_render_type == RENDER_LINES)
	{
		glLineWidth(10.0);
		if (object.m_obj_type == OBJ_POINTS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_LINE_LOOP, 0, object.vao_vertices.size());
		}
		if (object.m_obj_type == OBJ_TRIANGLES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, object.vao_vertices.size());
		}
	}

	if (object.m_obj_type == OBJ_POINTS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		glDrawArrays(GL_POINTS, 0, object.vao_vertices.size());
	}
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::draw_axis(Shader& shader, const glm::mat4 axis_obj_mat)
{
	// You can always see the arrow
	glDepthFunc(GL_ALWAYS);
	// Get arrow obj
	Object *arrow_obj = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "axis_arrow") {
			arrow_obj = &obj_list[i];
		}
	}

	if (arrow_obj == nullptr)
		return;

	// Draw main axis
	arrow_obj->obj_mat = axis_obj_mat;
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(arrow_obj->obj_mat));
	arrow_obj->obj_color = glm::vec4(1, 0, 0, 1);
	draw_object(shader, *arrow_obj);

	arrow_obj->obj_mat = axis_obj_mat;
	arrow_obj->obj_mat = glm::rotate(arrow_obj->obj_mat, glm::radians(90.0f), glm::vec3(0, 0, 1));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(arrow_obj->obj_mat));
	arrow_obj->obj_color = glm::vec4(0, 1, 0, 1);
	draw_object(shader, *arrow_obj);

	arrow_obj->obj_mat = axis_obj_mat;
	arrow_obj->obj_mat = glm::rotate(arrow_obj->obj_mat, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(arrow_obj->obj_mat));
	arrow_obj->obj_color = glm::vec4(0, 0, 1, 1);
	draw_object(shader, *arrow_obj);
	glDepthFunc(GL_LESS);
}

void Renderer::draw_plane(Shader& shader)
{
	Object *plane_obj = nullptr;
	
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "plane") {
			plane_obj = &obj_list[i];
			
		}
	}
	if (plane_obj == nullptr)
		return;
	//texture code
	//for (unsigned int i = 0; i < sizeof(plane_obj->vao_vertices); i++)
	//{
	//	vao[i] = plane_obj->vao_vertices[i].Position.; 
	//}
	//glBindVertexArray(plane_obj->vao); 
	//glActiveTexture(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//m_texture[0].Bind(plane_obj); 

	plane_obj->obj_mat =  glm::mat4(1.0f);
	plane_obj->obj_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-20, 0, -20));
	plane_obj->obj_mat = glm::scale(plane_obj->obj_mat, glm::vec3(10, 10, 10));
	center = plane_obj->obj_center;
	
	
	
	//glBindTexture(GL_TEXTURE_2D, 0);
	//std::cout << center.x << ", "<< center.y << ", " << center.z << std::endl;
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(plane_obj->obj_mat));
	plane_obj->obj_color = glm::vec4(0.36f, 0.25f, 0.20f, 1.0f);
	draw_object(shader, *plane_obj);
}

void Renderer::draw_environment(Shader& shader)
{
}

void Renderer::draw_fountain(Shader& shader, Fountain_Animation* m_fountain_animation)
{
			
	Object* particle_obj = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "particle") {
			particle_obj = &obj_list[i];
		}
	}
	if (particle_obj == nullptr)
		return;
	//keep track of location
	if (n != 1000)
	{
		m_fountain_animation[n].init(); 
		n++;
	}
	draw_structure(shader);
	
	//keep track of location
	for (int i = 0; i < n; i++)
	{
		m_fountain_animation[i].update(delta_time);
		glm::mat4 particle_obj_mat = glm::mat4(1.0f);
		particle_obj_mat *= glm::translate(glm::mat4(1.0f), m_fountain_animation[i].position);
		particle_obj_mat *= glm::scale(glm::mat4(1.0f), m_fountain_animation[i].particle_scale); 
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(particle_obj_mat));
		particle_obj->obj_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		draw_object(shader, *particle_obj);

	}
	
}
void Renderer::draw_structure(Shader& shader)
{
	Object* fountain_cub = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "fountain_cubes") {
			fountain_cub = &obj_list[i];
		}
	}
	Object* fountain_sph = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "fountain_spheres") {
			fountain_sph = &obj_list[i];
		}
	}
	Object* particle_obj = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "particle") {
			particle_obj = &obj_list[i];
		}
	}
	Object* base = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "fountain_base") {
			base = &obj_list[i];
		}
	}
	if (fountain_cub == nullptr)
		return;
	
	
	// Draw Fountain- level 1
	glm::mat4 fountain_cub_mat = glm::mat4(1.0f);
	fountain_cub_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, .75f, 0.0f));
	fountain_cub_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(.75f, .75f, .75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_cub_mat));
	fountain_cub->obj_color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);//0.7f, 0.0f, 0.0f, 1.0f);
	draw_object(shader, *fountain_cub);
	glm::mat4 fountain_sph_mat = glm::mat4(1.0f);
	fountain_sph_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.45f, 1.5f, -0.60f));
	fountain_sph_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(3.75f, 3.75f, 3.75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_sph_mat));
	fountain_sph->obj_color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	draw_object(shader, *fountain_sph);
	glm::mat4 fountain_sph_mat2 = glm::mat4(1.0f);
	fountain_sph_mat2 *= glm::translate(glm::mat4(1.0f), glm::vec3(0.45f, 2.25f, -0.60f));
	fountain_sph_mat2 *= glm::scale(glm::mat4(1.0f), glm::vec3(3.75f, 3.75f, 3.75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_sph_mat2));
	fountain_sph->obj_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	draw_object(shader, *fountain_sph);
	
	glm::mat4 fountain_cub_mat2 = glm::mat4(1.0f);
	fountain_cub_mat2 *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.25f, 0.0f));
	fountain_cub_mat2 *= glm::scale(glm::mat4(1.0f), glm::vec3(.75f, .75f, .75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_cub_mat2));
	fountain_cub->obj_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	draw_object(shader, *fountain_cub);
	//glm::mat4 bottom_obj_mat = glm::mat4(1.0f);
	//bottom_obj_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.05f, 0.0f));
	//bottom_obj_mat *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//bottom_obj_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(.2f, 0.2f, .002f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(bottom_obj_mat));
	//base->obj_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	//draw_object(shader, *base);
	
	//Draw Fountain- level 2
	glm::mat4 fountain_sph_mat3 = glm::mat4(1.0f);
	fountain_sph_mat3 *= glm::translate(glm::mat4(1.0f), glm::vec3(0.45f, 3.0f, -0.60f));
	fountain_sph_mat3 *= glm::scale(glm::mat4(1.0f), glm::vec3(3.75f, 3.75f, 3.75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_sph_mat3));
	fountain_sph->obj_color = glm::vec4(0.67f, 0.67f, 0.67f, 1.0f);
	draw_object(shader, *fountain_sph);
	
	glm::mat4 fountain_sph_mat4 = glm::mat4(1.0f);
	fountain_sph_mat4 *= glm::translate(glm::mat4(1.0f), glm::vec3(0.45f, 3.75f, -0.60f));
	fountain_sph_mat4 *= glm::scale(glm::mat4(1.0f), glm::vec3(3.75f, 3.75f, 3.75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_sph_mat4));
	fountain_sph->obj_color = glm::vec4(0.57f, 0.57f, 0.57f, 1.0f);
	draw_object(shader, *fountain_sph);
	
	glm::mat4 fountain_cub_mat3 = glm::mat4(1.0f);
	fountain_cub_mat3 *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.75f, 0.0f));
	fountain_cub_mat3 *= glm::scale(glm::mat4(1.0f), glm::vec3(.75f, .75f, .75f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(fountain_cub_mat3));
	fountain_cub->obj_color = glm::vec4(0.57f, 0.57f, 0.57f, 1.0f);
	draw_object(shader, *fountain_cub);
	
	glm::mat4 top_obj_mat = glm::mat4(1.0f);
	top_obj_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.25f, 0.0f));
	top_obj_mat *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	top_obj_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(.15f, 0.15f, .002f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(top_obj_mat));
	base->obj_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	draw_object(shader, *base);

	//Draw Fountain- Level 0
	glm::mat4 base_obj_mat = glm::mat4(1.0f);
	base_obj_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.4f, 0.0f));
	base_obj_mat *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	base_obj_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(.35f, 0.35f, .005f));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(base_obj_mat));
	base->obj_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	draw_object(shader, *base);
	
}

void Renderer::bind_vaovbo(Object &cur_obj)
{
	glGenVertexArrays(1, &cur_obj.vao);
	glGenBuffers(1, &cur_obj.vbo);
	//Texture Added
	glGenBuffers(1, &cur_obj.ebo);
	//Already Here
	glBindVertexArray(cur_obj.vao);

	glBindBuffer(GL_ARRAY_BUFFER, cur_obj.vbo);
	glBufferData(GL_ARRAY_BUFFER, cur_obj.vao_vertices.size() * sizeof(Object::Vertex), &cur_obj.vao_vertices[0], GL_STATIC_DRAW);
	//Texture Added
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cur_obj.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cur_obj.veo_indices.size(), &cur_obj.veo_indices[0], GL_STATIC_DRAW);
	//Already here
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, TexCoords));

	glBindVertexArray(0);
}

void Renderer::setup_uniform_values(Shader& shader)
{
	// Camera uniform values
	glUniform3f(glGetUniformLocation(shader.program, "camera_pos"), m_camera->position.x, m_camera->position.y, m_camera->position.z);

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(m_camera->get_projection_mat()));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(m_camera->get_view_mat()));

	// Light uniform values
	glUniform1i(glGetUniformLocation(shader.program, "dir_light.status"), m_lightings->direction_light.status);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.direction"), m_lightings->direction_light.direction[0], m_lightings->direction_light.direction[1], m_lightings->direction_light.direction[2]);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.ambient"), m_lightings->direction_light.ambient[0], m_lightings->direction_light.ambient[1], m_lightings->direction_light.ambient[2]);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.diffuse"), m_lightings->direction_light.diffuse[0], m_lightings->direction_light.diffuse[1], m_lightings->direction_light.diffuse[2]);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.specular"), m_lightings->direction_light.specular[0], m_lightings->direction_light.specular[1], m_lightings->direction_light.specular[2]);

	// Set current point light as camera's position
	m_lightings->point_light.position = m_camera->position;
	glUniform1i(glGetUniformLocation(shader.program, "point_light.status"), m_lightings->point_light.status);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.position"), m_lightings->point_light.position[0], m_lightings->point_light.position[1], m_lightings->point_light.position[2]);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.ambient"), m_lightings->point_light.ambient[0], m_lightings->point_light.ambient[1], m_lightings->point_light.ambient[2]);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.diffuse"), m_lightings->point_light.diffuse[0], m_lightings->point_light.diffuse[1], m_lightings->point_light.diffuse[2]);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.specular"), m_lightings->point_light.specular[0], m_lightings->point_light.specular[1], m_lightings->point_light.specular[2]);
	glUniform1f(glGetUniformLocation(shader.program, "point_light.constant"), m_lightings->point_light.constant);
	glUniform1f(glGetUniformLocation(shader.program, "point_light.linear"), m_lightings->point_light.linear);
	glUniform1f(glGetUniformLocation(shader.program, "point_light.quadratic"), m_lightings->point_light.quadratic);
}

void Renderer::scene_reset()
{
	load_models();
	m_camera->reset();
}