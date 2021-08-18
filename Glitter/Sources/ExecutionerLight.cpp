// Local Headers
#include "ExecutionerLight.h"
#include "ShaderLoader.h"
#include "GLFWManager.h"
#include "utils.h"
#include "camera.h"
#include "Material.h"
#include "Light.h"

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

void framebuffer_size_callback_light(GLFWwindow* window, int width, int height);
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

    // lighting
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

    GLFWManager glfwManager(mWidth, mHeight, "OpenGL");

    glfwSetFramebufferSizeCallback(glfwManager.GetWindow(), framebuffer_size_callback_light);
    glfwSetCursorPosCallback(glfwManager.GetWindow(), mouse_callback_light);
    glfwSetScrollCallback(glfwManager.GetWindow(), scroll_callback_light);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(glfwManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader(utils::getPath("materials_diffuse_map.vs", utils::fileType::SHADER).c_str(),
        utils::getPath("materials_diffuse_map.fs", utils::fileType::SHADER).c_str());
    Shader lightCubeShader(utils::getPath("1.light_cube.vs", utils::fileType::SHADER).c_str(),
        utils::getPath("1.light_cube.fs", utils::fileType::SHADER).c_str());


    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Load texture 
    unsigned int diffuseMap = utils::loadTexture("container2.png");
    unsigned int specularMap = utils::loadTexture("container2_specular.png");

    // shader configuration
    lightingShader.use(); 
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    // Set Light 
    Light light;
    light.ambient = glm::vec3(0.2f);
    light.diffuse = glm::vec3(0.5f);
    light.specular = glm::vec3(1.0f);
    light.position = glm::vec4(lightPos, 1.0f);


    // Init matrices as Identities
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    //set light poosition 
    glm::vec3 posOff = glm::vec3(0.0f,0.0f,0.0f);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

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
        if (glfwManager.WasKeyPressed(GLFW_KEY_SPACE))
            this->_camera.ProcessKeyboard(UP, this->_deltaTime);
        if (glfwManager.WasKeyPressed(GLFW_KEY_LEFT_SHIFT))
            this->_camera.ProcessKeyboard(DOWN, this->_deltaTime);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec4("light.position", light.position);
        lightingShader.setVec3("viewPos", this->_camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient", light.ambient); 
        lightingShader.setVec3("light.diffuse", light.diffuse);
        lightingShader.setVec3("light.specular", light.specular);

        // material properties
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 64.0f);

        lightingShader.setFloat("light.constant", light.constant);
        lightingShader.setFloat("light.linear", light.linear);
        lightingShader.setFloat("light.quadratic", light.quadratic);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(this->_camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        glm::mat4 view = this->_camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        // lightingShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        // render the cube
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(cubeVAO);

        for(unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        
        // rotate light
        float angle = glfwGetTime() * 20.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);
        glm::vec3 newLightPos = model[3];
        light.position = glm::vec4(newLightPos, 1.0f); // update light position 
        lightingShader.setVec4("light.position", light.position); 	
        
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events
        glfwManager.SwapBuffers();
        glfwManager.PollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwManager.TerminateAll();
    return EXIT_SUCCESS;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback_light(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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