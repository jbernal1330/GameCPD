// INCLUDEs FILE
#include "libs.h"

// Vertices para el gráfico
Vertex vertices[] =
{
	// Position                         // Color (R,G,B)                 // Texcoords
	glm::vec3(-0.5f, 0.5f, 0.f),        glm::vec3(1.f, 0.f, 0.f),        glm::vec2(0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),       glm::vec3(0.f, 1.f, 0.f),        glm::vec2(0.f, 0.f),
	glm::vec3(0.5f, -0.5f, 0.f),        glm::vec3(0.f, 0.f, 1.f),        glm::vec2(1.f, 0.f),
	glm::vec3(0.5f, 0.5f, 0.f),         glm::vec3(1.f, 1.f, 0.f),        glm::vec2(1.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

// Creación de triángulos a partir de vértices
GLuint indices[] =
{
	0, 1, 2, // Triangle 1 
	//0, 2, 3  // Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

// INPUT MANAGING
void updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// FRAMEBUFFER
void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

// SHADERS
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

// MAIN PROGRAM
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
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // ¿Se puede cambiar el tamaño de la ventana?

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

	// OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST); // Permite la profundidad de campo en el juego

	glEnable(GL_CULL_FACE); // No se dibujará lo que no se muestra al jugador
	glCullFace(GL_BACK); // No se dibuja lo que está detrás del jugador
	glFrontFace(GL_CCW); // CCW --> Counter Clock Wise --> Dibujo de los vértices en sentido contrario de las manecillas del reloj

	glEnable(GL_BLEND); // Blend colors
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Características del blend

	// Se rellenan los triángulos según su color
	// GL_LINE es otra opción
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Los polígonos se dibujaran atrás y delante mediante Fill

	// SHADER INIT
	GLuint core_program;
	if (!loadShaders(core_program)) {
		glfwTerminate();
	}

	// MODEL - RENDERING

	// VAO, VBO, EBO
	// GEN VAO AND BIND
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// GEN VBO, BIND AND SEND DATA
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// GEN EBO, BIND AND SEND DATA --> Element Buffer Object
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)); // Manejo de bits de almacenamiento
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color)); // Manejo de bits de almacenamiento
	glEnableVertexAttribArray(1);
	// Texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord)); // Manejo de bits de almacenamiento
	glEnableVertexAttribArray(2);

	// BIND VAO 0
	glBindVertexArray(0);

	// TEXTURE INIT
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image("images/logoUSA.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

	GLuint texture0; // Textura + ID
	glGenTextures(1, &texture0); // Cuántas texturas + textura
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Si la textura no cubre lo suficiente del espacio, se repite hasta hacerlo
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Antializing si se hace zoom en la imagen/textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D); // Mipmap --> OpenGL hace versiones pequeñas y grandes de una imagen
	}
	else {
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture --> 0 = No active texture
	SOIL_free_image_data(image); // Eliminar imagen de la memoria

	// MAIN LOOP 
	while(!glfwWindowShouldClose(window)) { // Mientras la ventana no sea cerrada
		// UPDATE INPUT
		glfwPollEvents(); // Permite interactuar con la ventana (cerrar, minimizar, etc)

		// UPDATE
		updateInput(window);

		// DRAW
		// *Clear
		glClearColor(0.f, 0.f, 0.f, 1.f); // (R,G,B) | 4 arg para transparencia --> 1.f = no transparente
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// *Use a program
		glUseProgram(core_program);

		// Update uniforms
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);

		// *Activate texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		// *Bind vertex array object
		glBindVertexArray(VAO);
		
		// *Draw
		// glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); Función de respaldo, no usa índices
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0); 


		// *End Draw
		glfwSwapBuffers(window); // Une los front y back buffers para cerrar el dibujado
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	// DELETE PROGRAM
	glDeleteProgram(core_program);

	return 0;
}