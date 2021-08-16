// Local Headers
#include "ExecutionerLight.h"
#include "ShaderLoader.h"
#include "GLFWManager.h"
#include "utils.h"
#include "camera.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 


#ifdef WINBUILD
#include <windows.h>
#else
#include <unistd.h>
#endif


static ExecutionerLight* exec;

void framebuffer_size_callback_light(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void mouse_callback_light(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_light(GLFWwindow* window, double xoffset, double yoffset);

int ExecutionerLight::run() {
    exec = this;

    // camera
    this->_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    this->_lastX = mWidth / 2.0f;
    this->_lastY = mHeight / 2.0f;
    this->_firstMouse = true;

    // timing
    this->_deltaTime = 0.0f;	// time between current frame and last frame
    this->_lastFrame = 0.0f;


    GLFWManager glfwManager(mWidth, mHeight, "OpenGL");

    glfwSetFramebufferSizeCallback(glfwManager.GetWindow(), framebuffer_size_callback_light);
    glfwSetCursorPosCallback(glfwManager.GetWindow(), mouse_callback_light);
    glfwSetScrollCallback(glfwManager.GetWindow(), scroll_callback_light);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader CubeShader(utils::getPath("1.colors.vs", utils::fileType::SHADER).c_str(),
        utils::getPath("1.colors.fs", utils::fileType::SHADER).c_str());
    Shader LightShader(utils::getPath("1.light_cube.vs", utils::fileType::SHADER).c_str(),
        utils::getPath("1.light_cube.fs", utils::fileType::SHADER).c_str());


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    CubeShader.use(); // don't forget to activate/use the shader before setting uniforms!
    CubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    CubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // Init matrices as Identities
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    CubeShader.setMat4("projection", projection);
    LightShader.setMat4("projection", projection);

    //set light poosition 
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 posOff = glm::vec3(0.0f,0.0f,0.0f);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // Enable Z-buffer testing
    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwManager.ShouldClose())
    {
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            //printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            //printf("%d fps\n", nbFrames);
            nbFrames = 0;
            lastTime += 1.0;
        }
        // per-frame time logic
        this->_deltaTime = currentTime - this->_lastFrame;
        this->_lastFrame = currentTime;

        // process input
        if (glfwManager.WasKeyPressed(GLFW_KEY_ESCAPE) )
            glfwManager.SetShouldClose(true);
        if (glfwManager.WasKeyPressed(GLFW_KEY_R))
            this->_camera.Position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (glfwManager.WasKeyPressed(GLFW_KEY_W))
            this->_camera.ProcessKeyboard(FORWARD, this->_deltaTime);
        if (glfwManager.WasKeyPressed(GLFW_KEY_S))
            this->_camera.ProcessKeyboard(BACKWARD, this->_deltaTime);
        if (glfwManager.WasKeyPressed(GLFW_KEY_A))
            this->_camera.ProcessKeyboard(LEFT, this->_deltaTime);
        if (glfwManager.WasKeyPressed(GLFW_KEY_D))
            this->_camera.ProcessKeyboard(RIGHT, this->_deltaTime);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(_camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        glm::mat4 view = _camera.GetViewMatrix();

        // also draw the lamp object
        LightShader.use();
        LightShader.setMat4("projection", projection);
        LightShader.setMat4("view", view);
        glm::mat4  model = glm::mat4(1.0f);
        float angle = glfwGetTime() * 25.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        LightShader.setMat4("model", model);
        glm::vec3 newLightPos = glm::vec3(model[3]);
        // update light position 

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // be sure to activate shader when setting uniforms/drawing objects
        CubeShader.use();
        CubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        CubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //CubeShader.setVec3("lightPos", lightPos);
        CubeShader.setVec3("lightPos", newLightPos);

        CubeShader.setMat4("projection", projection);
        CubeShader.setMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        CubeShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events
        glfwManager.SwapBuffers();
        glfwManager.PollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwManager.TerminateAll();
    return EXIT_SUCCESS;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback_light(GLFWwindow* window, double xpos, double ypos)
{
    if (exec->_firstMouse)
    {
        exec->_lastX = xpos;
        exec->_lastY = ypos;
        exec->_firstMouse = false;
    }

    float xoffset = xpos - exec->_lastX;
    float yoffset = exec->_lastY - ypos; // reversed since y-coordinates go from bottom to top

    exec->_lastX = xpos;
    exec->_lastY = ypos;

    exec->_camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback_light(GLFWwindow* window, double xoffset, double yoffset)
{
    exec->_camera.ProcessMouseScroll(yoffset);
}