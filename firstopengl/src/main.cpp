#include "config.h"
#include "triangle_mesh.h"
#include "material.h"
#include "linear_algebra.h"


unsigned int make_module(const std::string& filepath, unsigned int module_type); 
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

int main() {

	GLFWwindow* window;

	if(!glfwInit()) {
		std::cout << "GLFW couldn't initialize!" << std::endl;
		return -1;
	}

	//Width, Height, Name, Display, Shared resources
	window = glfwCreateWindow(640, 480, "Hello Window", NULL, NULL);

	//Make glfw notice this window
	glfwMakeContextCurrent(window);

	//Ensure OpenGL is loaded
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	//Set color
	glClearColor(0.75f, 0.5f, 0.75f, 1.0f);
	int w, h;

	TriangleMesh* triangle = new TriangleMesh();
	Material* material = new Material("../img/space.jpg");
	Material* mask = new Material("../img/vignette.jpg");

	unsigned int shader = make_shader(
			"../src/shaders/vertex.txt",
			"../src/shaders/fragment.txt"
			);

	glUseProgram(shader);

	//Set texture positions
	glUniform1i(glGetUniformLocation(shader, "material"), 0);
	glUniform1i(glGetUniformLocation(shader, "mask"), 1);

	vec3 quad_position = {0.1f, -0.2f, 0.0f};
	unsigned int model_location = glGetUniformLocation(shader, "model");

	//Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Main loop
	while (!glfwWindowShouldClose(window)) {
		//Watch for events
		glfwPollEvents();
		mat4 model = create_z_rotation(10 * glfwGetTime());
		glUniformMatrix4fv(model_location, 1, GL_FALSE, model.entries);

		//Reset screen color
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		//Draw to screen
		material->use(0);
		mask->use(1);
		triangle->draw();

		//Apply buffer reset
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);
	delete triangle;
	delete material;
	delete mask;
	glfwTerminate();
	return 0;
}

//Function to compile shaders
unsigned int make_module(const std::string& filepath, unsigned int module_type) {
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);

	while (std::getline(file, line)) {
		bufferedLines << line << "\n";
	}

	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();

	bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);

	//Shader, Num elements, C string pointer with shader source code, length of source code
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

	//Ensure shader compiled
	if (!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
	}

	return shaderModule;
}

//Function to make shaders
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath){
	std::vector<unsigned int> modules;

	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();

	for (unsigned int shaderModule : modules) { 
		glAttachShader(shader, shaderModule);
	}

	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	//Ensure shaders were linked
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << std::endl;
	}

	for (unsigned int shaderModule : modules) { 
		glDeleteShader(shaderModule);
	}

	return shader;
}
