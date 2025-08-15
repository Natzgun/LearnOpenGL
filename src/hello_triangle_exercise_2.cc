#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main() {\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char * primaryFSsource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "    FragColor = vec4(0.07f, 0.13f, 0.36f, 1.0f);\n"
  "}\0"; 

const char * secondaryFSsource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "    FragColor = vec4(0.17f, 0.47f, 0.70f, 1.0f);\n"
  "}\0"; 

int main(int argc, char *argv[]) {
  /* Esto inicializa GLFW con sus valores predeterminados, retorna GLFW_TRUE si
   * tiene exito
   * */
  glfwInit();

  /* Esto define la version de opengl con la que se quiere trabajar en este caso
   * la version 3.3 */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  /* EL primer argumento es la opcion para seleccionar, en este caso el perfil,
   * y el segundo argumento es el valor de esta opcion, core profile va excluir
   * funciones de compatibilidad antiguas*/
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Full OpenGL", NULL, NULL);

  if (window == NULL) {
    std::cout << "Fail to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  /* Indicamos a GLFW que convierta el contexto de nuestra ventana en el
   * contexto principal del hilo actual*/
  glfwMakeContextCurrent(window);
  /* Aqui glfw esta llamando a mi callback por mi cuando se haga un resize de la
   * ventana, el callback tiene que cumplir el contrato que espera como segundo
   * argumento de la funcion (puede llamarse como quiera). Un Callback es una
   * funcion que yo defino pero que otro programa la llama por mi cuando ocurra
   * cierto evento*/
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* gladLoadGLLoader necesita cargar los punteros de funcion de opengl es por
   * eso que antes de llamar a cualquier funcion de openGL necesitamos
   * cargarlos, el parametro que recibe es una funcion proporcionada por GLFW
   * que carga estos punteros segun el sistem operativo*/
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Fail to initialize GLAD" << std::endl;
    return -1;
  }

  /* ----------- SETUP SHADERS -----------*/
  /* Crea un object shader que se va identificar con un unsigned int, y luego
   * cramos el shader, el argumento es el tipo de shader, por lo que este object
   * shader sera un vertex shader */
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  /*Ahora enlazamos el codigo del shader con shader object (vertex shader en
   * este caso) y luego compilamos el shader, el segundo argumento es apra
   * indicar cuantos string le vamos a pasar en este caso solo 1, el ultimo
   * argument es un arreglo de longitudes en este caso NULL hace referencia a
   * que encontraremos un \0 al final*/
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  /* Revisamos si la compilacion del shader fue exitosa */
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  /*La funcion principal de un Fragment shader es calcular el color de salida de
   * los pixeles, el FS siembre generará un color anaranjado*/

  unsigned int fragmentShader[2];
  // ===================== PRIMARY COLOR =======================
  fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader[0], 1, &primaryFSsource, NULL);
  glCompileShader(fragmentShader[0]);
 
  // Verificamos que no haya error en la compilacion
  glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // ===================== SECONDARY COLOR =======================
  fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader[1], 1, &secondaryFSsource, NULL);
  glCompileShader(fragmentShader[1]);
 
  // Verificamos que no haya error en la compilacion
  glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  /*Un Shader Program es la version final enlazada de varios shaders combinados,
   * asi que creamos uno*/
  unsigned int shaderProgram[2];
  shaderProgram[0] = glCreateProgram();

  /*Ahora adjuntamos los shaders previamente compilados  al Program Object*/
  glAttachShader(shaderProgram[0], vertexShader);
  glAttachShader(shaderProgram[0], fragmentShader[0]);
  glLinkProgram(shaderProgram[0]);

  glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
  }

  /* Podemos activar este Program Object con glUseProgram(shaderProgram), y no olvidemos eliminar los shaders ya vinculados, podemos ponerlo al final del codigo */
  shaderProgram[1] = glCreateProgram();

  /*Ahora adjuntamos los shaders previamente compilados  al Program Object*/
  glAttachShader(shaderProgram[1], vertexShader);
  glAttachShader(shaderProgram[1], fragmentShader[1]);
  glLinkProgram(shaderProgram[1]);

  glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader[0]);
  glDeleteShader(fragmentShader[1]);

  /* ------------ SETUP VERTEX DATA ------------*/
  /* Verices del triangulo pero estas coordenadas estan en NDC (Normalized
   * Device Corrdinates) que van desde -1.0 hasta 1.0, cualquier valor fuera de
   * este rango OpenGL no los mostrará*/

  float verticesLeftTriangles[] {
    -0.6f,  0.7f,  0.0f, // 0 
    -0.6f, -0.7f,  0.0f, // 2
    -0.3f, -0.7f,  0.0f, // 3

    0.3f,  0.7f,  0.0f,  // 4
    0.6f,  0.7f, 0.0f,   // 5
    
    0.3f, -0.7f, 0.0f,   // 6
    -0.6f, 0.5f,  0.0f,  // 8
    0.6f, -0.5f, 0.0f,   // 9
  };

  float verticesRightTriangles[] {
    -0.6f,  0.7f,  0.0f, // 0 
    -0.3f,  0.7f,  0.0f, // 1
    -0.3f, -0.7f,  0.0f, // 3

    0.6f,  0.7f, 0.0f,   // 5
    0.3f, -0.7f, 0.0f,   // 6
    0.6f, -0.7f,  0.0f,  // 7

    -0.6f, 0.5f,  0.0f,  // 8
    0.6f, -0.5f, 0.0f,   // 9
  };

  // Podemos cargar solo 4 vertices y con esos 4 vertices podemos dibujar 2
  // triangulos los indices son las posiciones de los vertices, asi evitamos
  // sobreposicion de vertices
  unsigned int indicesLeftTriangle[] = {
      0, 1, 2,
      3, 4, 5,
      5, 6, 7,
  };

  unsigned int indicesRightTriangle[] = {
      0, 1, 2,
      3, 4, 5,
      1, 6, 7,
  };

  /* Aqui creamos un Vertex buffer object, generamos ese buffer que viene desde
   * la la GPU es como si reservaramos memoria*/
  unsigned int VBO[2], VAO[2], EBO[2];
  /* Vertex Array Object (VAO)*/
  /* Una VAO nos sirve para almacenar configuracion de nuestros atributos de vertice y que VBO usar*/
  // ================= Left GL_TRIANGLES =====================
  glGenVertexArrays(1, &VAO[0]);
  glGenBuffers(1, &VBO[0]);
  glGenBuffers(1, &EBO[0]);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLeftTriangles), verticesLeftTriangles, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLeftTriangle), indicesLeftTriangle, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);


  // ================== Right TRIANGLES =====================
  glGenVertexArrays(1, &VAO[1]);
  glGenBuffers(1, &VBO[1]);
  glGenBuffers(1, &EBO[1]);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRightTriangles), verticesRightTriangles, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRightTriangle), indicesRightTriangle, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);



  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
  
  // Podemos entrar en modo wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* La condicion revisa en cada loop si hay una instruccion que va cerrar la
   * ventana */
  while (!glfwWindowShouldClose(window)) {

    // --- Input ---
    processInput(window);

    // -- Funciones de render ---

    // Define el color con el que se va limpiar el color buffer, osea cuando
    // limpie el color buffer del frame anterior lo va llenar con estre color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    /* Borra el contenido del color buffer osea del frame anterior y lo rellena
     * con el color definido por glClearColor*/
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the object
    glUseProgram(shaderProgram[0]);
    // Aqui le decimos que use el VAO con la configuracion que ya guardamos antes
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glUseProgram(shaderProgram[1]);
    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    /* glfwSwapBuffers(window) intercambia el back buffer (donde OpenGL dibuja)
    con el front buffer (lo que se ve en pantalla). Durante cada frame, todo
    se dibuja primero en el back buffer, que es básicamente una imagen 2D con
    los colores de cada píxel (color buffer). Una vez que el frame está
    listo, glfwSwapBuffers lo muestra en la ventana. */
    glfwSwapBuffers(window);
    /* Verifica si un evento se ha activado, en base a ella actualiza el estado
     * de la ventana y llama a las funciones callback que yo haya registrado*/
    glfwPollEvents();
  }

  /*Limpiamos los recursos de GLFW asignados*/
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  /* Aqui verificamos si el ultimo estado del teclado ha sido presionado para
   * una ventana especifica*/
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  /* Los primeros dos puntos establece la ubicacion de la esquina inferior
   * izquiera de la ventana, los dos ultimos define el ancho y la altura de la
   * ventana de renderizado en pixeles*/
  glViewport(0, 0, width, height);
}
