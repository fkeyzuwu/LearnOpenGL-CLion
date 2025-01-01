#include <complex>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "src/camera.h"
#include "src/Shader.h"

#include "stb_image/stb_image.h"
#include "glfw/deps/linmath.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "src/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void setupLightingStaticUniforms(Shader& lightingShader, glm::vec3 lightPositions[]);
unsigned int generateTexture(const char* filePath);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    stbi_set_flip_vertically_on_load(true);

    unsigned int diffuseMap = generateTexture("../assets/container2.png");
    unsigned int specularMap = generateTexture("../assets/container2_specular.png");
    //unsigned int emissionMap = generateTexture("../matrix.jpg");

    Shader shader("../src/shaders/model_loadingvs.glsl", "../src/shaders/model_loadingfs.glsl");

    Model model("../assets/backpack_model/backpack.obj");

    static float lastTime = 0.f;

    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // drawing cube
        shader.use();

        camera.update(window, deltaTime);

        //glm::vec3 lightColor = glm::vec3(1.0);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model_mat = glm::scale(model_mat, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model_mat);
        model.Draw(shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scroll_callback(window, xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void setupLightingStaticUniforms(Shader& shader, glm::vec3 pointLightPositions[])
{
    glm::vec3 ambient = glm::vec3(0.05f);
    glm::vec3 diffuse = glm::vec3(0.8);
    glm::vec3 specular = glm::vec3(1.0f);

    shader.use();

    shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    //material
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);

    // directional light
    shader.setVec3("dirLight.ambient", ambient);
    shader.setVec3("dirLight.diffuse", 0.4, 0.4, 0.4);
    shader.setVec3("dirLight.specular", specular);
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);

    // point lights
    for (int i = 0; i < 4; ++i)
    {
        std::string pluniform = "pointLights["+std::to_string(i)+"].";

        shader.setVec3(pluniform + "ambient", ambient);
        shader.setVec3(pluniform + "diffuse",  diffuse); // darken diffuse spotLight a bit
        shader.setVec3(pluniform + "specular", specular);

        shader.setVec3(pluniform + "position", pointLightPositions[i]);

        shader.setFloat(pluniform + "constant",  1.0f);
        shader.setFloat(pluniform + "linear",    0.09f);
        shader.setFloat(pluniform + "quadratic", 0.032f);
    }

    // spot light
    shader.setVec3("spotLight.ambient",  0.0, 0.0, 0.0);
    shader.setVec3("spotLight.diffuse",  1.0, 1.0, 1.0); // darken diffuse spotLight a bit
    shader.setVec3("spotLight.specular", specular);

    shader.setFloat("spotLight.cutOff",   glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff",   glm::cos(glm::radians(17.5f)));

    shader.setFloat("spotLight.constant",  1.0f);
    shader.setFloat("spotLight.linear",    0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setVec3("spotLight.position",  camera.position);
    shader.setVec3("spotLight.direction", camera.front);
}

unsigned int generateTexture(const char* filePath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}