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

const char * fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

  /* Aqui creamos un Vertex buffer object, generamos ese buffer que viene desde
   * la la GPU es como si reservaramos memoria*/
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  /* Ahora enlazamos ese buffer a su tipo correspondiente en OpenGL que vendria
   * ser GL_ARRAY_BUFFER, entonces cuando modifiquemos GL_ARRAY_BUFFER vamos a
   * estar modificando  VBO*/
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  /* Verices del triangulo pero estas coordenadas estan en NDC (Normalized
   * Device Corrdinates) que van desde -1.0 hasta 1.0, cualquier valor fuera de
   * este rango OpenGL no los mostrará*/
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
  };

  /* Transfiere los vertices en el rango NDC hacia el buffer de vertexs, el
   * ultimo parametro es para indicarle como debemos manejar la CPU, en este
   * caso GL_STATIC_DRAW setea los valores una vez y lo usamos muchas veces */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
 
  // Verificamos que no haya error en la compilacion
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  /*Un Shader Program es la version final enlazada de varios shaders combinados,
   * asi que creamos uno*/
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  /*Ahora adjuntamos los shaders previamente compilados  al Program Object*/
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
  }

  /* Podemos activar este Program Object con glUseProgram(shaderProgram), y no olvidemos eliminar los shaders ya vinculados, podemos ponerlo al final del codigo */
  


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

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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
