/*
 Universidad Sergio Arboleda
 Computaci�n Paralela y Distribuida
 Proyecto final 3er Corte

 Hecho por: Juan Bernal, ...
*/

// INCLUDEs FILE
#include "libs.h"

// Vertices para el gr�fico
Vertex vertices[] =
{
	// Position                         // Color (R,G,B)                 // Texcoords               // Normals
	glm::vec3(-0.5f, 0.5f, 0.f),        glm::vec3(1.f, 0.f, 0.f),        glm::vec2(0.f, 1.f),       glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),       glm::vec3(0.f, 1.f, 0.f),        glm::vec2(0.f, 0.f),	    glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, -0.5f, 0.f),        glm::vec3(0.f, 0.f, 1.f),        glm::vec2(1.f, 0.f),	    glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, 0.5f, 0.f),         glm::vec3(1.f, 1.f, 0.f),        glm::vec2(1.f, 1.f),	    glm::vec3(0.f, 0.f, -1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

// Creaci�n de tri�ngulos a partir de v�rtices
GLuint indices[] =
{
	0, 1, 2, // Triangle 1
	0, 2, 3  // Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

// INPUT MANAGING
void updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
	// Movimiento adelante
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position.z -= 0.01f;
	}
	// Movimiento atr�s
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position.z += 0.01f;
	}
	// Movimiento izquierda
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position.x -= 0.01f;
	}
	// Movimiento derecha
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position.x += 0.01f;
	}
	// Giro izquierda
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		rotation.y -= 5.f;
	}
	// Giro derecha
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		rotation.y += 5.f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		scale += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		scale -= 0.1f;
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

	glAttachShader(program, vertexShader); // A�adir shader de v�rtices al programa
	glAttachShader(program, fragmentShader); // A�adir shader de fragmentos (pixeles) al programa

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
	const int WINDOW_WIDTH = 1440;
	const int WINDOW_HEIGHT = 1024;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // MAJOR.MINOR --> Versi�n de Open GL, en este caso, se usa 4.4 (MAJOR.MINOR)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // �Se puede cambiar el tama�o de la ventana?

	// * glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) --> SOLO PARA MAC OS

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game CPD", NULL, NULL); // Ancho, alto, t�tulo, fullscreen, ventana

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // Frame buffer para la ventana
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // Actualiza el Frame buffer al cambiar de tama�o la ventana

	glViewport(0, 0, framebufferWidth, framebufferHeight); // �Qu� tanto de la ventana se dibujar�? --> De 0 a los valores seleccionados en las variables framebuffer

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

	glEnable(GL_CULL_FACE); // No se dibujar� lo que no se muestra al jugador
	glCullFace(GL_BACK); // No se dibuja lo que est� detr�s del jugador
	glFrontFace(GL_CCW); // CCW --> Counter Clock Wise --> Dibujo de los v�rtices en sentido contrario de las manecillas del reloj

	glEnable(GL_BLEND); // Blend colors
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Caracter�sticas del blend

	// Se rellenan los tri�ngulos seg�n su color
	// GL_LINE es otra opci�n
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Los pol�gonos se dibujaran atr�s y delante mediante Fill

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
	// Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)); // Manejo de bits de almacenamiento
	glEnableVertexAttribArray(3);

	// BIND VAO 0
	glBindVertexArray(0);

	// TEXTURE INIT

	// Texture 0
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image("images/logoUSA.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

	GLuint texture0; // Textura + ID
	glGenTextures(1, &texture0); // Cu�ntas texturas + textura
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Si la textura no cubre lo suficiente del espacio, se repite hasta hacerlo
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Antializing si se hace zoom en la imagen/textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D); // Mipmap --> OpenGL hace versiones peque�as y grandes de una imagen
	}
	else {
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture --> 0 = No active texture
	SOIL_free_image_data(image); // Eliminar imagen de la memoria

	// Texture 1
	int image_width1 = 0;
	int image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("images/USA-campus.jpeg", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1; // Textura + ID
	glGenTextures(1, &texture1); // Cu�ntas texturas + textura
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Si la textura no cubre lo suficiente del espacio, se repite hasta hacerlo
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Antializing si se hace zoom en la imagen/textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D); // Mipmap --> OpenGL hace versiones peque�as y grandes de una imagen
	}
	else {
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture --> 0 = No active texture
	SOIL_free_image_data(image1); // Eliminar imagen de la memoria

	// INIT MODEL MATRIX
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ModelMatrix(1.f); // Inicializar matriz 4x4
	ModelMatrix = glm::translate(ModelMatrix, position); // Translaci�n de ModelMatrix
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Rotaci�n en X
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Rotaci�n en Y
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Rotaci�n en Z
	ModelMatrix = glm::scale(ModelMatrix, scale); // Escalado --> Aumenta el tama�o de la imagen

	// CAMERA
	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = 0.1f; // Para que no se pegue demasiado al cuerpo --> Evitar Clipping
	float farPlane = 1000.f; // Rango m�ximo de visi�n
	glm::mat4 ProjectionMatrix(1.f);

	ProjectionMatrix = glm::perspective(glm::radians(fov), 
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane
	);

	glUseProgram(core_program);

	// LIGHTS
	glm::vec3 lightPos0(0.f, 0.f, 2.f);

	// INIT UNIFORMS
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)); // Primera inicializaci�n
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glUniform3fv(glGetUniformLocation(core_program, "lightPos0"), 1, glm::value_ptr(lightPos0));
	glUniform3fv(glGetUniformLocation(core_program, "cameraPos"), 1, glm::value_ptr(camPosition));

	glUseProgram(0);



	// MAIN LOOP 
	while (!glfwWindowShouldClose(window)) { // Mientras la ventana no sea cerrada
		// UPDATE INPUT
		glfwPollEvents(); // Permite interactuar con la ventana (cerrar, minimizar, etc)
		updateInput(window, position, rotation, scale); // Movimientos

		// UPDATE
		updateInput(window);

		// DRAW
		// *Clear
		glClearColor(0.f, 0.f, 0.f, 1.f); // BACKGROUND (R,G,B,A) | 4 arg para transparencia --> 1.f = no transparente
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// *Use a program
		glUseProgram(core_program);

		// Update uniforms
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

		// Move, rotate and scale
		/*position.z -= 0.01f;
		rotation.y += 2.f;
		scale.x += 0.01f;*/

		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position); // Movimiento
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Rotaci�n en X
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Rotaci�n en Y
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Rotaci�n en Z
		ModelMatrix = glm::scale(ModelMatrix, scale); // Escalado --> Aumenta el tama�o de la imagen

		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);

		glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		// *Activate texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// *Bind vertex array object
		glBindVertexArray(VAO);
		
		// *Draw
		// glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); Funci�n de respaldo, no usa �ndices
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