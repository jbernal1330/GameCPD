
#include "libs.h"

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint& program) {
	bool loadSuccess = true;

	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	// Vertex
	in_file.open("vertex_core.glsl");

	if (in_file.is_open()) {
		while (std::getline(in_file, temp)) {
			src += temp + "\n";
		}
	}
	else {
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
		loadSuccess = false;
	}

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	temp = "";
	src = "";

	// Fragment
	in_file.open("fragment_core.glsl");

	if (in_file.is_open()) {
		while (std::getline(in_file, temp)) {
			src += temp + "\n";
		}
	}
	else {
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	// Program
	program = glCreateProgram();

	glAttachShader(program, vertexShader); // Añadir shader de vértices al programa
	glAttachShader(program, fragmentShader); // Añadir shader de fragmentos (pixeles) al programa

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	// End
	glUseProgram(0); // Reinicia el programa seleccionado
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

int main() {

	// GLFW INIT
	glfwInit();

	// CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // MAJOR.MINOR --> Versión de Open GL, en este caso, se usa 4.4 (MAJOR.MINOR)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // ¿Se puede cambiar el tamaño de la ventana?

	// * glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) --> SOLO PARA MAC OS

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game App", NULL, NULL); // Ancho, alto, título, fullscreen, ventana

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // Actualiza el Frame buffer al cambiar de tamaño la ventana

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // Frame buffer para la ventana
	glViewport(0, 0, framebufferWidth, framebufferHeight); // ¿Qué tanto de la ventana se dibujará? --> De 0 a los valores seleccionados en las variables framebuffer

	glfwMakeContextCurrent(window); // Contexto en la ventana --> IMPORTANTE PARA GLEW

	// INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	// ERROR MANAGING --> Verifica que GLEW se haya inicializado correctamente
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	// SHADER INIT
	GLuint core_program;
	if (!loadShaders(core_program)) {
		glfwTerminate();
	}

	// MAIN LOOP 
	while(!glfwWindowShouldClose(window)) { // Mientras la ventana no sea cerrada
		// UPDATE INPUT
		glfwPollEvents(); // Permite interactuar con la ventana (cerrar, minimizar, etc)

		// UPDATE

		// DRAW
		// *Clear
		glClearColor(0.f, 1.f, 1.f, 1.f); // (R,G,B) | 4 arg para transparencia --> 1.f = no transparente
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// *Draw

		// *End Draw
		glfwSwapBuffers(window); // Une los front y back buffers para cerrar el dibujado
		glFlush();
	}

	// END OF PROGRAM
	glfwTerminate();

	return 0;
}