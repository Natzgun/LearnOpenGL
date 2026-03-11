#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

float xMove = 0.0f;
float yMove = 0.0f;

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

  glfwWindowHintString(GLFW_WAYLAND_APP_ID, "opengl-project");
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
  Shader ourShader("../../src/shaders/shader.vert",
                   "../../src/shaders/shader.frag");

  /* ------------ SETUP VERTEX DATA ------------*/
  /* Verices del triangulo pero estas coordenadas estan en NDC (Normalized
   * Device Corrdinates) que van desde -1.0 hasta 1.0, cualquier valor fuera de
   * este rango OpenGL no los mostrará*/

  float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };
  // Podemos cargar solo 4 vertices y con esos 4 vertices podemos dibujar 2
  // triangulos los indices son las posiciones de los vertices, asi evitamos
  // sobreposicion de vertices
  unsigned int indices[] = {
      0, 1, 2, // first triangle
      // 1, 2, 3  // second triangle
  };

  /* Aqui creamos un Vertex buffer object, generamos ese buffer que viene desde
   * la la GPU es como si reservaramos memoria*/
  unsigned int VBO, VAO, EBO;
  /* Vertex Array Object (VAO)*/
  /* Una VAO nos sirve para almacenar configuracion de nuestros atributos de vertice y que VBO usar*/
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  /*Vinculamos el VAO*/
  glBindVertexArray(VAO);

  /* Ahora enlazamos ese buffer a su tipo correspondiente en OpenGL que vendria
   * ser GL_ARRAY_BUFFER, entonces cuando modifiquemos GL_ARRAY_BUFFER vamos a
   * estar modificando  VBO*/
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  /* Transfiere los vertices en el rango NDC hacia el buffer de vertexs, el
   * ultimo parametro es para indicarle como debemos manejar la GPU, en este
   * caso GL_STATIC_DRAW setea los valores una vez y lo usamos muchas veces */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  /*Le decimos a OpenGL como debe interpretar los datos del vertex,
   * (configurarmos los atributos de vertice)*/
  // Position attribute
   // Set the vertex attributes for the position.
   // attribute location | number of components | type | normalize | stride | offset
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);


  /* ----------- SETUP TEXTURES -----------*/
  float texCoords[] = {
      0.0f, 0.0f, // lower-left corner
      1.0f, 0.0f, // lower-right corner
      0.5f, 1.0f  // top-center corner
  };

  // Genera y enlaza un objeto de textura. Luego, carga los datos de la imagen en él
  // y genera mipmaps para un escalado adecuado.
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping parameters.
  // GL_TEXTURE_WRAP_S sets the wrap parameter for the S (or X) coordinate.
  // GL_TEXTURE_WRAP_T sets the wrap parameter for the T (or Y) coordinate.
  // GL_REPEAT repeats the texture image.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


  // Set texture filtering parameters.
  // GL_TEXTURE_MIN_FILTER is used when the texture is smaller than the area it's mapped to (minification).
  // GL_TEXTURE_MAG_FILTER is used when the texture is larger than the area it's mapped to (magnification).
  // GL_LINEAR specifies linear interpolation for filtering, which results in smoother textures.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // We can read a texture
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("../../assets/container.jpg", &width, &height, &nrChannels, 0);

  if (data) {
    // Load the image data into the currently bound 2D texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  // To draw in wireframe mode, uncomment the following line.
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


    // float timeValue = glfwGetTime();
    // int vertexColorLocation =  glGetUniformLocation(shaderProgram, "ourColor");
    // ourShader.use();
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    
    float timeValue = glfwGetTime();

    ourShader.use();
    ourShader.setFloat("xOffset", xMove);
    ourShader.setFloat("yOffset", yMove);
    ourShader.setFloat("time", timeValue);

    // ourShader.setColorRGB("customColor", colors[0], colors[1], colors[2]);
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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
  } else if (glfwGetKey(window, GLFW_KEY_W)) {
    yMove += 0.005;
  } else if (glfwGetKey(window, GLFW_KEY_A)) {
    xMove -= 0.005;
  } else if (glfwGetKey(window, GLFW_KEY_S)) {
    yMove -= 0.005;
  } else if (glfwGetKey(window, GLFW_KEY_D)) {
    xMove += 0.005;
  }


}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  /* Los primeros dos puntos establece la ubicacion de la esquina inferior
   * izquiera de la ventana, los dos ultimos define el ancho y la altura de la
   * ventana de renderizado en pixeles*/
  glViewport(0, 0, width, height);
}
