#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

#include "extra_tools/Window.h"
#include "extra_tools/camera.h"
#include "extra_tools/3d_object.h"

#include "external_lib/skybox.h"
using Skybox = Learus_Skybox::Skybox;

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void allCallbacks();

// windows
const unsigned int SCR_WIDTH = 450;
const unsigned int SCR_HEIGHT = 400;
GLFWwindow *window;

// camera
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0, 0, 60));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// stoping Animation
bool show = true;

int main() {
    Window win(SCR_WIDTH, SCR_HEIGHT, "OpenGL", true);
    window = win.window;
    if (window == NULL) {
        std::cout << "Failed to initWindow \n";
        return -1;
    }
    allCallbacks();

//    """"""""""""""""""""""""""""""""""""""""""""""""""""""""

    // build and compile our shader zprogram
    // ------------------------------------

    Skybox skyBox("resources/textures/skybox/top.png", "resources/textures/skybox/bottom.png",
                  "resources/textures/skybox/left.png", "resources/textures/skybox/right.png",
                  "resources/textures/skybox/front.png", "resources/textures/skybox/back.png");

    // lighting
    glm::vec3 lightPos(20.2f, 1.0f, 2.0f);

    Object obj1(mat4(1.0f));
    obj1.loadObj("resources/models/globe.obj");
    obj1.bind_VAO_VBO();
    obj1.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    obj1.setObjectColor(1.0f, 0.5f, 0.31f);
    obj1.setLightColor(0.92f, 1.0f, 0.0f);


    LightObj obj2(mat4(1.0f), lightPos);
    obj2.loadObj("resources/models/moon.obj");
    obj2.bind_VAO_VBO();
    obj2.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    obj2.setObjectColor(0.92f, 1.0f, 0.0f);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // ----------------
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skyBox.setUniforms(camera.UpdatedProjection(), glm::mat4(glm::mat3(camera.UpdatedView())));

        // animation
        //-----------------------------------------
        if (show) {
            obj1.Model = rotate(obj1.Model, radians(float(-3-3)), vec3(1.0f, 1.0f, 0.0f));

            obj2.Model = translate(obj2.Model, vec3(0.0f, 0.0f, 0.1f));
            obj2.Model =rotate(obj2.Model, radians(float(-0.3)), vec3(0.0f, 0.01f, 0.0f));
            lightPos = obj2.Model[3];
        }


        skyBox.Draw();

        obj1.updateLightPos(lightPos);
        obj1.updateViewPos(camera);
        obj1.drawObject(camera);

//        obj2.Model = translate(obj2.Model, vec3(0.0f, 0.0f, 0.1f));
//        obj2.Model =rotate(obj2.Model, radians(float(-0.3)), vec3(0.0f, 0.01f, 0.0f));
//        lightPos = obj2.Model[3];

        obj2.drawObject(camera);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    obj1.delete_VAO_Buffers();
    obj2.delete_VAO_Buffers();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        show = !show;
}


void allCallbacks() {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

